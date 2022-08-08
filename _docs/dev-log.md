
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

prima del primo commit,

- creazione di una nuova repository per il progetto, link [qui](https://github.com/programmatoroSeduto/ExperimentalRoboticsLab-Assignment-2.git)
- craeazione di un template di repository che permetta di usare plantUML (è finito il tempo di scrivere gli UML su draw.io) vedi [qui (privata)](https://github.com/programmatoroSeduto/plantuml-sphinx)
- **primo setup dell'area di lavoro** con plantUML e Sphinx funzionante
- struttura readme, solo elenco puntato per il momento (estendere man mano che andiamo avanti)
- **COMMIT** creazione di un branch vuoto per la copia degli altri eventuali pacchetti da scaricare
- **COMMIT** con tutto il materiale iniziale funzionante

*ora siamo pronti per iniziare a lavorare con ROS*. Prima di tutto, il package per il module testing:

- creazione package per il module testing:
	```
	catkin_create_pkg robocluedo_module_testing roscpp rospy std_msgs geometry_msgs nav_msgs std_srvs
	``` 
- documentazione interna al module testing con index e tutto il resto
- creazione template per la pagina index di un package
- qualche check prima del commit:
	- prova a compilare .. .OK
	- make documentazione .. OK
- **COMMIT** : 



TODO

- templates per il CMakeLists (praticamente continuare il lavoro del vecchio assignment)
- template per le pagine di documentazione con UML
- un branch per la documentazione Sphinx
- e uno script per mettere online la documentazione sphinx "senza sbattersi troppo"
- autenticazione SSH sul Docker di lavoro

NOTE

- la pagina di documentaizone del codice va assieme al codice, invece che in una cartella separata; penso sia meglio.
