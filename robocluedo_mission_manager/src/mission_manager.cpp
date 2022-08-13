
/********************************************//**
*  
* @file mission_manager.cpp
* 
* @brief the main node of the architecture, launched at last, and combining
* 	the ROSPlan flow with the aRMOR ontology. 
* 
* @authors Francesco Ganci
* @version v1.0 
* 
***********************************************/

#define NODE_NAME "robocluedo_mission_manager"

#define LOGSQUARE( str )  "[" << str << "] "
#define OUTLABEL          LOGSQUARE( NODE_NAME )
#define TLOG( msg )       ROS_INFO_STREAM( OUTLABEL << msg )
#define TWARN( msg )      ROS_WARN_STREAM( OUTLABEL << "WARNING: " << msg )
#define TERR( msg )       ROS_WARN_STREAM( OUTLABEL << "ERROR: " << msg )

#include "ros/ros.h"
#include "signal.h"
#include "robocluedo_rosplan_msgs/RosplanPipelineManagerService.h"
#include "robocluedo_rosplan_msgs/ActionFeedback.h"
#include "robocluedo_armor_msgs/AddHint.h"
#include "robocluedo_armor_msgs/FindConsistentHypotheses.h"
#include "robocluedo_armor_msgs/DiscardHypothesis.h"
#include "robocluedo_armor_msgs/Hypothesis.h"
#include "std_srvs/Trigger.h"
#include "erl2/Oracle.h"
#include "erl2/ErlOracle.h"

#include <map>

#define SERVICE_ROSPLAN_PIPELINE "/robocluedo/pipeline_manager"
#define SERVICE_ARMOR_ADD "/cluedo_armor/add_hint"
#define SERVICE_ARMOR_FIND "/cluedo_armor/find_consistent_h"
#define SERVICE_ARMOR_DEL "/cluedo_armor/wrong_hypothesis"
#define SERVICE_ARMOR_BACKUP "/cluedo_armor/backup"

#define SERVICE_ORACLE_SOLUTION "/oracle_solution"
/*
---
int32 ID
*/

#define TOPIC_ORACLE_HINT "/oracle_hint"
/*
int32 ID
string key
string value
*/

/// how many times the system can fail something
#define MAX_FAULT_COUNT 10

//define LANDMARK_NONE -1

/// landmar REPLAN identifier
#define LANDMARK_REPLAN 0

/// landmark COLLECT identifier
#define LANDMARK_COLLECT 1

/// landmark SOLVE identifier
#define LANDMARK_SOLVE 2

/// landmark REPLAN -- the system is restarting the exploration
#define MISSION_STATUS_REPLAN 0

/// landmark COLLECT -- the system is sill looking for hypotheses
#define MISSION_STATUS_COLLECT 1

/// landmark SOLVE -- the system has a solution to prsent
#define MISSION_STATUS_SOLVE 2

/// propose the solution to the Oracle ans check if the mystery has been solved
#define MISSION_STATUS_ASK_ORACLE 3

/// look for a complete hypothesis inside the ontology
#define MISSION_STATUS_ASK_ONTOLOGY 4

/// receive hints from the Oracle (simple wait)
#define MISSION_STATUS_GET_HINT 5

/// simple wait: the system could fail at most MAX_FAULT_COUNT times
#define MISSION_STATUS_COUNT_FAULT 6

/// the mission has failed
#define MISSION_STATUS_FAULT 7

class node_mission_manager
{
public:
	
	/** node class constructor */
	node_mission_manager( )
	{
		// rosplan service
		if( !open_client<robocluedo_rosplan_msgs::RosplanPipelineManagerService>( SERVICE_ROSPLAN_PIPELINE, cl_rosplan_pipeline ) )
			return;
		
		// aRMOR clients
		if( !open_client<robocluedo_armor_msgs::AddHint>( SERVICE_ARMOR_ADD, cl_armor_add ) )
			return;
		if( !open_client<robocluedo_armor_msgs::FindConsistentHypotheses>( SERVICE_ARMOR_FIND, cl_armor_find ) )
			return;
		if( !open_client<robocluedo_armor_msgs::DiscardHypothesis>( SERVICE_ARMOR_DEL, cl_armor_del ) )
			return;
		if( !open_client<std_srvs::Trigger>( SERVICE_ARMOR_BACKUP, cl_armor_backup ) )
			return;
		
		// oracle elements
		if( !open_client<erl2::Oracle>( SERVICE_ORACLE_SOLUTION, cl_oracle_solution ) )
			return;
		TLOG( "subscribing to " << TOPIC_ORACLE_HINT << " ... " );
		sub_oracle_hint = nh.subscribe( TOPIC_ORACLE_HINT, 10, &node_mission_manager::cbk_oracle_hint, this );
		TLOG( "subscribing to " << TOPIC_ORACLE_HINT << " ... OK" );
	}
	
