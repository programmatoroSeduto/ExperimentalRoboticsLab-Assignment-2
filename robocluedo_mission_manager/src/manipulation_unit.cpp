
/********************************************//**
*  
* @file manipulation_unit.cpp
* 
* @brief mission manager bridge between the robocluedo ROSPlan framework 
* 	and the navigation controller
* 
* This node is a bridge between the ROSPlan framework, which assumes 
* some other node implemented the service for the navigation from rosplan, 
* and the navigation system: a service call into a service call. 
* 
* The task of this node is quite simple: when received a request from the
* ROSPlan module, the node associates the cartesian coordinates of the
* pointed waypoint, then calls the navigation service; when the navigation
* has finished, the node adapt and returns the response to ROSPlan.
* 
* There are also other sub-tasks performed by this node: listening for the
* markers in order to find the coordinates of the waypoints, for instance,
* using a one-shot subscriber. Or also to initialise the navigation system. 
* 
* @attention This implementation doesn't supports the develop mode. 
* 
* @authors Francesco Ganci
* @version v1.1
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
#include "robocluedo_movement_controller_msgs/Pose2D.h"
#include "visualization_msgs/MarkerArray.h"
#include "visualization_msgs/Marker.h"

#include <map>
#include <signal.h>
#include <math.h>

#include <string>
#define SS(chr) std::string( chr )
#define SSS(el) std::to_string( el )

// rosplan manipulation command
#define SERVICE_ROBOPLAN_MANIP "/robocluedo/manipulator_command"

// manipulation controller service
#define SERVICE_MANIP "/tip_pos"

#define TOPIC_MARMERS "/visualization_marker"

#define POSE_HOME 0
#define POSE_EXTENDED 1
#define POSE_LOW 2
#define POSE_FRONT_LOW 3
#define POSE_FRONT_HIGH 4

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
		
		// topic markers
		TLOG( "sub topic " << TOPIC_MARMERS << " ... " );
		sub_markers = nh.subscribe( TOPIC_MARMERS, 1, &node_manipulation_unit::cbk_markers, this );
		TLOG( "sub topic " << TOPIC_MARMERS << " ...  OK" );
	}
	
	/// spin function: just wait for shutdown
	void spin( )
	{
		// simple spin
		ros::waitForShutdown( );
	}
	
	/********************************************//**
	 *  
	 * \brief service implementation for the rosplan interface
	 * 
	 * this service behaves as a simple delegate, which takes the message 
	 * from the ROSPlan side, associates a posture of the command which 
	 * also depends on the quote of the selected marker, and then calls
	 * the real manipulation controller to perform the movement of the
	 * arm. 
	 * 
	 * @note the ROSplan action calls this function, in RCL#2, when the
	 * 	robot stands in front of a marker. 
	 * 
	 * @param req the command, ROSPlan side
	 * @param res the response to ROSPlan
	 * 
	 * @returns always true
	 * 
	 ***********************************************/
	bool cbk_roboplan_manip( 
		robocluedo_rosplan_msgs::ManipulationCommand::Request& req, 
		robocluedo_rosplan_msgs::ManipulationCommand::Response& res )
	{
		TLOG( "received a request" << (req.home_position? " -- home position" : "") );
		
		if( !found_markers )
		{
			TLOG( "waiting for markers... " );
			ros::Rate r(5);
			do
				r.sleep();
			while( !found_markers );
			TLOG( "found markers! " );
		}
		
		// pass the request to the manipulation controller
		robocluedo_movement_controller_msgs::ManipulatorPosition cmd;
		cmd.request.command = ( req.home_position? POSE_HOME : ( waypoint_heigth[req.waypoint] ? POSE_FRONT_HIGH : POSE_FRONT_LOW ) );
		
		if( !this->cl_manip.call( cmd ) )
		{
			TWARN( "unable to make a service request to SRV "<< SERVICE_MANIP << " -- failed calling service " 
				<< LOGSQUARE( SERVICE_MANIP ) 
				<< (!cl_manip.exists( ) ? " -- it seems not opened" : "") );
			
			res.success = false;
			return true;
		}
		
		res.success = cmd.response.success;
		if( !cmd.response.success )
			TWARN( "manipulator command FAILED -- explaination: " << cmd.response.details );
		
		return true;
	}
	
	/********************************************//**
	 *  
	 * \brief "one-shot" listener for the markers from the Oracle
	 * 
	 * The callback is called once: after having read the markers from the 
	 * topic, it shuts down the subscriber, in order to save a bit of
	 * performance. 
	 * 
	 * @param data the array of markers
	 * 
	 * @todo currently the algorithm just distinguishes between "high"
	 * 	markers and low markers; this approach could be improved in order
	 * 	to perform a better manipulation. 
	 * 
	 ***********************************************/
	void cbk_markers( const visualization_msgs::MarkerArray::ConstPtr& data )
	{
		TLOG( "reading markers ... " );
		
		// read the markers
		int i = 0;
		float max_h = 0.f;
		for( const visualization_msgs::Marker& mrk : data->markers )
		{
			if( mrk.pose.position.z > max_h )
				max_h = mrk.pose.position.z;
		}
		
		i = 0;
		for( const visualization_msgs::Marker& mrk : data->markers )
		{
			std::string marker_name = SS("wp") + SSS(i+1);
			waypoint_heigth[marker_name] = ( mrk.pose.position.z >= max_h );
		}
			
		
		// even add the center
		{
			robocluedo_movement_controller_msgs::Pose2D markerpose;
			markerpose.x = 0.0;
			markerpose.y = 0.0;
			markerpose.yaw = 0.0;
			waypoints["center"] = markerpose;
		}
		
		// unsubscribe
		sub_markers.shutdown( );
		
		found_markers = true;
		TLOG( "reading markers done -- found " << i << " markers" );
	}
	
private:
	
	/// ROS node handle
    ros::NodeHandle nh;
    
    /// ROSPlan manipulation controller service
    ros::ServiceServer srv_roboplan_manip;
    
    /// manipulation controller client
    ros::ServiceClient cl_manip;
    
    /// subscription handle to markers from the Oracle
	ros::Subscriber sub_markers;
	
	/// signal fro the markers listener
	bool found_markers = false;
	
	/// positions of the waypoints
	std::map<std::string, robocluedo_movement_controller_msgs::Pose2D> waypoints;
	
	/// heigth of the markers
	std::map<std::string, bool> waypoint_heigth;
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
	node_manipulation_unit node;
	
	TLOG( "ready" );
	node.spin( );
	
	return 0;
}
