
/********************************************//**
*  
* @file manipulation_unit.cpp
* 
* @brief mission manager bridge between the robocluedo ROSPlan framework 
* 	and the manipulation controller.
* 
* @authors Francesco Ganci
* @version v1.0 
* 
***********************************************/

#define NODE_NAME "manipualtion_unit"

#define LOGSQUARE( str )  "[" << str << "] "
#define OUTLABEL          LOGSQUARE( NODE_NAME )
#define TLOG( msg )       ROS_INFO_STREAM( OUTLABEL << msg )
#define TWARN( msg )      ROS_WARN_STREAM( OUTLABEL << "WARNING: " << msg )
#define TERR( msg )       ROS_WARN_STREAM( OUTLABEL << "ERROR: " << msg )

#include "ros/ros.h"
#include "robocluedo_rosplan_msgs/ManipulationCommand.h"
#include "robocluedo_movement_controller_msgs/ManipulatorPosition.h"

#include <string>
#include <map>
#include <signal.h>

// rosplan manipulation command
#define SERVICE_ROBOPLAN_MANIP "/robocluedo/navigation_command"

// manipulation controller service
#define SERVICE_MANIP "/tip_pos"

/********************************************//**
 *  
 * \class nove_manipulation_unit
 * 
 * \brief bridge between robocluedo ROSPlan interface and the manipulation
 * 	controller
 * 
 * @note in future, someone maybe wants to add functionalities to ROSPlan
 * robocluedo interface: that's why this bridge is located in its node, and
 * not in only one node. 
 * 
 ***********************************************/
class node_manipulation_unit
{
public:
	
	/// node constructor
	node_manipulation_unit( )
	{
		// rosplan manipulation service
		TLOG( "Advertising service " << LOGSQUARE( SERVICE_ROBOPLAN_MANIP  ) << "..." );
		srv_roboplan_manip = nh.advertiseService( SERVICE_ROBOPLAN_MANIP, &node_manipulation_unit::cbk_roboplan_manip, this );
		TLOG( "Advertising service " << LOGSQUARE( SERVICE_ROBOPLAN_MANIP  ) << "... ok" );
		
		// manipulation controller
		TLOG( "Opening client " << LOGSQUARE( SERVICE_MANIP ) << "..." );
		cl_manip = nh.serviceClient<robocluedo_movement_controller_msgs::ManipulatorPosition>( SERVICE_MANIP );
		if( !cl_manip.waitForExistence( ros::Duration( 60 ) ) )
		{
			TERR( "unable to contact the server " << SERVICE_MANIP << " - timeout expired (60s) " );
			return;
		}
		TLOG( "Opening client " << LOGSQUARE( SERVICE_MANIP ) << "... OK" );
	}
	
	/// spin function: just swait for shutdown
	void spin( )
	{
		// simple spin
		ros::waitForShutdown( );
	}
	
	/********************************************//**
	 *  
	 * \brief service implementation for the rosplan interface
	 * 
	 * @param req
	 * @param res
	 * 
	 * @returns always true
	 * 
	 ***********************************************/
	bool cbk_roboplan_manip( 
		robocluedo_rosplan_interface_msgs::ManipulationCommand::Request& req, 
		robocluedo_rosplan_interface_msgs::ManipulationCommand::Response& res )
	{
		TLOG( "received request" << (req.home_position? " -- home position" : "") );
		
		return true;
	}
	
private:
	
	/// ROS node handle
    ros::NodeHandle nh;
    
    /// ROSPlan manipulation controller service
    ros::ServiceServer srv_roboplan_manip;
    
    /// manipulation controller client
    ros::ServiceClient cl_manip;
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
	ros::AsyncSpinner spinner( 4 );
	spinner.start( );
	ros::NodeHandle nh;
	
	TLOG( "starting ... " );
	
	node_manipulation_unit node;
	
	TLOG( "ready" );
	
	( node ).spin( );
	
	return 0;
}