	/** node working cycle */
	void spin( )
	{
		// counter of faults
		int fault_count = MAX_FAULT_COUNT;
		
		// next state after fault (-1 if not used)
		int state_after_fault = -1;
		
		// the last complete hypothesis from the Ontology
		robocluedo_armor_msgs::Hypothesis last_hyp;
		
		// status ready to solve
		bool ready_to_solve = false;
		
		TLOG( "mission manager STARTING WORKING CYCLE" );
		
		while( ros::ok() )
		{
			switch( this->mission_status )
			{
			case MISSION_STATUS_REPLAN: // landmark
			{
				TLOG( "status: " << "MISSION_STATUS_REPLAN" );
				
				// make the plan with LANDMARK_REPLAN
				robocluedo_rosplan_msgs::RosplanPipelineManagerService::Response res = make_plan( LANDMARK_REPLAN );
				
				// check the outcome of the operation
				if( !res.success )
				{
					// inspect the type of failure
					if( !res.success_load_problem )
						TWARN( "unable to load the problem!" );
					else if( !res.success_solve_problem )
					{
						if( res.problem_not_solvable )
							TWARN( "VERY UNEXPECTED! REPLAN PROBLEM NOT SOLVABLE!" ); // IT COULD NOT OCCUR! but eventually ...
						else
							TWARN( "unable to solve the plan (unexplained error, maybe from the planner popf?)" );
					}
					else
						TLOG( "error in calling the rosplan pipeline manager" );
					
					state_after_fault = MISSION_STATUS_REPLAN;
					this->mission_status = MISSION_STATUS_COUNT_FAULT;
					
					continue;
				}
				
				// execute the plan
				robocluedo_rosplan_msgs::RosplanPipelineManagerService::Response res = execute_plan( );
				
				// check the outcome of the operation
				if( !res.success )
				{
					// inspect the type of failure
					if( !res.success_parse_plan )
						TWARN( "unable to parse the plan!" );
					else if( !res.success_execute_plan )
						TWARN( "unable to dispatch the problem!" );
					else
						TWARN( "unable to execute (due to unexplained error, maybe the service call?)" );
					
					state_after_fault = MISSION_STATUS_REPLAN;
					this->mission_status = MISSION_STATUS_COUNT_FAULT;
					
					continue;
				}
				
				// check also the goal after the dispatch
				if( res.feedback_received && !res.feedback.goal_achieved )
				{
					TWARN( "UNEXPECTED: not reached the goal at the end of the execution! REPLANNING" );
					
					state_after_fault = MISSION_STATUS_REPLAN;
					this->mission_status = MISSION_STATUS_COUNT_FAULT;
					
					continue;
				}
				
				// next status (REPLAN success)
				this->mission_status = MISSION_STATUS_COLLECT;
				
				continue;
			}
			break;
			case MISSION_STATUS_COLLECT: // landmark
			{
				TLOG( "status: " << "MISSION_STATUS_COLLECT" );
				
				// make the plan with LANDMARK_COLLECT
				robocluedo_rosplan_msgs::RosplanPipelineManagerService::Response res = make_plan( LANDMARK_COLLECT );
				
				// check the outcome of the operation
				if( !res.success )
				{
					// inspect the type of failure
					if( !res.success_load_problem )
					{
						TWARN( "unable to load the problem! RETRYING" );
						state_after_fault = MISSION_STATUS_COLLECT;
						this->mission_status = MISSION_STATUS_COUNT_FAULT;
					}
					else if( !res.success_solve_problem )
					{
						if( res.problem_not_solvable )
						{
							// each waypoint has been explored (in this case, skip a step)
							TWARN( "Plan not solvable (all the waypoints have been explored) TRYING TO directly SOLVE THE MYSTERY" ); 
							this->mission_status = MISSION_STATUS_ASK_ONTOLOGY;
							
						}
						else
						 {
							TWARN( "unable to solve the plan (unexplained, maybe a syntax error in the PDDL files?) REPLANNING" );
							
							state_after_fault = MISSION_STATUS_REPLAN;
							this->mission_status = MISSION_STATUS_COUNT_FAULT;
						}
					}
					else
						TLOG( "error in calling the rosplan pipeline manager" );
					
					continue;
				}
				
				// execute the plan
				robocluedo_rosplan_msgs::RosplanPipelineManagerService::Response res = execute_plan( );
				
				// check the outcome of the operation
				if( !res.success )
				{
					// inspect the type of failure
					if( !res.success_parse_plan )
						TWARN( "unable to parse the plan!" ); // per non saper ne leggere ne scrivere, ti faccio eseguire di nuovo l'azione
					else if( !res.success_execute_plan )
					{
						if( res.feedback.failure )
						{
							if( res.hw_failure )
							{
								if( res.failure_nav_system )
									TWARN( "unable to perform navigation (hw navigation problem)" );
								else
									TWARN( "unable to perform manipulation (hw manipulation problem)" );
							}
							else
								TWARN( "unable to dispatch the plan! (failure flag=true ... maybe a problem with services inside robocluedo ROSPlan?)" );
						}
						else
							TWARN( "unable to dispatch the plan! (unexplained)" );
					}
					else
						TWARN( "unable to execute (due to unexplained error, maybe the service call?)" );
					
					state_after_fault = MISSION_STATUS_COLLECT;
					this->mission_status = MISSION_STATUS_COUNT_FAULT;
					
					continue;
				}
				
				// check also the goal after the dispatch
				if( res.feedback_received && !res.feedback.goal_achieved )
				{
					TWARN( "UNEXPECTED: not reached the goal at the end of the execution! REPLANNING" );
					
					state_after_fault = MISSION_STATUS_REPLAN;
					this->mission_status = MISSION_STATUS_COUNT_FAULT;
					
					continue;
				}
				
				// next status
				this->mission_status = MISSION_STATUS_ASK_ONTOLOGY;
				
				continue;
			}
			break;
			case MISSION_STATUS_ASK_ONTOLOGY: // armor
			{
				TLOG( "status: " << "MISSION_STATUS_ASK_ONTOLOGY" );
				
				// ask the ontology for any complete hypothesis
				robocluedo_armor_msgs::FindConsistentHypotheses find_req;
				
				if( !cl_armor_find.call( find_req ) )
				{
					TWARN( "unable to call aRMOR for finding hypotheses! REPEATING" );
					
					state_after_fault = MISSION_STATUS_ASK_ONTOLOGY;
					this->mission_status = MISSION_STATUS_COUNT_FAULT;
					
					continue;
				}
				
				// check the number of returned hints from the ontology
				if( find_req.response.hyp.size( ) > 0 )
				{
					TLOG( "(from the ontology) found " << find_req.response.hyp.size( ) << "consistent hypotheses" );
					ready_to_solve = true;
					last_hyp = hyp[0]; // take the first returned
				}
				else
				{
					TLOG( "no brilliant ideas from the RoboCLuedo B.B.B. (Big Brain Bruh) COLLECTING AGAIN" );
					ready_to_solve = false;
					this->mission_status = MISSION_STATUS_COLLECT;
				}
				
				continue;
			}
			break;
			case MISSION_STATUS_SOLVE: // landmark
			{
				TLOG( "status: " << "MISSION_STATUS_SOLVE" );
				
				// check ready to solve (just to be super sure)
				if( !ready_to_solve )
				{
					TLOG( "HEYHEYHEY WHAT ARE YOU DOING HERE? NOT READY TO SOLVE! " );
					
					ready_to_solve = false;
					this->mission_status = MISSION_STATUS_COLLECT;
					
					continue;
				}
				
				// make the plan with LANDMARK_SOLVE
				robocluedo_rosplan_msgs::RosplanPipelineManagerService::Response res = make_plan( LANDMARK_SOLVE );
				
				// inspect the error if any
				if( !res.success )
				{
					// inspect the type of failure
					if( !res.success_load_problem )
					{
						TWARN( "unable to load the problem! RETRYING" );
						state_after_fault = MISSION_STATUS_SOLVE;
						this->mission_status = MISSION_STATUS_COUNT_FAULT;
					}
					else if( !res.success_solve_problem )
					{
						if( res.problem_not_solvable )
							TWARN( "UNEXPECTED! Plan not solvable, can't find a solution (maybe a issue with the PDDL model?)" ); 
						else
							TWARN( "unable to solve the plan (unexplained, maybe a syntax error in the PDDL files?) REPLANNING" );
						
						
						ready_to_solve = false;
						state_after_fault = MISSION_STATUS_REPLAN;
						this->mission_status = MISSION_STATUS_COUNT_FAULT;
					}
					else
						TLOG( "error in calling the rosplan pipeline manager" );
					
					continue;
				}
				
				// execute the plan
				robocluedo_rosplan_msgs::RosplanPipelineManagerService::Response res = execute_plan( );
				
				// inspect the error if any
				if( !res.success )
				{
					// inspect the type of failure
					if( !res.success_parse_plan )
					{
						TWARN( "unable to parse the plan!" ); // per non saper ne leggere ne scrivere, ti faccio eseguire di nuovo l'azione
						
						state_after_fault = MISSION_STATUS_SOLVE;
						this->mission_status = MISSION_STATUS_COUNT_FAULT;
						
						continue;
					}
					else if( !res.success_execute_plan )
					{
						if( res.feedback.failure )
						{
							if( res.hw_failure )
								TWARN( "unable to perform navigation (hw navigation problem" << (res.feedback.failure_manipulation ? ", strange: failure_manipulation=true" : "") << ")" );
							else
								TWARN( "unable to dispatch the plan! (hw_failure=false but failure=true ... maybe a problem with services inside robocluedo ROSPlan?)" );
						}
						else
							TWARN( "unable to dispatch the plan! (unexplained, no hw failure)" );
					}
					else
						TWARN( "unable to execute due to unexplained error (maybe did a service call fail?)" );
					
					ready_to_solve = false;
					state_after_fault = MISSION_STATUS_COLLECT;
					this->mission_status = MISSION_STATUS_COUNT_FAULT;
					
					continue;
				}
				
				// check the goal
				if( res.feedback_received && !res.feedback.goal_achieved )
				{
					TWARN( "UNEXPECTED: not reached the goal at the end of the execution! REPLANNING" );
					
					ready_to_solve = false;
					state_after_fault = MISSION_STATUS_REPLAN;
					this->mission_status = MISSION_STATUS_COUNT_FAULT;
					
					continue;
				}
				
				// next mission status
				this->mission_status = MISSION_STATUS_ASK_ORACLE;
				
				continue;
			}
			break;
			case MISSION_STATUS_ASK_ORACLE: // oracle, armor
			{
				TLOG( "status: " << "MISSION_STATUS_"ASK_ORACLE );
				
				// ask the Oracle for the solution of the mystery
				erl2::Oracle solution;
				if( !cl_oracle_solution.call( solution ) )
				{
					TWARN( "unable to contact the oracle -- RETRYING" );
					
					state_after_fault = MISSION_STATUS_ASK_ORACLE;
					this->mission_status = MISSION_STATUS_COUNT_FAULT;
					
					continue;
				}
				
				// check the solution
				TLOG( "Narrator voice: \n\t(wispering) RoboCLuedo thinks that the solution is ID=" << last_hyp.ID );
				TLOG( "RoboCLuedo: \n\tWELL, >>" << last_hyp.who << "<< DID YOU MURDER Dr. Black ? \n\n\t\tHUH?\n\n" );
				if( solution.response.ID == last_hyp.ID )
				{
					TLOG( "gg ez" );
					TLOG( "==== where[" << last_hyp.where << "] what[" << last_hyp.what << "] who[" << last_hyp.who << "] ====" );
					
					return;
				}
				else
				{
					TLOG( "Oracle: \n\tID=" << solution.response.ID );
					TLOG( "" << last_hyp.who << ": \n\tNO, RoboCLuedo, \n\tI'm not the killer. \n\n\tWho killed Dr. Black is still there, in this room. \n\t\tWE MUST FIND THAT.\n" );
					TLOG( "(just a hint: did you check the waiter?)" );
					
					// mark the hint as wrong
					robocluedo_armor_msgs::DiscardHypothesis discard_note;
					discard_note.request.ID = last_hyp;
					
					if( !cl_armor_del.call( discard_note ) )
					{
						TWARN( "unable to discard the ID -- can't contact the armor service" );
					}
					else
					{
						std_srvs::Trigger backup_cmd;
						if( !cl_armor_backup( backup_cmd ) || !backup_cmd.response.success )
						{
							TWARN( "ontologybackup failed (retrying later ...)" );
						}
					}
					
					ready_to_solve = false;
					continue;
				}
			}
			break;
			case MISSION_STATUS_COUNT_FAULT: 
			{
				--fault_count;
				TWARN( "status: " << "MISSION_STATUS_COUNT_FAULT (remaining " << fault_count << ")" );
				
				if( fault_count <= 0 )
				{
					mission_status = MISSION_STATUS_FAULT;
					continue;
				}
				
				// next status depending on the current state
				mission_status = state_after_fault;
				continue;
			}
			break;
			
			case MISSION_STATUS_FAULT:
			{
				TERR( "status: " << "MISSION_STATUS_FAULT" );
				
				TERR( "mission failed : too much errors, counter was zero" );
				return;
			}
			break;
			default:
			{
				TWARN( "unknown mission status (obtained " << mission_status << ") REPLANNING" );
				mission_status = MISSION_STATUS_COUNT_FAULT;
				state_after_fault = MISSION_STATUS_REPLAN;
				
				ready_to_solve = false;
			}
			}
		}
	}
	
