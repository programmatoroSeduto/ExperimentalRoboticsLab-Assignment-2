
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
#include "std_srvs/Trigger.h"
#include "erl2/Oracle.h"
#include "erl2/ErlOracle.h"

#include <map>

#define SERVICE_ROSPLAN_PIPELINE "/robocluedo/pipeline_manager"
/*
# service 'RosplanPipelineManagerService'

## Request

# landmarks
int32 LANDMARK_NONE = -1
int32 LANDMARK_REPLAN = 0
int32 LANDMARK_COLLECT = 1
int32 LANDMARK_SOLVE = 2

# load the problem
bool load_problem

# try to solve the problem with the planner
bool solve_problem

# try to parse the plan
bool parse_plan

# execute the plan
bool execute_plan

# landmark to execute
int32 landmark

---

## Response

# success or failure?
bool success

# loading phase success
bool success_load_problem

# planner execution success
bool success_solve_problem

# planner not able to solve the problem
bool problem_not_solvable

# parsing execution success
bool success_parse_plan

# execution phase success
bool success_execute_plan

# received a feedback (not received a feedback indicates a anomalous situation)
bool feedback_received

# the last feedback received from the system
ActionFeedback feedback
*/


#define SERVICE_ARMOR_ADD "/cluedo_armor/add_hint"
/*
## file 'AddHint.srv'
# it represents a hint of the type -> (Aelem, Belem):property

# the numeric ID of the hint
int32 hypID

# fields of the property
string property
string Aelem
string Belem

---

bool success
*/

#define SERVICE_ARMOR_FIND "/cluedo_armor/find_consistent_h"
/*
## file 'FindConsistentHypotheses.srv'

# empty request

---

# list of consistent hypotheses from the ontology
Hypothesis[] hyp
*/

#define SERVICE_ARMOR_DEL "/cluedo_armor/wrong_hypothesis"

/*
## file 'DiscardHypothesis.srv'
string hypothesisTag

---

bool success
*/

#define SERVICE_ARMOR_BACKUP "/cluedo_armor/backup"
/*
---
bool success   # indicate successful run of triggered service
string message # informational, e.g. for error messages
*/

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

//define LANDMARK_NONE -1
#define LANDMARK_REPLAN 0
#define LANDMARK_COLLECT 1
#define LANDMARK_SOLVE 2

#define MISSION_STATUS_REPLAN 0
#define MISSION_STATUS_COLLECT 1
#define MISSION_STATUS_SOLVE 2
#define MISSION_STATUS_ASK_ORACLE 3
#define MISSION_STATUS_ASK_ONTOLOGY 4
#define MISSION_STATUS_GET_HINT 5

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
		// ros::waitforShutdown( );
		
		switch( this->mission_status )
		{
		case MISSION_STATUS_REPLAN:
		{
			
		}
		break;
		case MISSION_STATUS_COLLECT:
		{
			
		}
		break;
		case MISSION_STATUS_ASK_ONTOLOGY:
		{
			
		}
		break;
		case MISSION_STATUS_SOLVE:
		{
			
		}
		break;
		case MISSION_STATUS_ASK_ORACLE:
		{
			
		}
		break;
		default:
			TWARN( "unknown mission status (obtained " << mission_status << ") RESTARTING" );
			mission_status = MISSION_STATUS_REPLAN;
		}
	}
	
	/** change progress state of the mission */
	void change_mission_status( int status )
	{
		this->mission_status = status;
	}
	
	/** make a plan ready to be executed */
	bool make_plan( int landmark )
	{
		return true;
	}
	
	/** execute the plan */
	void execute_plan( )
	{
		// send exec request and wait
		
		
		// store the message in a field of the class
		
	}
	
	/** receive a hint from the Oracle */
	void cbk_oracle_hint( const erl2::ErlOracle::ConstPtr& hint )
	{
		// store it into the ontology
		
		
	}
	
	/** check wether the hint is valid or not */
	bool is_valid_hint( const erl2::ErlOracle::ConstPtr& hint )
	{
		// ... perform all the checkings ...
		
		return true;
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
