
# the RoboCLuedo Project no.2 -- robot URDF model -- User Manual

```{toctree}
./robocluedo-urdf-user-manual.md
```

inside the folder `robocluedo_urdf_model` there are these files:

- `generate_model.sh` is a script to tect the correctness of the model; it also generates a `.pdf` schematic of the model

- `robocluedo_xacro.xacro` is (as the name suggests) the XACRO model of a robot labeled **robocluedo_robot**. the model is split into three files:
	- `robocluedo_gazebo_materials` : graphical appearance of the robot
	- `robocluedo_chassis` : the moving platform of the robot
	- `robocluedo_arm` : the robotics arm
	- `robocluedo_arm_gripper` : the gripper of the robotic arm; it includes the link labeled *cluedo_link*
	- `robocluedo_gazebo_sensing` : definition of the Gazebo pluging related to the sensing; vision, laser, everything inside this file
	- `robocluedo_sensing` : this file contains the sensors mounted on the robot
	- `robocluedo_gazebo_plugins` : definition of the Gazebo plugins, without the sensing part
	- `robocluedo_transmission` : Gazebo controllers

## HOW TO generate the model

execute the file `generate_model.sh`; the URDF and its schematic will be located into the folder `model` with names 

- `robocluedo_urdf.urdf` for the URDF model, 
- and `robocluedo_urdf.pdf` for the schematic

the output should be like the following:

```text
robot name is: robocluedo_robot
---------- Successfully Parsed XML ---------------
root Link: base_link has 4 child(ren)
    child(1):  arm_base_link
        child(1):  arm_link_01
            child(1):  arm_link_02
                child(1):  arm_link_03
                    child(1):  cluedo_link
                        child(1):  left_grip_link
                        child(2):  right_grip_link
    child(2):  laser
    child(3):  link_left_wheel
    child(4):  link_right_wheel
Created file robocluedo_robot.gv
Created file robocluedo_robot.pdf
```
