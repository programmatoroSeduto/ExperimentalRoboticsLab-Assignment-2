
# aRMOR UML CLASS -- old schematics

```{toctree}
./armor-old-schematics.md
```

see the first part of the project.

## comonent schema

![UML aRMOR node](UML_components_cluedo_armor_interface.png)

here's the updated UML of the component:

```{uml} 
@startuml

''' INFOS
title package robocluedo_armor
skinparam Padding 8
allow_mixing

''' COMPONENTS
component "aRMOR interface" <<RCL>> as NODE_RCL_ARMOR
component "aRMOR service" <<aRMOR>> as NODE_ARMOR

''' INTERFACES
() "/cluedo_armor/add_hint" as SRV_ARMOR_ADD
SRV_ARMOR_ADD <-- "srv" NODE_RCL_ARMOR
note on link: robocluedo_armor_msgs/AddHint.srv

() "/cluedo_armor/find_consistent_h" as SRV_ARMOR_FIND
SRV_ARMOR_FIND <-- "srv" NODE_RCL_ARMOR
note on link: robocluedo_armor_msgs/FindConsistentHypotheses.srv

() "/cluedo_armor/wrong_hypothesis" as SRV_ARMOR_DEL
SRV_ARMOR_DEL <-- "srv" NODE_RCL_ARMOR
note on link: robocluedo_armor_msgs/DiscardHypothesis.srv

() "/cluedo_armor/backup" as SRV_ARMOR_BACKUP
SRV_ARMOR_BACKUP <-- "srv" NODE_RCL_ARMOR
note on link: std_srvs/Trigger.srv

() "/armor_interface_srv" as SRV_ARMOR
SRV_ARMOR --> "cl" NODE_RCL_ARMOR
SRV_ARMOR <-- "srv" NODE_ARMOR


@enduml
```

## the old arch schema

here's how the component were linked to the previous version of the architecture. 

![UML old arch](UML_components_arch_sketch.png)
