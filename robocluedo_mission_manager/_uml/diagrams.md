
# UML -- mission manager interfaces

---

```{toctree}
---
caption: Contents
---
./diagrams.md
```

---

This document provides a reference of all the interfaces provided by the `robocluedo_mission_manager` package. The most complex node from this point of view is the `mission_manager.cpp` which is the "head" of the entire architecture. 

## The mission manager

There are many interfaces involving this node. Let's see it under many points of view.

### mission_manager -- rosplan communication

The mission manager continuously have to interact with the node *rosplan_pipeline_manager.py* to load and set each of the three available landmarks. 

```{uml}
@startuml

title Mission Manager -- ROSPlan communication
left to right direction

''' COMPONENTS
component MISSION_MANAGER <<RCL Mission Manager>>
component PIPELINE <<RCL ROSPlan>>

''' INTERFACES
() "/robocluedo/pipeline_manager" as SRV_PIPELINE
PIPELINE "srv" --> SRV_PIPELINE : robocluedo_rosplan_msgs/RosplanPipelineManagerService.srv

''' LINKS
SRV_PIPELINE --> "cl" MISSION_MANAGER

@enduml
```

### mission_manager -- interaction with aRMOR interface

The mission manager uses all the four directives provided by the package aRMOR interface. 

```{uml}
@startuml

title Mission Manager -- interaction with aRMOR interface
left to right direction

''' COMPONENTS
component ARMOR_INTERFACE <<RCL aRMOR>>

() "/cluedo_armor/add_hint" as SRV_ARMOR_ADD
() "/cluedo_armor/find_consistent_h" as SRV_ARMOR_FIND
() "/cluedo_armor/wrong_hypothesis" as SRV_ARMOR_DEL
() "/cluedo_armor/backup" as SRV_ARMOR_BACKUP

ARMOR_INTERFACE "srv" --> SRV_ARMOR_ADD : robocluedo_armor_msgs/AddHint.srv
ARMOR_INTERFACE "srv" --> SRV_ARMOR_FIND : robocluedo_armor_msgs/FindConsistentHypotheses.srv
ARMOR_INTERFACE "srv" --> SRV_ARMOR_DEL : robocluedo_armor_msgs/DiscardHypothesis.srv
ARMOR_INTERFACE "srv" --> SRV_ARMOR_BACKUP : std_srvs/Trigger.srv

component MISSION_MANAGER <<RCL Mission Manager>>

SRV_ARMOR_ADD --> "cl" MISSION_MANAGER
SRV_ARMOR_FIND --> "cl" MISSION_MANAGER
SRV_ARMOR_DEL --> "cl" MISSION_MANAGER
SRV_ARMOR_BACKUP --> "cl" MISSION_MANAGER


@enduml
```

### mission_manager -- communication with the Oracle

```{uml}
@startuml

title Mission Manager -- oracle
left to right direction

''' COMPONENTS
component MISSION_MANAGER <<RCL Mission Manager>>
component ORACLE <<erl2>>

() "/oracle_hint" as TOPIC_ORACLE_HINT
() "/oralce_solution" as SRV_ORACLE_SOLUTION

ORACLE "pub" --> TOPIC_ORACLE_HINT : erl2/ErlOracle.msg
ORACLE "srv" --> SRV_ORACLE_SOLUTION : erl2/Oracle.srv

SRV_ORACLE_SOLUTION --> "cl" MISSION_MANAGER
TOPIC_ORACLE_HINT --> "sub" MISSION_MANAGER

@enduml
```

## navigation_unit

The navigation unit takes the waypoint string from the ROSPlan action, decodes it into a cartesian position, then delegates the command to the navigation manager. 

```{uml}
@startuml

title Navigation Unit
left to right direction

''' COMPONENTS
component NAV_UNIT <<RCL Mission Manager>>
component NAV_MANAGER <<RCL Movement Controller>>
component MOVE_TO <<RCL ROSPlan Action>>
component ORACLE <<erl_assignment3>>

''' INTERFACES
() "/robocluedo/navigation_command" as SRV_ROSPLAN_NAV
() "/navigation_manager/navigation" as SRV_NAV
() "/navigation_manager/set_algorithm" as SRV_ALGO
() "/visualization_marker" as TOPIC_MARKER

NAV_UNIT "srv" --> SRV_ROSPLAN_NAV : robocluedo_rosplan_msgs/NavigationCommand.srv
NAV_MANAGER "srv" --> SRV_NAV : robocluedo_movement_controller_msgs/NavigationService.srv
NAV_MANAGER "srv" --> SRV_ALGO : robocluedo_movement_controller_msgs/Algorithm.srv
ORACLE "pub" --> TOPIC_MARKER : visualization_msgs/MarkerArray.msg

SRV_ROSPLAN_NAV --> "cl" MOVE_TO
SRV_NAV --> "cl" NAV_UNIT
SRV_ALGO --> "cl" NAV_UNIT
TOPIC_MARKER -down-> "sub" NAV_UNIT

@enduml
```

## manipulation_unit

The manipulation unit also subscribes to the topic markers fuor understanding if the marker is located "high" or "low". The pose associated to the command from ROSPlan changes depending on the quote of the marker. 

```{uml}
@startuml

title Manipulation unit - RCL#2 version
left to right direction

component MANIP_ROSPLAN <<RCL ROSPlan Actions>>
component ORACLE <<erl2>>
component MANIP_UNIT <<RCL Mission Manager>>
component MANIP_CONTROLLER <<RCL Movement Controller>>

() "/robocluedo/manipulator_command" as SRV_ROSPLAN_MANIP
() "/tip_pos" as SRV_MANIP
() "/visualization_marker" as TOPIC_MARKERS

MANIP_UNIT "srv" --> SRV_ROSPLAN_MANIP : robocluedo_rosplan_msgs/ManipulationCommand.srv
MANIP_CONTROLLER "srv" --> SRV_MANIP : robocluedo_movement_controller_msgs/ManipulatorPosition.srv
ORACLE "pub" --> TOPIC_MARKERS : visualization_msgs/MarkerArray.msg

SRV_ROSPLAN_MANIP --> "cl" MANIP_ROSPLAN
SRV_MANIP --> "cl" MANIP_UNIT
TOPIC_MARKERS --> "sub" MANIP_UNIT
note on link: one-shot subscription

@enduml
```
