
# UML CLASS -- robocluedo armor

```{uml} 
@startuml

title robocluedo_armor project structure
skinparam Padding 8

class "armor_tools" as ARMOR1 {

}

class "armor_cluedo" as ARMOR2 {

}

component "armor_interface" as NODE {

}

database "armor" as ARMOR {

}

ARMOR1 -up-|> ARMOR2 : "   "
ARMOR2 --> ARMOR1 : "   "
ARMOR2 -up-|> NODE : "inherits..."
ARMOR1 "cl" -right-> "srv" ARMOR : "as client"

@enduml
```
