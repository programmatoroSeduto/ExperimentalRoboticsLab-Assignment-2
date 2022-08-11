
# RoboCLuedo -- Development Log

[plantUML online](http://www.plantuml.com/plantuml/uml/VLNlQkGs4F-kfvWB77NWuetthd-Q3-NI59h0fLt8XdufB8eqNelOaf7acAKK-XfzlJv9PoHxDybo-M2FD7z-C_ERyUxd4AMFGzSAyKvZRIo22t952cXYxCF5Ok7bM6vDR8Q78Q1NpaQqiLIkMrnv6HhKdR5wiMgbZVUtNyvSZpQW6ho9E-bLOoAgE7XSdXcA3OjEXeXUl3DMjOFUfrjSkQvpjkpfV6oyfymBsRPVCLzBhqVfyGsNMnFK6-Oxz4zlfhWpyHcy-AQ4M-btO098-0MViAM-FHWBiK5OUQS75I6Yx4gu8qqZsV4FOigD0QfpM5s1j9eUkBJQEwEXRvp5af5_TWyP-5PQkJt0NYhb1Xl3X7kTOCb9pL1cjSUuUU9xOEtD6hR33iR6GUlS8-dgY3uXXjHs2HonRd07D2ABNBbBTejnTFuHQFWVKf8d8q52RJoEHCRiTcC9a7nBGSm03ok8wBP8DWz_2U9mmxkpsNf4kz4pNGLJ-05EM9oGV4uRt_Uydfo-nc2jZCRPK72dvCo2WwZRzHIVXmgNlA774BJEnc88cowpN13j54HlZdt1DIkcMH3EtzmarMOK7hMfCJn6rnUzef3gnsLPVVT3MPNLEKCSnf-wlPfgQcNF8Pry5RFCQTKiidUUSM5w5apIpihEABXPiL-sGbNn9PrVXicyR4Tnqn9IQQy3yueKMRngAZdVFn1F0qnaBy_Byq_mPNrt642cZ3ZxBM-Jc9XY0ZUZyTZo5BmR8kKPJMqkLo_pCDGKEnL5-rZGG_hVwvfWW2xiTKqYUymhMim7idvRbH-_BvUVgFAFnvYgTOCkqfjiyqg_z1CYVVLdPpz1houWkC5JSkKq7WmJnMnLHhRGeJPI2DghPEua1TR6IfkinjPkRyj3lO0aG57LWLBoPYTpdi45VwIrsGxv0n0YHoMFp9wOIdYkoe8rp9KG6Mj_fwhsv_vm58BUwJRJACHyIkf45-cSYuJslZOjqbfXSGeUhKQFcWg83Sn_3q190rKDxwT3SVgdNJS8roQgfZ6FwF_xvzy0lmv68qIt3nIV2I_zz0hnFm00)

template:

```
## data

- ...fatto...
- ...fatto...
- ...

```

## 08/08/2022 -- setup della documentazione e project refactoring

la versione precedente del progetto ha parecchi difetti:

- veramente poco intuitiva
- mancanza di un module testing serio
- codice poco chiaro ed eccessivo
- usare ROSPlan come KB è una pessima idea: complica il PDDL e potrebbe dare strani errori

link alla vecchia versione del progetto: [ExperimentalRoboticsLab-Assignment-2-OLD](https://github.com/programmatoroSeduto/ExperimentalRoboticsLab-Assignment-2-OLD.git)

---

prima del primo commit,

- creazione di una nuova repository per il progetto, link [qui](https://github.com/programmatoroSeduto/ExperimentalRoboticsLab-Assignment-2.git)
- craeazione di un template di repository che permetta di usare plantUML (è finito il tempo di scrivere gli UML su draw.io) vedi [qui (privata)](https://github.com/programmatoroSeduto/plantuml-sphinx)
- **primo setup dell'area di lavoro** con plantUML e Sphinx funzionante
- struttura readme, solo elenco puntato per il momento (estendere man mano che andiamo avanti)
- **COMMIT** creazione di un branch vuoto per la copia degli altri eventuali pacchetti da scaricare
- **COMMIT** con tutto il materiale iniziale funzionante

---

*ora siamo pronti per iniziare a lavorare con ROS*. Prima di tutto, il package per il module testing:

- **NUOVO PACKAGE** creazione package per il module testing:
	```
	catkin_create_pkg robocluedo_module_testing roscpp rospy std_msgs geometry_msgs nav_msgs std_srvs
	``` 
- documentazione interna al module testing con index e tutto il resto
- creazione template per la pagina index di un package
- qualche check prima del commit:
	- prova a compilare .. .OK
	- make documentazione .. OK
- **COMMIT** : "module testing package"
- nuova todolist (stavolta su carta)

---

lavoro su aRMOR, estrazione del codice dal vecchio assignment, vedi [erl1](https://github.com/programmatoroSeduto/ExperimentalRoboticsLab-Assignment-1.git):

- download codice sul desktop
- **NUOVO PACKAGE** armor msgs
	```
	catkin_create_pkg robocluedo_armor_msgs std_msgs armor_msgs std_srvs
	```
- senza perdere tempo, prima documentazione
- **NUOVO PACKAGE** armor
	```
	catkin_create_pkg robocluedo_armor roscpp rospy armor_msgs robocluedo_armor_msgs srd_msgs std_srvs
	```
- e prima documentazione anche qui senza perdere tempo
- giusto per, proviamo a compilare, non si sa mai
- **COMMIT** : "armor packages setup"
- importazione del codice delle interfacce aRMOR
	- (header) armor tools
	- (header) armor cluedo
	- (codice) armor tools e armor cluedo
	- (codice nodo) cluedo armor interface
- il primo lavoro è quello di creare i messaggi da compilare, guardando che cosa richiede l'interfaccia nodo armor
	- AddHint srv
	- DiscardHypothesis srv 
	- Hypothesis msg
	- FindConsistentHypotheses srv
- compilaziione messaggi con catkin
- ora devo "solo" aggiornare il codice C++ del nodo ROS e scrivere il catkin
- ora mancano cose tipo ... la ontology ... e tutto il resto
- selezione dei vecchi diagrammi UML riferiti ad aRMOR
- prima di fare altro, proviamo a compilare ... OK! perfetto
- e aggiornamento docs
- **COMMIT** : "armor package first re-adaptation"
- template per le pagine di documentazione con UML
- nuovi diagrammi per aRMOR (quelli che ci sono sono sbagliati...)
	- class diagram iniziale
	- vediamo se genera... 
	- ho dovuto risolvere uno *stupido problema di padding*, vedi `skinparam Padding <tot>`
	- per il momento ci accontentiamo di semplici blocchi
- voglio provare un po' la compilazione esterna di una libreria
	- module testing dal vecchio modulo ... in quello del module testing
	- proviamo a compiare ... non va
		```text
		CMake Error at /opt/ros/noetic/share/catkin/cmake/catkinConfig.cmake:83 (find_package):
		  Could not find a package configuration file provided by "armor_tools" with
		  any of the following names:

			armor_toolsConfig.cmake
			armor_tools-config.cmake

		  Add the installation prefix of "armor_tools" to CMAKE_PREFIX_PATH or set
		  "armor_tools_DIR" to a directory containing one of the above files.  If
		  "armor_tools" provides a separate development package or SDK, be sure it
		  has been installed.
		Call Stack (most recent call first):
		  ExperimentalRoboticsLab-Assignment-2/robocluedo_module_testing/CMakeLists.txt:10 (find_package)


		-- Configuring incomplete, errors occurred!
		See also "/root/ros_ws/build/CMakeFiles/CMakeOutput.log".
		See also "/root/ros_ws/build/CMakeFiles/CMakeError.log".
		make: *** [Makefile:17330: cmake_check_build_system] Error 1
		Invoking "make cmake_check_build_system" failed
		```
	- *per usare le librerie di un package A in un package B basta riferirsi al package A nel package B*
	- proviamo un po' questo test, giusto per essere sicuri
		- questo ... OK (dopo qualche fortissima bestemmia ... prossima volta controlla anche i file inclusi)
			```
			roslaunch robocluedo_module_testing test_armor.launch
			```
		- questo ... OK (ci è voluto poco stavolta, per fortuna)
			```
			roslaunch robocluedo_module_testing test_armor_tools.launch
			```
- voglio un attimo documentare i test in UML, per avere sempre presenti come sono fatti
- **COMMIT** : "testing armor and first documentation"

---

## 09/08/2022 -- modello PDDL e ROSPlan package

- serve uno *schema completo* dell'applicazione prima di iniziare
	- components robocluedo rosplan (solo bozza)
	- *meglio lavorare in MD* : nessuna tabulazione fastidiosa, è più facile lavorarci su
	- components armor 
		- (ricorda di usare l'opzione `allow_mixing` all'inizio del codice per poter usare diversi tipi di UML nello stesso schema)
	- component movement controller
	- **il mission manager non lo faccio adesso** perchè è il nodo che va a "legare" tutte le parti dell'architettura. quindi, meglio lasciarlo a quando avrò un'idea chiara
- **COMMIT** : "uml arch first draft"

---

ora, iniziamo a lavorare su rosplan:

- **NUOVO PACKAGE**
	```bash
	catkin_create_pkg robocluedo_rosplan_msgs std_msgs std_srvs diagnostic_msgs geometry_msgs nav_msgs rosplan_dispatch_msgs rosplan_knowledge_msgs
	```
- documentazione del package e prima configurazione (penso userò quello che ho fatto nella precedente versione, inutile reinventare chissà che)
- e trasferimento dei messaggi nel nuovo package (copia incolla facile facile)
	- e compilazione ... OK
- **NUOVO PACKAGE** 
	```bash
	catkin_create_pkg robocluedo_rosplan roscpp rospy std_msgs std_srvs diagnostic_msgs rosplan_dispatch_msgs rosplan_knowledge_msgs rosplan_dependencies robocluedo_rosplan_msgs
	```
- e primo setup del package
- compilazione da vuoto ... OK
- **COMMIT** : "rosplan first setup"

---

è il momento di rifare il PDDL

- iniziamo col prendere l'environment dal precedente progetto
- mi converrà progettare prima di fiondarmi sul codice...
	- nuovo documento sul PDDL
	- una lista delle azioni PDDL e dei landmarks
	- (ho dovuto aggiornare l'UML di robocluedo rosplan, mi serviva un componente che non avevo ancora menzionato)
	- (l'idea è di fare il design dell'intero PDDL sul documento, in modo da non dover fare due cose assieme, design e implementazione, nella fase di implementazione, *perchè so quanto romperà il planner*)
	- pddl delle varie azioni
- e ora, implementazione
	- prima versione *completamente vuota*
	- parse ... OK (però bisogna vedere se fa il plan o no)
	- plan ... NO ... OK
		domain:
		```lisp
		(define (domain robocluedo)

		(:requirements 
			;; === PDDL1.1 === ;;
			:strips 
			:typing 
			:equality 
			:existential-preconditions
			:universal-preconditions 
			:conditional-effects
			:quantified-preconditions
			
			;; === PDDL2.1 === ;;
			:numeric-fluents
			:durative-actions
		)

		(:types
			;; navigation system
			waypoint
		)

		(:predicates
			(stub ) (not-stub )
		)

		(:functions
			
		)

		(:durative-action stub-true
			:parameters ( )
			
			:duration (= ?duration 0.99 )
			
			:condition (and 
				(at start (not-stub ))
			)
			
			:effect (and
				(at start (not (not-stub )))
				(at end (stub))
			)
		)

		)
		```
		problem:
		```lisp
		(define (problem robocluedo-task)

		(:domain robocluedo)

		(:objects
			
		)

		(:init
			(not-stub )
		)

		(:goal (stub ))

		)
		```
		plan (fino ad ora):
		```text
		root@3b17871017fd:~/ros_ws/src/erl2-new/robocluedo_rosplan/pddl/robocluedo# ./solve_popf.sh 
		Number of literals: 2
		Constructing lookup tables:
		Post filtering unreachable actions: 
		No analytic limits found, not considering limit effects of goal-only operators
		All the ground actions in this problem are compression-safe
		Initial heuristic = 1.000
		;;;; Solution Found
		; States evaluated: 2
		; Cost: 0.990
		; Time 0.00
		0.000: (stub-true)  [0.990]
		```
	- (ora, una sola azione alla volta)
	- replan
		- importazione
		- parse ... OK
		- plan ... OK
			- *precondition e effect richiedono comunque l'operatore and, anche se hanno un solo predicato al loro interno*
		- uml del replan
	- move-to
		- importazione
			- meglio separare `move-to` dall'ottenere l'hint ... nuova azione `signal-collect-hint`
		- parse ...
		- plan ...
		- uml dell'azione implementata in ROS
	- signal-collect-hint
		- importazione
		- parse ...
		- plan ... OK
		
		plan finora: 
		```text
		; States evaluated: 14
		; Cost: 9.909
		; Time 0.00
		0.000: (replan)  [0.990]
		0.991: (move-to center wp1)  [0.990]
		1.982: (signal-collect-hint wp1)  [0.990]
		2.973: (collect-hint wp1)  [0.990]
		3.964: (move-to wp1 wp2)  [0.990]
		4.955: (move-to wp2 wp3)  [0.990]
		5.946: (signal-collect-hint wp3)  [0.990]
		6.937: (collect-hint wp3)  [0.990]
		7.928: (move-to wp3 wp4)  [0.990]
		8.919: (signal-collect-hint wp4)  [0.990]
		```
		per il goal
		```lisp
		(:goal (and (not-dirty ) (at wp4 ) (hint-ready ) (hint-collected wp1) (hint-collected wp3 )))
		```
		
		- uml
	- move to center
		- importazione
		- solve
		
		ultimo output:
		
		```text
		# ./solve_popf.sh 
		Number of literals: 28
		Constructing lookup tables: [10%] [20%] [30%] [40%] [50%] [60%] [70%] [80%] [90%] [100%] [110%] [120%] [130%] [140%] [150%] [160%]
		Post filtering unreachable actions:  [10%] [20%] [30%] [40%] [50%] [60%] [70%] [80%] [90%] [100%] [110%] [120%] [130%] [140%] [150%] [160%]
		No analytic limits found, not considering limit effects of goal-only operators
		93% of the ground temporal actions in this problem are compression-safe
		Initial heuristic = 8.000
		b (7.000 | 0.990)b (6.000 | 0.990)b (5.000 | 2.972)b (4.000 | 5.945)b (3.000 | 6.936)b (2.000 | 7.927)b (1.000 | 8.918);;;; Solution Found
		; States evaluated: 15
		; Cost: 9.909
		; Time 0.00
		0.000: (replan)  [0.990]
		0.991: (move-to center wp1)  [0.990]
		1.982: (signal-collect-hint wp1)  [0.990]
		2.973: (collect-hint wp1)  [0.990]
		3.964: (move-to wp1 wp2)  [0.990]
		4.955: (move-to wp2 wp3)  [0.990]
		5.946: (signal-collect-hint wp3)  [0.990]
		6.937: (collect-hint wp3)  [0.990]
		7.928: (move-to wp3 wp4)  [0.990]
		8.919: (move-to-center wp4 center)  [0.990]
		```
		
		per il goal:
		
		```lisp
		(:goal (and (not-dirty ) (hint-collected wp1) (hint-collected wp3 ) (at-center )))
		```
- **COMMIT** : "working on PDDL (not yet finished)"
- ancora lavoro sul PDDL
	- azione solve
- ora chiariamo la questione dei landmark
- **COMMIT** : "working on PDDL (end of the job)"

---

il pddl di base è pronto. adesso, passiamo al package:

- una descrizione formale della KB
	- un UML sistemato della KB di ROSPlan
	- senza dovermi reinventare nulla, dovrei avere del codice già pronto dalla vecchia repo... 
- **ancora meglio** : ho direttamente le classi!
	- importo il codice nella repo nuova allora, e lo rivedo un attimino
	- il codice va un attimino semplificato... ma soprattutto reso più comprensibile
- kb tools pezzo per pezzo
	- le macro per il log non mi piacciono molto...
		
		```c++
		#define NODE_NAME "kb_tools"

		#ifndef __DEBUG_MACROS__
		#define __DEBUG_MACROS__

		#define LOGSQUARE( str )  "[" << str << "] "
		#define OUTLABEL          LOGSQUARE( NODE_NAME )
		#define TLOG( msg )       ROS_INFO_STREAM( OUTLABEL << msg )
		#define TWARN( msg )      ROS_WARN_STREAM( OUTLABEL << "WARNING: " << msg )
		#define TERR( msg )       ROS_WARN_STREAM( OUTLABEL << "ERROR: " << msg )

		#endif
		```
		
		non mi piacciono perchè fanno spuntare troppi warning inutili...
		
	- via la db mode!
		- un compromesso: piuttosto che avere la variabile al costruttore, posso definire una macro...
	- meglio fare anche il codice in parallelo oltre che l'header
	- per il momento, meglio *eliminare la parte dei fluents* dato che comunque per l'attuale progetto non mi servono. Posso pensare di reintrodurli più avanti.
	- anche la sezione *other queries* si può eliminare, non mi è mai servita nemmeno per il precedente progetto
		- già che ci sono, documento la lettura di un predicate da codice
	- escluderei l'altra libreria... non mi sembra molto utile, e comunque voglio mantenere tutto sul semplice
- prima di andare avanti, *prova a compilare la libreria*
- assumiamo che kb tools funzioni a dovere (dovrebbe, l'ho usata nel vecchio progetto e andava bene)
	- UML di kb_tools
- **COMMIT** : "re-adaptation of kb_tools"
- manca ancora una cosa prima di poter implementare ad esempio il sistema di landmark: *come settare un goal* ?
	- documentazione sui goal
	- implementazione dei due metodi per i goal ... anzi, solo il setter, il getter è noioso da fare e probabilmente non mi servirà
	- aggiornamento degli uml di ROSplan
- compila? compila.
- **COMMIT** : "set goal method"

---

implementazione dell'interfaccia minima con la kb

- launch file per ROSPlan
	- copiato dal vecchio progetto
	- avviare si avvia senza problemi
- già che ci sono voglio fare qualche prova sui goals

problem instance:

```bash
rosservice call /rosplan_problem_interface/problem_generation_server
rostopic echo /rosplan_problem_interface/problem_instance -n 1 -p

```

ottenere l'attuale goal:

```text
# rosservice call /rosplan_knowledge_base/state/goals "predicate_name: ''" 
attributes: 
  - 
    knowledge_type: 1
    initial_time: 
      secs: 0
      nsecs:         0
    is_negative: False
    instance_type: ''
    instance_name: ''
    attribute_name: "dirty"
    values: []
    function_value: 0.0
    optimization: ''
    expr: 
      tokens: []
    ineq: 
      comparison_type: 0
      LHS: 
        tokens: []
      RHS: 
        tokens: []
      grounded: False
  - 
    knowledge_type: 1
    initial_time: 
      secs: 0
      nsecs:         0
    is_negative: False
    instance_type: ''
    instance_name: ''
    attribute_name: "hint-collected"
    values: 
      - 
        key: "wp"
        value: "wp1"
    function_value: 0.0
    optimization: ''
    expr: 
      tokens: []
    ineq: 
      comparison_type: 0
      LHS: 
        tokens: []
      RHS: 
        tokens: []
      grounded: False
  - 
    knowledge_type: 1
    initial_time: 
      secs: 0
      nsecs:         0
    is_negative: False
    instance_type: ''
    instance_name: ''
    attribute_name: "hint-collected"
    values: 
      - 
        key: "wp"
        value: "wp3"
    function_value: 0.0
    optimization: ''
    expr: 
      tokens: []
    ineq: 
      comparison_type: 0
      LHS: 
        tokens: []
      RHS: 
        tokens: []
      grounded: False
  - 
    knowledge_type: 1
    initial_time: 
      secs: 0
      nsecs:         0
    is_negative: False
    instance_type: ''
    instance_name: ''
    attribute_name: "at-center"
    values: []
    function_value: 0.0
    optimization: ''
    expr: 
      tokens: []
    ineq: 
      comparison_type: 0
      LHS: 
        tokens: []
      RHS: 
        tokens: []
      grounded: False

```

- (altre prove più avanti, mi rifiuto di scrivere chissà che sulla shell)
- **NUOVO NODO** : (c++) `kb_interface`
	- uml del nuovo nodo
	- template del codice
	- apertura dell'unico servizio del nodo
- compila? compila.
- ora occupiamoci di fare il replan. l'idea sarebbe di fare un ciclo "stupido" su un tot di predicati con parametri.
	- anzitutto, struttura degli elementi del PDDL nel codice c++
	- scrittura del codice nel costruttore, **PER ORA SOLO INIT**
	- compilare compila (a runtime bisogna vedere...)
	- replan: il ciclo stupido di cui si parlava sopra
- alcune domande:
	- *è necessario resettare anche il goal?* penso proprio di no
	- *serve un servizio per scrivere il goal?* assolutamente sì
- servizio per indicare il goal
	- aggiornamento UML
	- creazione del nuovo tipo di servizio
	- ora, servirà definire i vari predicati (il principio è sempre quello del ciclo stupido)
	- apertura del nuovo servizio
	- compilare compila (ora bisogna vedere se funge)
- un minimo testing per il nodo
	```bash
	roslaunch robocluedo_rosplan load_rosplan.launch
	
	rosrun robocluedo_rosplan kb_interface
	
	rosservice call /rosplan_problem_interface/problem_generation_server
	
	# rostopic echo /rosplan_problem_interface/problem_instance -n 1 -p
	
	rosservice list
	# /update_goal
	# /replan
	```
	azione di replan:
	```bash
	rosservice call /replan "{}" 
	
	rosservice call /rosplan_problem_interface/problem_generation_server
	
	rosservice call /rosplan_knowledge_base/clear 
	
	rosservice call /replan "{}" 
	
	rosservice call /rosplan_problem_interface/problem_generation_server
	
	# rostopic echo /rosplan_problem_interface/problem_instance -n 1 -p
	```
	landmarks:
	```bash
	rosservice call /update_goal "landmark: 0" 
	
	rosservice call /rosplan_problem_interface/problem_generation_server
	
	rosservice call /update_goal "landmark: 1" 
	
	rosservice call /rosplan_problem_interface/problem_generation_server
	
	rosservice call /update_goal "landmark: 2" 
	
	rosservice call /rosplan_problem_interface/problem_generation_server
	
	```
- direi che funziona! bene bene bene
- **COMMIT** : "kb interface first version (seems stable)"

---

sperando che il nodo  appena implementato funzioni a dovere senza sorprese, andiamo avanti col pipeline manager. *stavolta lo voglio implementare in Python*.

- **NUOVO NODO** : rosplan_pipeline_manager.py
	- primo template del nodo (penso stavolta farò un nodo semplice, procedurale, tanto sono solo servizi)
	- subito, UML del nodo e degli elementi principali di ROSplan
	- ora, *usando l'UML che ho appena fatto*, implemento i servizi per il pipeline manager (manca il feedback per ora)
	- ho preparato un minimo template per l'apertura dei client in ROSpy:
		```py
		def open_cl( cl_name, cl_type, SRV_TIMEOUT=60 ):
			'''open a client
			'''
			
			global NODE_NAME
			
			rospy.loginfo(f"({NODE_NAME}) client: {cl_name} ... ")
			try:
				rospy.wait_for_service( cl_name, timeout=SRV_TIMEOUT )
				
			except ROSException as e:
				rospy.logwarn(f"({NODE_NAME}) client: {cl_name} UNABLE TO CONTACT within the timeout ({SRV_TIMEOUT}s) cause: {e}")
				raise e
				
			cl_this = rospy.ServiceProxy( cl_name, cl_type )
			rospy.loginfo("OK")
			
			rospy.sleep(rospy.Duration(0.75))
			
			return cl_this
		```
	- ora che le interfacce le abbiamo, abbozzo un UML del nodo (mancano ancora parecchie cose)
- vediamo un po' di riadattare se possibile il vecchio sistema di feedback ... possibile?
	- posso ... riadattare l'idea, ma il vecchio sistema in se va riadattato
	- partiamo dallo scrivere il messaggio di feedback
	- ora il servizio implementato dal pipeline manager
	- compila? compila, dopo qualche bestemmia
		- ricorda di inserire nel cmake anche *message_runtime* oltre a *message_generation*, altrimenti non si riesce ad importare i messaggi nei servizi nell'ambito dello stesso package
	- (voglio mantenere l'idea del feedback da C++)
	- aggiornamento dell'UML del nodo includendo anche il feedback manager
	- e apriamo il servizio e il topic
- adesso che il nodo ha tutte le interfacce esposte, lavoro per aggiungere le funzionalità una ad una
	- *caricamento del problema*
		- serve ancora una subscription, altrimenti non riesco a capire quando il load non ha avuto successo
		- prima di andare avanti, UML (sarà lunga stasera...)
		- ovviamente manca anche la parte in cui scrivo i landmark nel problema... aggiungere
		- e dopo averla implementata, aggiornare l'UML
	- e test intermedio (mai fidarsi di Python)
		```
		# shell 1
		roslaunch robocluedo_rosplan load_rosplan.launch
		
		# shell 2
		rosrun robocluedo_rosplan kb_interface
		
		# shell 3
		rosrun robocluedo_rosplan rosplan_pipeline_manager.py
		
		# shell 4
		rosservice list | grep robocluedo
		
		rosservice call /robocluedo/pipeline_manager "{load_problem: false, solve_problem: false, parse_plan: false, execute_plan: false, landmark: 0}" 
		
		rosservice call /robocluedo/pipeline_manager "{load_problem: true, solve_problem: false, parse_plan: false, execute_plan: false, landmark: 0}" 
		
		rosservice call /robocluedo/pipeline_manager "{load_problem: true, solve_problem: false, parse_plan: false, execute_plan: false, landmark: 1}" 
		
		rosservice call /robocluedo/pipeline_manager "{load_problem: true, solve_problem: false, parse_plan: false, execute_plan: false, landmark: 2}" 
		
		rosservice call /robocluedo/pipeline_manager "{load_problem: true, solve_problem: false, parse_plan: false, execute_plan: false, landmark: 3}" 
		
		rosservice call /robocluedo/pipeline_manager "{load_problem: true, solve_problem: false, parse_plan: false, execute_plan: false, landmark: -1}" 
		```
- *per oggi ho sofferto abbastanza*
- **COMMIT** : "working on pipeline manager (adding functionalities)"

---

## 10/08/2021 -- ancora su rosplan

torniamo a lavorare sul pipeline manager, una funzionalità per volta. Ieri: l'istanza del problema. 

- *soluzione del problema* usando il planner
	- implementazione
	- serve anche aprire il subscriber con il planner per capire quando il plan è andato bene
	- *come capire se il problema non è risolvibile o se c'è stato qualcos'altro che non va?* 
	- per provare, rendo volutamente irrisolvibile il problema:
		```
		(:goal (and (dirty ) (not-dirty ) (hint-collected wp1) (hint-collected wp3 ) (at-center )))
		```
	- output lato rosplan:
		```
		[ INFO] [1660112152.481829500]: KCL: (/rosplan_problem_interface) (OUT.pddl) Generating problem file.
		[ INFO] [1660112152.829298800]: KCL: (/rosplan_problem_interface) (OUT.pddl) The problem was generated.
		[ INFO] [1660112152.829847100]: KCL: (/rosplan_planner_interface) Problem received.
		[ INFO] [1660112162.234649900]: KCL: (/rosplan_planner_interface) (OUT.pddl) Writing problem to file.
		[ INFO] [1660112162.234786600]: KCL: (/rosplan_planner_interface) (OUT.pddl) Running: timeout 10 /root/ros_ws/src/ROSPlan/rosplan_planning_system/common/bin/popf -T /root/ros_ws/src/erl2-new/robocluedo_rosplan/pddl/robocluedo/robocluedo_domain.pddl /root/ros_ws/src/erl2-new/robocluedo_rosplan/data/OUT.pddl > /root/ros_ws/src/erl2-new/robocluedo_rosplan/data/plan.pddl
		[ INFO] [1660112172.240622500]: KCL: (/rosplan_planner_interface) (OUT.pddl) Planning complete
		[ INFO] [1660112172.240715400]: KCL: (/rosplan_planner_interface) (OUT.pddl) Plan was unsolvable.
		```
	- output lato shell:
		```
		root@3b17871017fd:~/ros_ws/src/erl2-new/robocluedo_rosplan/launch# rosservice call /rosplan_problem_interface/problem_generation_server

		root@3b17871017fd:~/ros_ws/src/erl2-new/robocluedo_rosplan/launch# rosservice call /rosplan_planner_interface/planning_server
		ERROR: service [/rosplan_planner_interface/planning_server] responded with an error: b''
		root@3b17871017fd:~/ros_ws/src/erl2-new/robocluedo_rosplan/launch# 
		```
	- il topic non riceve nulla, quindi significa che il plan non è andato a buon fine
	- il goal che ho scelto causa attesa (in questo caso viene sollevata un'eccezione riguadante il server)
	- mi serve un goal che dia contraddizione immediata. questo funziona:
		```
		(:goal (and (not-is-center center )))
		```
	- anche in questo caso non viene inviato nulla 
	- il file generato dal planner contiene questo:
		```
		Number of literals: 28
		Constructing lookup tables: [10%] [20%] [30%] [40%] [50%] [60%] [70%] [80%] [90%] [100%] [110%] [120%] [130%] [140%] [150%] [160%] [170%]
		A problem has been encountered, and the problem has been deemed unsolvable
		--------------------------------------------------------------------------
		The goal fact:
		(not-is-center center)

		...cannot be found either in the initial state, as an add effect of an
		action, or as a timed initial literal.  As such, the problem has been deemed
		unsolvable.
		```
	- per capire se il problema non è risolvibile, occore
		- eliminare il precedente file di plan
		- caricare nel parameter server il percorso dove verrà caricato il file di plan dal planner
		- leggere il file di plan e cercare all'interno di esso la stringa "the problem has been deemed unsolvable"
		- se la stringa c'è, allora il problema è chiaramente irrisolvibile, altrimenti (il file non esiste proprio oppure è diverso) il problema è di altra natura
	- implementazione ...
		- caricamento percorso nel parameter server tramite launch file
		- lavoro col file di plan
		- ora manca solo l'eccezione
	- e urge una prova a questo punto: per ora nel caso del plan unsolvable immediatamente
		```
		# shell 1
		roslaunch robocluedo_rosplan load_rosplan.launch

		# shell 2
		rosrun robocluedo_rosplan kb_interface

		# shell 3
		rosrun robocluedo_rosplan rosplan_pipeline_manager.py
		
		rosservice call /robocluedo/pipeline_manager "{load_problem: true, solve_problem: true, parse_plan: false, execute_plan: false, landmark: 0}" 
		```
	- pare che funzioni. tutti i landmark paiono portare ad una soluzione
	- **ISSUE** (ma basta solo stare attenti): il goal col fallimento immediato `(:goal (and (not-is-center center )))` rimane nel goal, non viene eliminata dalla kb interface. 
- **COMMIT** : "working on pipeline manager (planning interface)"
- *parsing del plan*
	- implementazione (solo trigger per ora)
	- e test ... funge!
- **COMMIT** : "working on pipeline manager (parsing interface)"
- prima di andare avanti mi servono le simulated actions
	- posso implementare una prima versione del dispatcher (per ora senza il feedback)
	- test
		```
		rosservice call /robocluedo/pipeline_manager "{load_problem: true, solve_problem: true, parse_plan: true, execute_plan: true, landmark: 0}" 
		```
- per quanto riguarda il pipeline manager questo è tutto ciò che posso fare ora.
- **COMMIT** : "working on pipeline manager (simulated action and dispatch)"

--- 

passiamo a lavorare sul feedback manager:

- anzitutto lettura del vecchio codice (l'idea è la stessa: una classe che posso usare da padre per altre classi, o che posso istanziare)
- la classe precedente non è più utilizzabile, però posso riadattarla, stesso lavoro di kb_tools direi
	- creazione header e cpp file
- adesso, una cosa per volta, a partire dall'header
	- librerie e "contorno"
	- funzione per la scrittura rapida del messaggio
- per ora basta che compili (i feedback li scrivo man mano che scrivo le azioni)
	- compila? compila.
- UML del feedback manager (per ora senza indicare i tipi di feedback)
- **COMMIT** : "working on feedback manager (first basic implementation)"

---

meglio iniziare a strutturare le implementazioni delle azioni PDDL:

- prima, meglio farsi un template minimo per gli header
	
	header: (action_name.h)
	
	```c++
	
	```
	
	implementazione della classe: (action_name.cpp)
	
	```c++

	```
	
	nodo che implementa la classe: (action\_name_node.cpp)
	
	```c++
	
	```
	
	catkin:
	
	```cmake
	add_library( action_name src/robocluedo_rosplan_actions/action_name.cpp )
	target_link_libraries( action_name ${catkin_LIBRARIES} feedback_manager kb_tools rosplan_action_interface )
	add_executable( node_action_name src/robocluedo_rosplan_actions/action_name_node.cpp )
	add_dependencies( node_action_name ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS} )
	target_link_libraries( node_action_name ${catkin_LIBRARIES} feedback_manager kb_tools rosplan_action_interface action_name )
	```
	
- compila? 
	- [ricorda questo errore!](https://cplusplus.com/forum/general/203321/) piuttosto fastidioso da risolvere
	- dopo aver tribolato mezz'ora, ci siamo
- adesso possiamo partire con l'implementazione delle azioni
- *replan*
	- creazione delle azioni
	- UML dell'azione, giusto per capire che canali aprire
	- implementazione
	- cmake
	- compila? compila.
- *move-to*
	- creazione dell'azione
	- UML della action 
	- manca un servizio per move_to ... 
	- apertura delle interfacce (ne serve solo una)
	- implementazione
	- e cmake
	- compila? compila.
- **COMMIT** : "working on robocluedo_rosplan_actions (replan, move_to)"
- *collect-hint*
	- *pessimo design* : meglio ridimensionare questa azione
	- *manca il servizio manipulation!*
	- creazione dei file per l'implementazione dell'azione
	- implementazione
	- cmake
	- compila? compila.
- *move-to-center* è praticamente identica alla *move-to* di prima
	- uml copiato e incollato
	- l'implementazione è praticamente identica a *move-to*
	- cmake
	- compila? ovviamente compila.
- la *solve* è una simulated action: dato che solo il mission manager può accedere alla ontology, la solve perde di ogni utilità
- **COMMIT** : "working on robocluedo_rosplan_actions (end of the job)"
- implementare (solo per il testing) un nodo che imiti navigation e manipulation
	- **NUOVO NODO** : rosplan_simulated_motion_system
	- implementazione e installazione
	- e un minimo testing
- (manca solo da chiarire il sistema di feedback)
- feedback "hw manipulation failure"
	- ...e tutti gli altri tipi di feedback
- compila? compila
- ora, tutti i feedback in tutte le azioni
	- in collect hint
	- in move to
	- in move to center
	- in replan
- **COMMIT** : "working on robocluedo_rosplan_actions (feedbacks from the actions)"
- come *ultima fatica per oggi* implementazione dell'esecuzione nella pipeline (test domani)
	- (manca solo la parte riguardande i feedback)
	- giusto un micro test per capire se è tutto a posto
		```bash
		# shell 1
		roslaunch robocluedo_rosplan load_rosplan.launch

		# shell 2
		rosrun robocluedo_rosplan kb_interface

		# shell 3 (not with simulated actions)
		# rosrun robocluedo_rosplan rosplan_simulated_motion_system.py
		
		# shell 4
		rosrun robocluedo_rosplan rosplan_pipeline_manager.py

		rosservice call /robocluedo/pipeline_manager "{load_problem: true, solve_problem: true, parse_plan: true, execute_plan: true, landmark: 2}" 
		```
- **ISSUES** che non potevo notare prima di avere il sistema completo (usando solo simulated actions)
	- (GRAVE) la sequenza dei landmark porta ad un problema irrisolvibile, vedi la combinazione tra i predicati at e explored (probabilmente è un problema della attuale kb interface che non tiene conto di questo conflitto). lo scenario di utilizzo dovrebbe essere: (landmarks) 0 1 1 1 ... 1 2 0 1 ...
	- (minore) la pipeline non segnala il raggiungimento del goal nel messaggio...
	- (minore) il log nella pipeline non segna la fine del dispatch
- **COMMIT** : "working on the pipeline manager (feedback management)"

---

## 11/08/2022 -- ancora su ROSPlan

anzitutto, vorrei lavorare sulle issue di ieri: il fatto che la sequenza di landmark che mi aspetto non funzioni come si deve è un problema grave che va risolto al più presto. 

- un test più approfondito:

```bash
# shell 1
roslaunch robocluedo_rosplan load_rosplan.launch

# shell 2
rosrun robocluedo_rosplan kb_interface

# shell 3
rosrun robocluedo_rosplan rosplan_pipeline_manager.py

# shell 4
#     landmarks: 0 1 1 1 2
rosservice call /robocluedo/pipeline_manager "{load_problem: true, solve_problem: true, parse_plan: true, execute_plan: true, landmark: 0}" 
rosservice call /robocluedo/pipeline_manager "{load_problem: true, solve_problem: true, parse_plan: true, execute_plan: true, landmark: 1}" 
rosservice call /robocluedo/pipeline_manager "{load_problem: true, solve_problem: true, parse_plan: true, execute_plan: true, landmark: 1}" 
rosservice call /robocluedo/pipeline_manager "{load_problem: true, solve_problem: true, parse_plan: true, execute_plan: true, landmark: 1}" 
rosservice call /robocluedo/pipeline_manager "{load_problem: true, solve_problem: true, parse_plan: true, execute_plan: true, landmark: 2}"
```

- **ALTRO ASPETTO DA AFFINARE**: il feedback di successo managi non servirà, però va comunque scritto nel messaggio di ritorno.
	- (lo faccio adesso)
	- test ... e ok!
- ora, sostituisco le simulated actions con le mie implementazioni

```xml
<node name="rosplan_interface_???" pkg="robocluedo_rosplan" type="node_???" respawn="false" output="screen">

	<param name="knowledge_base"		value="rosplan_knowledge_base" />
	<param name="pddl_action_name"		value="???" />
	<param name="action_dispatch_topic"	value="/rosplan_plan_dispatcher/action_dispatch" />
	<param name="action_feedback_topic"	value="/rosplan_plan_dispatcher/action_feedback" />
	
	<param name="action_duration"		value="0.99" />
	<param name="action_probability"	value="1.0" />

</node>
```

- e test delle actions reali
	- *mi sono dimenticato di compilare move_to_center!*

```bash
# shell 1
roslaunch robocluedo_rosplan load_rosplan.launch

# shell 2
rosrun robocluedo_rosplan kb_interface

# shell 3
rosrun robocluedo_rosplan rosplan_simulated_motion_system.py

# shell 4
rosrun robocluedo_rosplan rosplan_pipeline_manager.py

# shell 5
#     landmarks: 0 1 1 1 2 0 1 2

rosservice call /robocluedo/pipeline_manager "{load_problem: true, solve_problem: true, parse_plan: true, execute_plan: true, landmark: 0}" 
rosservice call /robocluedo/pipeline_manager "{load_problem: true, solve_problem: true, parse_plan: true, execute_plan: true, landmark: 1}" 
rosservice call /robocluedo/pipeline_manager "{load_problem: true, solve_problem: true, parse_plan: true, execute_plan: true, landmark: 1}" 
rosservice call /robocluedo/pipeline_manager "{load_problem: true, solve_problem: true, parse_plan: true, execute_plan: true, landmark: 1}" 
rosservice call /robocluedo/pipeline_manager "{load_problem: true, solve_problem: true, parse_plan: true, execute_plan: true, landmark: 2}"

# ERRORE problema non risolvibile (correttamente segnalato dal pipeline manager)
rosservice call /robocluedo/pipeline_manager "{load_problem: true, solve_problem: true, parse_plan: true, execute_plan: true, landmark: 1}" 

# problema risolvibile
rosservice call /robocluedo/pipeline_manager "{load_problem: true, solve_problem: true, parse_plan: true, execute_plan: true, landmark: 0}" 
rosservice call /robocluedo/pipeline_manager "{load_problem: true, solve_problem: true, parse_plan: true, execute_plan: true, landmark: 1}" 
rosservice call /robocluedo/pipeline_manager "{load_problem: true, solve_problem: true, parse_plan: true, execute_plan: true, landmark: 2}"
```

- (alla fine la issue di ieri era ... semplice sonno)
- **COMMIT** : "working on robocluedo rosplan actions (launch file)"


	
	





TODO

- discutere l'utilità del landmark zero... è davvero così utile avere un'azione che faccia fare replan?
- **gestire meglio l'invio dei feedback**. CI sono vari problemi:
	- il feedback dev'essere inviato *solo quando si presenta un problema* e il pipeline manager dev'essere pronto a riceverlo
	- va inviato solo ed esclusivamente un feedback alla volta
	- il feedback di successo non esiste, è inutile
- *module testing* su kb_tools

- implementazione precisa dell'azione move-to

- in tutti gli UML dei componenti delle implementazioni delle azioni PDDL c'è una parte comune di feedback: scrivila una volta per tutte e copia-incolla in tutti i diagrammi
- aggiungere le pagine di documentazione del codice di Sphinx!
- aggiornare i template delle actions in questo file
- rimuovere le immagini dalla documentazione del codice per armor
- un branch per la documentazione Sphinx
- e uno script per mettere online la documentazione sphinx "senza sbattersi troppo"
- approfondire UML armor
- aggiornare il file di descrizione e design del PDDL con il codice corretto
	- correggere i landmark, oppure eliminare direttamente la sezione

- autenticazione SSH sul Docker di lavoro

NOTE

- la pagina di documentaizone del codice va assieme al codice, invece che in una cartella separata; penso sia meglio.