	/** make a plan ready to be executed (and call the service) */
	robocluedo_rosplan_msgs::RosplanPipelineManagerService::Response make_plan( int landmark )
	{
		robocluedo_rosplan_msgs::RosplanPipelineManagerService cmd = create_planning_request( LANDMARK_REPLAN );
		
		// send the request and wait
		if( !cl_rosplan_pipeline.call( cmd ) )
		{
			TWARN( "unable to make a plan! client calling failed" );
			set_standard_response( cmd.response.success, true );
		}
		
		return cmd.response;
	}
	
	/** execute the plan */
	robocluedo_rosplan_msgs::RosplanPipelineManagerService::Response execute_plan( )
	{
		robocluedo_rosplan_msgs::RosplanPipelineManagerService cmd = create_exec_request( );
		
		// send the request and wait
		if( !cl_rosplan_pipeline.call( cmd ) )
		{
			TWARN( "unable to execute the plan! client calling failed" );
			set_standard_response( cmd.response.success, true );
		}
		
		return cmd.response;
	}
	
	
	/** receive a hint from the Oracle */
	void cbk_oracle_hint( const erl2::ErlOracle::ConstPtr& hint )
	{
		TLOG( "(cbk_oracle_hint, add hint) received hint with content (ID=" << hint->ID << ", key=" << hint->key << ", value=" << hint->value << ")" );
		
		// check validity
		if(is_valid_hint( hint ))
		{
			// store it into the ontology
			robocluedo_armor_msgs::AddHint addh;
			addh.request.hypID = hint->ID;
			addh.request.property = hint->key;
			addh.request.Belem = hint->value;
			
			if( !cl_armor_add.call( addh ) )
			{
				TWARN( "(cbk_oracle_hint, add hint) can't reach the service" );
				return;
			}
			
			if( !hint.response.success )
			{
				TWARN( "(cbk_oracle_hint, add hint) aRMOR interface responded with success=false" );
				return;
			}
			
			TLOG( "(cbk_oracle_hint, add hint) valid hint received and registered" );
		}
		else
			TLOG( "(cbk_oracle_hint, add hint) NOT VALID hint, skip" );
	}
	
