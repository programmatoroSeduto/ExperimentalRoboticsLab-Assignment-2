
# UML -- bug_m PY node

```{toctree} 
./bug-m.md
```

bug_m combines the two behaviours `go_to_point.py` ans `wall_follow.py` in order to reach a given position in a indoor environment with walls. 

## SCHEMA

```{uml} 
@startuml

''' INFOS
title NODE bug_m
skinparam Padding 8
allow_mixing


''' COMPONENTS
component "bug_m" <<ROSPy node>> as NODE_BUG_M {
	node "clbk_odom" as NODE_BUG_M.READ_ODOM
	node "clbk_laser" as NODE_BUG_M.LISTENER_LASER
}

() "/odom" as TOPIC_ODOM
NODE_BUG_M.READ_ODOM "sub" <-- TOPIC_ODOM
note on link: nav_msgs/Odometry.msg

() "/scan" as TOPIC_LASER
NODE_BUG_M.LISTENER_LASER "sub" <-- TOPIC_LASER
note on link: sensor_msgs/LaserScan.msg

() "/cmd_vel" as TOPIC_VEL
NODE_BUG_M "pub" --> TOPIC_VEL
note on link: geometry_msgs/Twist.msg

() "/go_to_point_switch" as SRV_GOTO_SWITCH
NODE_BUG_M "cl" <-- SRV_GOTO_SWITCH
note on link: std_srvs/SetBool.srv

() "/wall_follower_switch" as SRV_WALL_SWITCH
NODE_BUG_M "cl" <-- SRV_WALL_SWITCH
note on link: std_srvs/SetBool.srv

@enduml
```

## HOW TO use the node -- basic way

it requires another node listening for the odom channel and checking when to deactivate the service

- set `des_pos_x` and `des_pos_y` into the paramater server
- activate the service with a request to `/go_to_point_switch` of type `std_srvs/SetBool` (flag `SetBool.Request.data=True`)
- ... wait until the robot is near enough to the point
- turn off the node (flag `SetBool.Request.data=False`)
