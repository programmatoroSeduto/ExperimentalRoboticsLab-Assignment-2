
# UML -- components schema of the project

## pkg -- robocluedo_rosplan

```{uml} 
@startuml

''' DIAGRAM INFOS
title "robocluedo rosplan"
skinparam Padding 8


''' DIAGRAM ELEMENTS
package "robocluedo rosplan" {

file "pddl\ndomain" as PDDL_DOM
file "pddl\nproblem" as PDDL_PROB

package "ROSplan" {
	database "knowledge base" <<ROSPlan>> as KB
	[problem interface] <<ROSPlan>> as PROBLEM
	[planning interface] <<ROSPlan>> as PLAN
	[parsing interface] <<ROSPlan>> as PARSE
	[plan dispatcher] <<ROSPlan>> as DISPATCH

	KB -right-> PROBLEM
	PROBLEM -down-> PLAN
	PLAN -down-> PARSE
	KB -right- PARSE
	PARSE -down-> DISPATCH
	KB -left- DISPATCH
}

package "actions dispatch" {
	[action] as ACTION
}

[feedback\nmanager] as FEEDBACK
interface "feedback topic" as TOPIC_FEEDBACK
[ROSPlan\npipeline manager] <<node>> as MANAGER
interface "service pipeline manager" as SRV_MANAGER

DISPATCH -right-> ACTION
PDDL_DOM --> KB
PDDL_PROB --> KB

ACTION --> FEEDBACK
FEEDBACK --> TOPIC_FEEDBACK

PROBLEM --> MANAGER
PLAN --> MANAGER
PARSE --> MANAGER
DISPATCH --> MANAGER

MANAGER --> SRV_MANAGER
}
@enduml
```



## pkg -- robocluedo_movement_controller

```{uml} 
@startuml

''' INFOS
title robocluedo movement controller
skinparam Padding 8
allow_mixing


''' COMPONENTS
package "pkg movement controller" {
component "navigation" as NAV
component "manipulation" as MANIP
}

node "MoveIt!" as MOVEIT


''' CONNECTIONS
() "nav service" as SRV_NAV
NAV --> SRV_NAV
() "cmd vel" as TOPIC_VEL
NAV -up-> TOPIC_VEL
() "odometry" as TOPIC_ODOM
NAV <-up- TOPIC_ODOM

() "manip service" as SRV_MANIP
MANIP --> SRV_MANIP

MOVEIT --> MANIP

@enduml
```


## pkg -- robocluedo_armor

```{uml} 
@startuml

''' INFOS
title package robocluedo_armor
skinparam Padding 8
allow_mixing

''' COMPONENTS
package "aRMOR" {
	database "knowledge base" <<aRMOR>> as A_KB
	component "services" <<aRMOR>> as A_SRV
	A_KB -right- A_SRV
}

package "aRMOR tools" {
	class "aRMOR tools" as A_TOOLS 
	class "aRMOR cluedo" as A_CLUEDO 
	A_TOOLS -up-|> A_CLUEDO
	component "aRMOR interface" <<ROS node>> as A_NODE
	A_CLUEDO -up-|> A_NODE
}

() "add hint" as SRV_ADD
() "mark wrong hint" as SRV_DELETE
() "check hints" as SRV_GET

''' CONNECTIONS
A_SRV -right-> A_TOOLS
A_NODE --> SRV_ADD
A_NODE --> SRV_DELETE
A_NODE --> SRV_GET

@enduml
```



## pkg -- robocluedo_mission_manager

```{uml} 

```