	/** check wether the hint is valid or not */
	bool is_valid_hint( const erl2::ErlOracle::ConstPtr& hint )
	{
		return (
			(hint->ID >= 0) &&
			(hint->key != "") &&
			(hint->value != "") &&
			(hint->key != "-1") &&
			(hint->value != "-1")
		);
	}
	
	/** make a planning request without parsing and execution */
	robocluedo_rosplan_msgs::RosplanPipelineManagerService create_planning_request( int landmark )
	{
		robocluedo_rosplan_msgs::RosplanPipelineManagerService cmd;
		
		set_request( cmd, true, true, false, false );
		set_standard_response( cmd, false );
		
		return cmd;
	}
	
	/** make a planning request without loading and solving */
	robocluedo_rosplan_msgs::RosplanPipelineManagerService create_exec_request( int landmark )
	{
		robocluedo_rosplan_msgs::RosplanPipelineManagerService cmd;
		
		set_request( cmd, false, false, true, true );
		set_standard_response( cmd, false );
		
		return cmd;
	}
	
	/** set a standard response for the service planning pipeline request */
	void set_standard_response( robocluedo_rosplan_msgs::RosplanPipelineManagerService& cmd, bool failure = false )
	{
		cmd.response.success = !failure;
		
		cmd.response.success_load_problem = true;
		cmd.response.success_solve_problem = true;
		cmd.response.success_parse_plan = true;
		cmd.response.success_execute_plan = true;
		
		cmd.response.problem_not_solvable = false;
		
		cmd.response.feedback_received = !failure;
		
		cmd.response.feedback.action_name = "";
		cmd.response.feedback.goal_achieved = false;
		cmd.response.feedback.failure = failure;
		cmd.response.feedback.hw_failure = false;
		cmd.response.feedback.failure_nav_system = false;
		cmd.response.feedback.failure_manipulation = false;
		cmd.response.feedback.details = "";
	}
	
