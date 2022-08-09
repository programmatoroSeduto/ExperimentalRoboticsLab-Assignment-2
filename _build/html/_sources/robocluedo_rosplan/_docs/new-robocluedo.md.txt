
# PDDL description -- the new RoboCLuedo

## differences from the previous version

- usage of *landmarks* instead of only one goal
- the PDDL just manages the movements and how the robot works, and not othe rinformations
- clear distinction between the KB os ROSPlan and the aRMOR KB
- *a simpler, concise model*: this increases the flexibility of the model, and in particular it makes quicker any future update of the arch
- much more like a stata machine than the previous version (or also like a flow chart)
- *no usage of fluents*

## list of Landmarks

- replan
	preconditions:
	```lisp
	
	```
	goal:
	```lisp
	
	```
- propose a solution -- solve
	preconditions:
	```lisp
	
	```
	goal:
	```lisp
	
	```
- collect a new hint -- collect
	preconditions:
	```lisp
	
	```
	goal:
	```lisp
	
	```

### list of predicates

environment:

- `(at ?wp - waypoint )`
- `(not-at ?wp - waypoint )`
- `(passage ?wp1 ?wp2 - waypoint )`
- `(is-center ?wp - waypoint )`
- `(not-is-center ?wp - waypoint )`
- `(explored ?wp - waypoint )`
- `(not-explored ?wp - waypoint )`
- `(at-center )`
- `(not-at-center )`
- `( )`

LANDMARK replan:

- `(dirty )`
- `(not-dirty )`
- `( )`

LANDMARK collect:

- `(hint-ready )`
- `(not-hint-ready )`
- `( )`

LANDMARK solve

- `( )`

### list of services provided by the kb interface

- write a predicate into the kb
- read a predicate from kb
- read the goal
- set the goal

## list of actions

### replan

- **(ACTION) replan**
	- c'è un bool `(dirty )` che indica quando il sistema non è inizializzato
	- questa azione serve solo per "resettare" la KB di rosplan
	- per l'aggiornamento, la action usa il nodo `KB interface` che si occuperà di lettura e scrittura nella kb di rosplan

azione PDDL:

```lisp
(:durative-action replan
	:parameters ( )
	:duration (= ?duration 0.99 )
	:condition (and 
		(at start (dirty ))
	)
	:effect (and 
		(at end (not (dirty )))
		(at end (not-dirty ))
	)
)
```

implementazione ROS:

```{uml} 
@startuml

''' DIAGRAM INFOS
title "(ACTION) replan"
skinparam Padding 8
allow_mixing


''' FROM OTHER DIAGRAMS
() "KB interface service" as SRV_IKB


''' COMPONENTS
component "replan" <<PDDL action>> as ACT_REPLAN


''' LINKS
ACT_REPLAN <-up- SRV_IKB
() "action dispatch" as DISPATCH
ACT_REPLAN <-left- DISPATCH
() "action feedback" as DISP_FEEDBACK 
ACT_REPLAN -right-> DISP_FEEDBACK
() "feedback manager" as TOPIC_FEEDBACK_MANAGER
ACT_REPLAN -down-> TOPIC_FEEDBACK_MANAGER

@enduml
```

### collect

- **(ACTION) move-to**
	- semplice azione di movimento da un punto ad un altro
	- il sistema è *molto simile a quello della vecchia versione* se non per qualche predicato di meno
	- le posizioni in cui il robot può andare sono quelle non marcate come `(is-center ?wp)`, ovvero usa il not
	- il robot *non può compiere un'azione di movimento verso lo stesso punto in cui si trova*: insomma, deve muoversi

azione PDDL:

```lisp
(:durative-action move-to
	:parameters ( ?from ?to - waypoint )
	
	:duration (= ?duration 0.99 )
	
	:condition (and
		;;status
		(at start (not-hint-ready ))
		(at start (not-dirty ))
		
		;; localization
		(at start (at ?from ))
		
		;; passage
		(at start (passage ?from ?to ))
		
		;; path tracking
		(at start (not-explored ?to ))
		
		;;center conditions
		(at start (not-is-center ?to ))
	)
	
	:effect (and
		;; localization
		(at start (not (at ?from )))
		(at end (at ?to ))
		
		;; (in any case)
		(at end (not (at-center )))
		(at end (not-at-center ))
		
		;; path tracking
		(at start (not (not-explored ?to)))
		(at start (explored ?to))
	)
)
```

implementazione ROS:

```{uml} 
@startuml

''' DIAGRAM INFOS
title "(ACTION) go_to_point"
skinparam Padding 8
allow_mixing


''' FROM OTHER DIAGRAMS
() "nav service" as SRV_NAV


''' COMPONENTS
component "move-to" <<PDDL action>> as ACT_MOVE_TO


''' LINKS
ACT_MOVE_TO --> SRV_NAV
() "action feedback" as DISP_FEEDBACK 
ACT_MOVE_TO -right-> DISP_FEEDBACK
() "feedback manager" as TOPIC_FEEDBACK_MANAGER
ACT_MOVE_TO -down-> TOPIC_FEEDBACK_MANAGER
() "action dispatch" as DISPATCH
ACT_MOVE_TO <-left- DISPATCH

@enduml
```

