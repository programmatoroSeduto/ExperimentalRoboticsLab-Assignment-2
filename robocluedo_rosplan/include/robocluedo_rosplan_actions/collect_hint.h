
/********************************************//**
*  
* @file collect_hint.h
* @brief ROSPlan action implementation
* 
* This node implements the action (collect ?wp ) used for taking a clue
* from a marker. It performs a simple behaviour: move down the arm to touch 
* the marker, wait, and finally move the arm bask to the home position. 
* 
* @authors Francesco Ganci
* @version v1.0
* 
***********************************************/

#ifndef __H_COLLECT_HINT_H__
#define __H_COLLECT_HINT_H__

#define ROSPLAN_ACTION_NAME "collect_hint"

#include "ros/ros.h"
#include "rosplan_action_interface/RPActionInterface.h"
#include "knowledge_base_tools/feedback_manager.h"
#include "rosplan_dispatch_msgs/ActionDispatch.h"
#include "robocluedo_rosplan_msgs/ManipulationCommand.h"

#define SRV_MANIPULATION "/robocluedo/manipulator_command"

namespace KCL_rosplan 
{

class RP_rcl_collect_hint : public RPActionInterface
{
public:
	
	/** class constructor */
	RP_rcl_collect_hint( ros::NodeHandle& nh_ );
	
	/** class destructor */
	~RP_rcl_collect_hint( );
	
	/** ROSPlan concrete callback */
	bool concreteCallback( const rosplan_dispatch_msgs::ActionDispatch::ConstPtr& msg );

private:
	
	/// node handle
	ros::NodeHandle& nh;
	
	/// action feedback manager (as object instance)
	action_feedback_manager fb;
	
	/// name of the implemented action
	std::string action_name = ROSPLAN_ACTION_NAME;
	
	/// manipulation client
	ros::ServiceClient cl_manip;
};

}

#endif
