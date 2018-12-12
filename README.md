# EGP-410
Template projects for base code for EGP 410 assignments.

# This repository contains three major directories:
 * DeanLib - Base utility library provided by Dean Lawson. Memory tracking and basic vector mathematics utilities.
 * External Libs - Allegro 5.0.10 support libraries.
 * Game AI - Base source code for EGP 410 Assignments
   * pathfinding - the one you'll be using
   * state machine
   * steering

Building instructions are left up to the student. 

1. A link to your commit on GitHub 
     Editor's Commit link: https://github.com/DrabbyPage/EGP-410

2. How to compile/link your project 
     To compile the code, clone the repository. After cloning go into the GameAI folder. Inside
the folder is the pathfinding folder. Lastly open the .sln file called pathfinding.sln
I recommend running the program in Visual Studios 2017. Make sure that all libraries are running
your platform's proper SDK.

3. The keyboard inputs 
     - Left Mouse Click: Sets Pac_Man's path
     - Esc: Exits the program

4. Gameplay:
	- pacman eats pips (10 points), and big pips(50 points)
	- after big pip eaten, the ghosts are killable
	- ghost wander until within range of pacman then pathfind to get to his location
	- there are speed power ups for the enemy they are red like mines