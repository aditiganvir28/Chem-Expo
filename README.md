# Chem-Expo

### CS-352: Computer Graphics and Visualisation Course Project
### Under Prof. Somnath Dey, IIT Indore
### Team Members: Aditi Ganvir (210001016), Princy Sondarva (210001068), Manas Wagh (210001038).

Chem-Expo is a project aimed at demonstrating chemical reactions through fully animated visuals, combining Computer Graphics and Visualization principles. We focused on two well-known reactions: the formation of sulfur trioxide (SO2 + O2 → SO3) and the Decomposition of Sodium Bi Carbonate (NaHCO3 → Na2CO3 + H2O + CO2). 

### Demo Video Link - https://drive.google.com/file/d/1OsBBYy-6fkQGbtQrVuszhREi3YWLnnlO/view?usp=sharing

### Instructions to run

 All we need to run the project are the OpenGL library setup and SOIL library included in Ubuntu system. 

Clone the repository using the following command:
```
$ git clone https://github.com/aditiganvir28/Chem-Expo.git
```
We can install OpenGL using:

```
$ sudo apt-get update

$ sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev
```
Install the necessary libraries by running the following commands:

Navigate to the project directory:
```
cd Chem-Expo
```

Compile and run SO3.cpp:
```
$ g++ SO3.cpp -o SO3 -lGL -lGLU -lglut -lm -lSOIL

$ ./SO3
```
Compile and run NaHCO3.cpp:
```
$ g++ NaHCO3.cpp -o NaHCO3 -lGL -lGLU -lglut -lm -lSOIL

$ ./NaHCO3
```
That’s it! You will see the reaction window in OpenGL pop-up, with the reaction in place


#### Key controls to play around with the reaction:
*	Use Keys: W/A (to translate along x-axis), S/D(to translate along y-axis), 5/8(to translate along z-axis).
*	Use Mouse: Hold Middle Button and Drag to Zoom In / Zoom Out.
*	Use Mouse: Hold Left Button and Drag to Rotate Axis
*	Press Enter to Start/Pause animation for the reaction.
*	Press Space to Start/Pause rotation of individual molecules about their centers.
*	Press P to Change Projection types.
*	Press C to Change Camera positions.
*	Press L to Change Lighting Effects.
*	Press M to Toggle display of Legend.
*	Press R to Reset animation from the beginning.
*	Press + to Add/Remove display of Axes of the coordinate system.
*	Press – to Highlight the colors of molecules.
*	Press Esc to Close Window.



