
# RoboCLuedo -- Development Log

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

	





TODO

- nuovi diagrammi per aRMOR (quelli che ci sono sono sbagliati...)
- rimuovere le immagini dalla documentazione del codice per armor
- template per le pagine di documentazione con UML
- un branch per la documentazione Sphinx
- e uno script per mettere online la documentazione sphinx "senza sbattersi troppo"
- autenticazione SSH sul Docker di lavoro

NOTE

- la pagina di documentaizone del codice va assieme al codice, invece che in una cartella separata; penso sia meglio.
