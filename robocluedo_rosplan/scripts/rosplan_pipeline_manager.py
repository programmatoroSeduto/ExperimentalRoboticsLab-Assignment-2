#! /usr/bin/env python

import rospy
from std_srvs.srv import Empty, EmptyRequest, EmptyResponse
from std_msgs.msg import String
from rosplan_dispatch_msgs.srv import DispatchService, DispatchServiceRequest, DispatchServiceResponse
from robocluedo_rosplan_msgs.msg import ActionFeedback
from robocluedo_rosplan_msgs.srv import RosplanPipelineManagerService, RosplanPipelineManagerServiceRequest, RosplanPipelineManagerServiceResponse
from robocluedo_rosplan_msgs.srv import UpdateGoal, UpdateGoalRequest, UpdateGoalResponse

NODE_NAME = "rosplan_pipeline_manager"
''' nothing to say, node name
'''

SRV_TIMEOUT = rospy.Duration(60)
''' a timeout of 1min used for the client connection
'''

LANDMARK_REPLAN = 0
''' landmark corresponding to : replan
'''

LANDMARK_COLLECT = 1
''' landmark corresponding to : collect
'''

LANDMARK_SOLVE = 2
''' landmark corresponding to : solve
'''

service_problem = "/rosplan_problem_interface/problem_generation_server"
''' name of the trigger of problem interface
'''

cl_problem = None
''' (ros client handle) problem instance trigger
'''

topic_problem_instance = "/rosplan_problem_interface/problem_instance"
''' topic used to understand if the problem generation succeeded
'''

sub_problem_instance = None
''' (ros subscriber handle) 
'''

problem_instance_received = False
''' this flag is set true when the subscriber received a problem instance.
	this means that the problem generation worked as expected. 
'''

problem_instance_text = ""
''' the last problem instance received
'''

service_plan = "/rosplan_planner_interface/planning_server"
''' name of the trigger of planning interface
'''

cl_plan = None
''' (ros client handle) planning interface trigger
'''

service_parse = "/rosplan_parsing_interface/parse_plan"
''' name of the trigger of the plan parser
'''

cl_parse = None
''' (ros client handle) plan parser trigger
'''

service_dispatch = "/rosplan_plan_dispatcher/dispatch_plan"
''' name of the service plan dispatch
'''

cl_dispatch = None
''' (ros client handle) plan dispatcher
'''

topic_action_feedback = "/robocluedo/action_feedback"
''' the topic for receiving the feedback
'''

sub_action_feedback = None
''' (ros subscriber handle) the feedback subscription
'''

service_pipeline = "/robocluedo/pipeline_manager"
''' service for the pipeline command manager
'''

srv_pipeline = None
''' (ros service handle) service handle for the pipeline service
'''

service_update_goal = "/update_goal"
''' used to apply a landmark to the knowledge base
'''

cl_update_goal = None
''' (ros client handle) set a particular goal into the rosplan knowledge base
'''

def cbk_problem_instance( problem ):
	''' this callback received the problem instance from the problem interface
	'''
	
	global problem_instance_received
	global problem_instance_text
	
	problem_instance_received = True
	problem_instance_text = problem.data


def cbk_action_feedback( feedback ):
	''' receive a feedback from the ROSPlan action
	'''
	
	pass


