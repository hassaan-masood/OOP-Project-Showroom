# Object Oriented Programming
## Vehicle ShowRoom & Repairing System

In order to run this Program you must pull all the **.txt** files and the **showroom.cpp** file as the showroom.cpp is the main file containing all the classes with their definitions (so VsCode users dont have link all the files manually which feels like hell atleast to me) Visual Studio Users might pull the header and cpp files as The VS compiler automatically links all the files so they dont have to Write all the code in 1 file.

---

Make sure the .txt and .cpp file is in the same directory.
In case of using .h & cpp make sure everything is in 1 folder and kindly dont change the spacing in the .txt files.

---

### Command to compile for VScode users.

Make sure you have Raylib installed and setup on your machine if not you can do it by Pacman or AUR (in case of using ARCH linux or CachyOS). 

`g++ showroom.cpp -o showroom -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -std=c++17`

Then run

`./showroom`

