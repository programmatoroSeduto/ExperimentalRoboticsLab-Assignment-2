
.. _pkg-robocluedo-movement-controller:

PKG DOCS -- robocluedo_movement_controller
==========================================

this package enables the robot to move the manipulator and to navigate into the environment, usign different methods.

here's a brief list of the nodes inside the package: 

- **bug_m.py** (composite behaviour) : the node performs a very simplified navigation point to point combining the two elementary behaviours *go_to_point* and *head_orientation*. 
	
	The node also implements strategie to gain manouver space before pointing the head in the direction of the target. 

- **go_to_point_service_m.py** (elementary behaviour) : the node aligns the head in the direction of the target and moves along a straight line to the objective position
	
	the node is used to implement a very simplified navigation algorithm with no obstacles between the current position and the target one. It is switchable. 

- **head_orientation.py** (elementary behaviour) : orient the head with a given angle wrt the world frame
	
	the robot keeps turning around its vertical axis until the orientation isn't equal to the target one.

- **navigation_manager.cpp** : a high-level navigation service based on the concept of the controllers
	
	the node has two main objectives:
	
	- to provide a handy *abstraction* between the real implementation of tbe navigation system
	- and the service which is simply interested in reaching a (x,y,th) target
	
	.. note::
		(*RCL#2*) the project implements only one controller so far, using the bug_m implementation, with ID=0. 
		The idea of the controllers is more related to furure developments of the project instead of a advantage in this particular version. 

- **manipulation_controller.cpp** : a (too much) simple implementation of a moveit-based manipulation controller capable of moving te arm to a given *preset pose*.
	
	.. note::
		(RCL#2) the node is compatible with the first version of RoboCLuedo


.. toctree::
	:caption: User Documentation
	:glob:
	
	./_docs/*


.. toctree::
	:caption: UML diagrams
	:glob:
	
	./_uml/*


.. toctree::
	:caption: code reference
	:glob:
	
	./_code_reference/*


.. toctree::
	:caption: Package Code Documentation
	:glob:
	
	./src/*
	./scripts/*


.. toctree::
	:caption: Code Examples
	:glob:
	
	./_examples/*
