
/********************************************//**
*  
* @file navigation_unit.cpp
* 
* @brief mission manager bridge between the robocluedo ROSPlan framework 
* 	and the navigation controller
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
#include "robocluedo_rosplan_msgs/NavigationCommand.h"
#include "robocluedo_movement_controller_msgs/NavigationService.h"
#include "robocluedo_movement_controller_msgs/Pose2D.h"
#include "robocluedo_movement_controller_msgs/Algorithm.h"
#include "visualization_msgs/MarkerArray.h"
#include "visualization_msgs/Marker.h"

#include <map>
#include <signal.h>
#include <math.h>

#include <string>
#define SS(chr) std::string( chr )
#define SSS(el) std::to_string( el )

#define SERVICE_ROSPLAN_NAVIGATION "/robocluedo/navigation_command"
#define SERVICE_SET_ALGORITHM "/navigation_manager/set_algorithm"
#define SERVICE_NAVIGATION "/navigation_manager/navigation"
#define TOPIC_MARMERS "/visualization_marker"

// i.e. use the bug_m motion planning algorithm
#define NAV_ALGO_BUG_M 0

class node_navigation_unit
{
public:
	
	/** node class constructor */
	node_navigation_unit( )
	{
		// topic markers
		TLOG( "sub topic " << TOPIC_MARMERS << " ... " );
		sub_markers = nh.subscribe( TOPIC_MARMERS, 1, &node_navigation_unit::cbk_markers, this );
		TLOG( "sub topic " << TOPIC_MARMERS << " ...  OK" );
		
		// service rosplan to unit
		TLOG( "advertising service " << SERVICE_ROSPLAN_NAVIGATION << " ... " );
		srv_rosplan_nav = nh.advertiseService( SERVICE_ROSPLAN_NAVIGATION, &node_navigation_unit::cbk_navigation, this );
		TLOG( "advertising service " << SERVICE_ROSPLAN_NAVIGATION << " ... OK" );
		
		// client navigation controller
		TLOG( "opening client " << SERVICE_NAVIGATION << " ... " );
		cl_nav = nh.serviceClient<robocluedo_rosplan_msgs::NavigationCommand>( SERVICE_NAVIGATION );
		if( !cl_nav.waitForExistence( ros::Duration(60) ) )
		{
			TERR( "unable to contact the server " << SERVICE_NAVIGATION << " - timeout expired (60s) " );
			return;
		}
		TLOG( "opening client " << SERVICE_NAVIGATION << " ... OK" );
		
		// client navigation algorithm
		TLOG( "opening client " << SERVICE_SET_ALGORITHM << " ... " );
		cl_nav_algorithm = nh.serviceClient<robocluedo_rosplan_msgs::NavigationCommand>( SERVICE_SET_ALGORITHM );
		if( !cl_nav_algorithm.waitForExistence( ros::Duration(60) ) )
		{
			TERR( "unable to contact the server " << SERVICE_SET_ALGORITHM << " - timeout expired (60s) " );
			return;
		}
		TLOG( "opening client " << SERVICE_SET_ALGORITHM << " ... OK" );
	}
	
	/** spin function: just swait for shutdown */
	void spin( )
	{
		// simple spin
		ros::waitForShutdown( );
	}
	
	/** the service required by the rosplan package */
	bool cbk_navigation( 
		robocluedo_rosplan_msgs::NavigationCommand::Request& req, 
		robocluedo_rosplan_msgs::NavigationCommand::Response& res )
	{
		TLOG( "received a request (wp=)" << req.waypoint );
		
		if( !mpl_enabled )
		{
			TLOG( "enabling motion planning algorithm..." );
			
			robocluedo_movement_controller_msgs::Algorithm algo;
			algo.request.algorithm = NAV_ALGO_BUG_M;
			algo.request.enabled = true;
			
			if( !cl_nav_algorithm.call( algo ) )
			{
				TWARN( "unable to contact the service to enable the navigation algorithm." );
				
				res.success = false;
				return true;
			}
			
			if( !algo.response.success )
			{
				TWARN( "unable to activate the navigation controller! details: " << algo.response.details );
				
				res.success = false;
				return true;
			}
			
			TLOG( "enabling motion planning algorithm... OK" );
		}
		
		if( !found_markers )
		{
			TLOG( "waiting for markers... " );
			ros::Rate r(5);
			do
				r.sleep();
			while( !found_markers );
			TLOG( "found markers! " );
		}
		
		// before starting, check if the marker exists
		if( waypoints.find( req.waypoint ) == waypoints.end() )
		{
			TWARN( "the marker " << req.waypoint << " does not exist." );
			
			res.success = false;
			return true;
		}
		
		// send the command
		TLOG( "sending the navigation command..." );
		
		robocluedo_movement_controller_msgs::NavigationService cmd;
		cmd.request.target = waypoints[req.waypoint];
		TLOG("waypoint with coordinates (" << cmd.request.target.x << ", " << cmd.request.target.y << ", " << cmd.request.target.yaw << ")");
		
		if( !cl_nav_algorithm.call( cmd ) )
		{
			TWARN( "unable to contact the navigation service" );
			
			res.success = false;
			return true;
		}
		
		// check for success
		if( !cmd.response.success )
			TWARN( "navigation controlled FAILED -- explaination: " << cmd.response.details );
		
		TLOG("done!");
		res.success = cmd.response.success;
		return true;
	}
	
	/** "one-shot" listener for the markers from the Oracle */
	void cbk_markers( const visualization_msgs::MarkerArray::ConstPtr& data )
	{
		// read the markers
		int i = 1;
		for( const visualization_msgs::Marker& mrk : data->markers )
		{
			std::string marker_name = SS("wp") + SSS(i);
			
			robocluedo_movement_controller_msgs::Pose2D markerpose;
			markerpose.x = mrk.pose.position.x;
			markerpose.y = mrk.pose.position.y;
			markerpose.yaw = atan2( markerpose.x, markerpose.y );
			
			waypoints[marker_name] = markerpose;
			
			++i;
		}
		
		// unsubscribe
		sub_markers.shutdown( );
		
		found_markers = true;
		TLOG( "reading markers done -- found " << i << " markers" );
	}

private:
	
	/// ROS node handle
	ros::NodeHandle nh;
	
	/// service handle rosplan to unit
	ros::ServiceServer srv_rosplan_nav;
	
	/// client handle unit to navigation
	ros::ServiceClient cl_nav;
	
	/// subscription handle to markers from the Oracle
	ros::Subscriber sub_markers;
	
	/// to change the algorithm (in case...)
	ros::ServiceClient cl_nav_algorithm;
	
	/// positions of the waypoints
	std::map<std::string, robocluedo_movement_controller_msgs::Pose2D> waypoints;
	
	/// signal fro the markers listener
	bool found_markers = false;
	
	/// is the motion planning algorithm active?
	bool mpl_enabled = false;
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
	
	TLOG( "starting ... " );
	node_navigation_unit node;
	
	TLOG( "ready" );
	node.spin( );
	
	return 0;
}


