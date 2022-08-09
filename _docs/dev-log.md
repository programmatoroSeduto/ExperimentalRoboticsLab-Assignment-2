
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

## 09/08/2022 -- 

- serve uno *schema completo* dell'applicazione prima di iniziare
	- components robocluedo rosplan (solo bozza)
	- *meglio lavorare in MD* : nessuna tabulazione fastidiosa, è più facile lavorarci su
	- components armor 
		- (ricorda di usare l'opzione `allow_mixing` all'inizio del codice per poter usare diversi tipi di UML nello stesso schema)
	- component movement controller
	- **il mission manager non lo faccio adesso** perchè è il nodo che va a "legare" tutte le parti dell'architettura. quindi, meglio lasciarlo a quando avrò un'idea chiara
- **COMMIT** : "uml arch first draft"




	





TODO

- rimuovere le immagini dalla documentazione del codice per armor
- un branch per la documentazione Sphinx
- e uno script per mettere online la documentazione sphinx "senza sbattersi troppo"
- autenticazione SSH sul Docker di lavoro
- approfondire UML armor

NOTE

- la pagina di documentaizone del codice va assieme al codice, invece che in una cartella separata; penso sia meglio.