def cbk_pipeline( req ):
	''' implementation of the service pipeline manager
	'''
	
	global cl_problem
	global cl_update_goal
	global problem_instance_received
	global problem_instance_text
	
	res = RosplanPipelineManagerServiceResponse( )
	res.success = True
	res.success_load_problem = True
	res.success_solve_problem = True
	res.problem_not_solvable = False
	res.success_parse_plan = True
	res.success_execute_plan = True
	
	if req.load_problem:
		# needed a landmark!
		if req.landmark < 0 or req.landmark > 2:
			rospy.logwarn(f"({NODE_NAME}) not a valid landmark: {req.landmark}")
			
			res.success_load_problem = False
			res.success = False
			return res
		else:
			rospy.loginfo(f"({NODE_NAME}) problem interface -- loading problem ...")
			
			# set the landmark into the ontology
			newgoal = UpdateGoalRequest( )
			newgoal.landmark = req.landmark
			print(type(req.landmark))
			newgoal_res = cl_update_goal( newgoal )
			if not newgoal_res.success:
				if newgoal_res.applicable:
					rospy.logwarn(f"({NODE_NAME}) unable to apply the landmark {req.landmark} (reason: UNKNOWN)")
				else:
					rospy.logwarn(f"({NODE_NAME}) unable to apply the landmark {req.landmark} (reason: landmark not applicable)")
				res.success_load_problem = False
				res.success = False
				return res
			else:
				rospy.loginfo(f"({NODE_NAME}) problem interface -- applied landmark {req.landmark}")
			
			problem_instance_text = ""
			problem_instance_received = False
			
			# trigger the problem generation and wait
			cl_problem( )
			rospy.sleep(rospy.Duration(1))
			
			if not problem_instance_received:
				rospy.logwarn(f"({NODE_NAME}) something went wrong while generating the problem instance")
				
				res.success_load_problem = False
				res.success = False
				return res
			else:
				rospy.loginfo(f"({NODE_NAME}) problem interface -- problem loading SUCCEEDED with landmark {req.landmark}")
	else:
		rospy.loginfo(f"({NODE_NAME}) skipping problem generation")
	
	return res





def shut_msg( ):
	''' called at the shutdown of the node, just a message
	'''
	rospy.loginfo( f"({NODE_NAME}) stopping ... " )


def open_cl( cl_name, cl_type ):
	'''open a client
	'''
	
	global SRV_TIMEOUT
	global NODE_NAME
	
	rospy.loginfo(f"({NODE_NAME}) client: {cl_name} ... ")
	try:
		rospy.wait_for_service( cl_name, timeout=SRV_TIMEOUT )
		
	except rospy.ROSException as e:
		rospy.logwarn(f"({NODE_NAME}) client: {cl_name} UNABLE TO CONTACT within the timeout ({SRV_TIMEOUT}s) cause: {e}")
		raise e
		
	cl_this = rospy.ServiceProxy( cl_name, cl_type )
	rospy.loginfo(f"({NODE_NAME}) OK")
	
	rospy.sleep(rospy.Duration(0.75))
	
	return cl_this


if __name__ == "__main__":
	try:
		rospy.init_node( NODE_NAME )
		rospy.on_shutdown( shut_msg )
		
		rospy.loginfo(f"{NODE_NAME} starting... ")
		
		cl_problem = open_cl( service_problem, Empty )
		# cl_plan = open_cl( service_plan, Empty )
		# cl_parse = open_cl( service_parse, Empty )
		# cl_dispatch = open_cl( service_dispatch, DispatchService )
		
		cl_update_goal = open_cl( service_update_goal, UpdateGoal )
		
		rospy.loginfo(f"({NODE_NAME}) service: {service_pipeline} ... ")
		srv_pipeline = rospy.Service( service_pipeline, RosplanPipelineManagerService, cbk_pipeline )
		rospy.loginfo(f"({NODE_NAME}) OK")
		
		rospy.loginfo(f"({NODE_NAME}) subscription: {topic_action_feedback} ... ")
		sub_action_feedback = rospy.Subscriber( topic_action_feedback, ActionFeedback, cbk_action_feedback )
		rospy.loginfo(f"({NODE_NAME}) OK")
		
		rospy.loginfo(f"({NODE_NAME}) subscription: {topic_problem_instance} ... ")
		sub_problem_instance = rospy.Subscriber( topic_problem_instance, String, cbk_problem_instance )
		rospy.loginfo(f"({NODE_NAME}) OK")
		
		rospy.loginfo(f"{NODE_NAME} ready")
		rospy.spin()
		
	except rospy.ROSException as e:
		rospy.logwarn("ROS Exception raised!")
		rospy.logwarn(f"exception text: {e}")