	/** quick set request */
	void set_request( robocluedo_rosplan_msgs::RosplanPipelineManagerService& cmd, 
		bool load, bool solve, bool parse, bool exec, int landmark )
	{
		cmd.landmark = landmark;
		
		cmd.request.load_problem = load;
		cmd.request.solve_problem = solve;
		cmd.request.parse_plan = parse;
		cmd.request.execute_plan = exec;
	}
	
private:
	
	/// the node handle
	ros::NodeHandle nh;
	
	/// client rosplan pipeline manager
	ros::ServiceClient cl_rosplan_pipeline;
	
	/// client for armor add hint
	ros::ServiceClient cl_armor_add;
	
	/// client for armor find consistent hypotheses
	ros::ServiceClient cl_armor_find;
	
	/// client for armor discard hypotheses
	ros::ServiceClient cl_armor_del;
	
	/// client for armo backup request
	ros::ServiceClient cl_armor_backup;
	
	/// client for requiring the solution to the oracle
	ros::ServiceClient cl_oracle_solution;
	
	/// subscription oracle hints
	ros::Subscriber sub_oracle_hint;
	
	/// state of the mission
	int mission_status = MISSION_STATUS_REPLAN;
	
	/** open a client */
	template<class T>
	bool open_client( std::string cl_name, ros::ServiceClient& cl_handle )
	{
		TLOG( "Opening client " << LOGSQUARE( cl_name ) << "..." );
		cl_handle = nh.serviceClient<T>( cl_name );
		if( !cl_handle.waitForExistence( ros::Duration( 60 ) ) )
		{
			TERR( "unable to contact the server " << cl_name << " - timeout expired (60s) " );
			return false;
		}
		TLOG( "Opening client " << LOGSQUARE( cl_name ) << "... OK" );
		
		return true;
	}
};



void shut_msg( int sig )
{
	TLOG( "stopping... " );
	ros::shutdown( );
}


int main( int argc, char* argv[] )
{
	ros::init( argc, argv, NODE_NAME, ros::init_options::NoSigintHandler );
	signal( SIGINT, shut_msg );
	ros::NodeHandle nh;
	
	ros::AsyncSpinner spinner( 5 );
	spinner.start( );
	
	TLOG( "starting ... " );
	node_mission_manager node;
	
	TLOG( "ready" );
	node.spin( );
	
	return 0;
}