--- 

- **(ACTION) signal-collect-hint**
	- il robot si prepara a raccogliere l'hint dal marker
	- **implementato come una simulated action**

azione PDDL:

```lisp
(:durative-action signal-collect-hint 
	:parameters ( ?wp - waypoint )
	
	:duration (= ?duration 0.99 )
	
	:condition (and
		;; status
		(at start (not-hint-ready ))
		(at start (not-dirty ))
		
		;; localization
		(at start (at ?wp ))
		
		;; hint inside
		(at start (not-hint-collected ?wp ))
	)
	
	:effect (and
		(at start (not (not-hint-ready )))
		(at end (hint-ready ))
	)
)
```

--- 

- **(ACTION) collect-hint**
	- l'azione include due step, per semplicità: mossa del manipolatore verso l'hint, aggiornamento della kb, e verifica della presenza di una soluzione
	- *in caso si presenterà il bisogno di aggiornare il sistema di raccolta degli hint* (ad esempio, detection degli hint tramite una videocamera piuttosto che tramite un sistema di hints) *si dovrà cambiare solo questa particolare azione* e stop. 

azione PDDL:

```lisp
(:durative-action collect-hint
	:parameters ( ?wp - waypoint )
	
	:duration (= ?duration 0.99 )
	
	:condition (and
		;; status
		(at start (hint-ready ))
		
		;; localization
		(at start (at ?wp ))
		(at start (not-is-center ?wp ))
	)
	
	:effect (and
		;; status
		(at end (not (hint-ready )))
		(at end (not-hint-ready ))
	)
)
```

implementazione ROS:

```{uml} 
@startuml

''' DIAGRAM INFOS
title "(ACTION) collect-hint"
skinparam Padding 8
allow_mixing


''' FROM OTHER DIAGRAMS
() "manip service" as SRV_MANIP
() "add hint" as SRV_ADD
() "hint from the Oracle" as TOPIC_ORACLE_HINT


''' COMPONENTS
component "collect-hint" <<PDDL action>> as ACT_COLLECT_HINT


''' LINKS
ACT_COLLECT_HINT --> SRV_ADD
ACT_COLLECT_HINT --> SRV_MANIP
() "action feedback" as DISP_FEEDBACK 
ACT_COLLECT_HINT -right-> DISP_FEEDBACK
() "feedback manager" as TOPIC_FEEDBACK_MANAGER
ACT_COLLECT_HINT -down-> TOPIC_FEEDBACK_MANAGER
() "action dispatch" as DISPATCH
ACT_COLLECT_HINT <-left- DISPATCH
ACT_COLLECT_HINT <-up- TOPIC_ORACLE_HINT

@enduml
```

### solve

- **(ACTION) move-to-center** 
	- come precondizione il robot non si trova al centro, quindi la posizione attuale è `(not-is-center ?wp)`
	- al termine dell'azione, il robot è in stato `(ready-to-solve )`

azione PDDL:

```lisp
(:durative-action move-to-center
	:parameters ( ?from ?to - waypoint )
	
	:duration (= ?duration 0.99 )
	
	:condition (and
		;;status
		(at start (not-hint-ready ))
		(at start (not-dirty ))
		
		;; localization
		(at start (at ?from ))
		(at start (is-center ?to ))
		
		;; center conditions
		(at start (not-at-center ))
		
		;; passage
		(at start (passage ?from ?to ))
	)
	
	:effect (and
		;; localization
		(at start (not (at ?from )))
		(at end (at ?to ))
		
		;; in center
		(at start (not (not-at-center )))
		(at end (at-center ))
	)
)
```

implementazione ROS:

```{uml} 
@startuml

''' DIAGRAM INFOS
title "(ACTION) move-to-center"
skinparam Padding 8
allow_mixing


''' FROM OTHER DIAGRAMS
' ...


''' COMPONENTS
component "move-to-center" <<PDDL action>> as ACT_MOVE_TO_CENTER


''' LINKS
' ...
```

--- 

- **(ACTION) solve** 
	- l'azione PDDL non fa che settare tutto per fare il replan
	- a differenza della precedente versione, il PDDL non tiene traccia delle ipotesi (per questo c'è aRMOR)
	- l'implementazione ROS cerca possibili soluzioni dalla KB aRMOR e propone la soluzione

azione PDDL:

```lisp
(:durative-action solve
	:parameters ( )
	
	:duration (= ?duration 0.99 )
	
	:condition (and
		;;status
		(at start (not-hint-ready ))
		(at start (at-center ))
		;; (at start (not-dirty ))
		
	)
	
	:effect (and
		(at end (dirty ))
	)
)
```

implementazione ROS:

```{uml} 
@startuml

''' DIAGRAM INFOS
title "(ACTION) solve"
skinparam Padding 8
allow_mixing


''' FROM OTHER DIAGRAMS
' ...


''' COMPONENTS
component "solve" <<PDDL action>> as ACT_SOLVE


''' LINKS
' ...
```
