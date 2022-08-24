
(run-the-project)=

# HOW TO Run RCL#2 Project

```{seealso}
If something don't work, see the testing procedures you can find in the documentation here: {any}`project-testing`
```

Here are the instructions for running the RCL#2 project. I suggest you to use at least two shells: one for the "useless output", and another one for the output from the mission manager. 

### Shell 1

all the components, without the `mission manager`

```bash
roslaunch robocluedo_mission_manager run_components_final.launch 2>/dev/null

```

### Shell 2

the shell will show the output from the mission manager, to show how the investigation is proceeding. 

```bash
rosrun robocluedo_mission_manager mission_manager

```
