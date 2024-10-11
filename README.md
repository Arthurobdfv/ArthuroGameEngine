# ArthuroGameEngine
# Table of Contents
1. [Introduction](#introduction)
2. [The Project](#the-project)
3. [The To-do list](#the-to-dos)

## Introduction
Welcome to my game engine repository! The initial idea with this project is to learn about OpenGL while sharpening my skills in C++ and its building system with CMake and other tools. <br>
I started coding in C++ in an university class while I was attending engineering classes and instantly fell in love with it, though later on in my career I shifted to Csharp as I wanted to get into the game development world and the easiest way was to grab an already-existing engine, on which I chose Unity. Now, after some years working professionally as a .Net developer and being a Hobbyist in game development, I decided to sharpen my skills into C++ and computer graphics to land a job in the video game industry, and building a game engine would be the perfect project to get my skills to the industry standards level! <br>
What I hope to learn out of this journey:
* OpenGL
* Computer Graphics and rendering pipelines
* Sharpen my C++ skills
* CMake and multi-platform building in the C++ environment (Web and Desktop builds)
* The "Behind the scenes" of game engines.
* Applied computer graphics Maths

## The Project
The idea of the projec is to end up with a small game engine that enables the user to create games and export those either as executables (both for WebGL and Desktop OSs) or scene-descripting .json files to share with friends and then import them into the engine to be able to play. To achieve that, I have been following the LearnOpenGL PDF tutorial series to get a grasp of how the low-level rendering pipeline works, while also getting the best out of my Software-Engineering skills to design other parts of the game engine in a clean and efficient way.<br> 
As I progress through the contents of the LearnOpenGL series, I will be pushing commits with the chapters I have concluded as well as extra clean-ups or set ups I have done. I will also update the "To do list" making in what stage I am currently.

## The To-dos
In this section I will keep the overall features and milestones I want to achieve as well as marking them as completed to keep track of the engine and my studies progress. <br>
This list will be constantly changed as I plan and design the game engine.
### **Studies** - Currently at Chapter 6.3 of LearnOpenGL series.

### **Engine Features and Progress**
- [ ] Refactor the setting up of VAO into different classes
    - [ ] Create a EBOModelRenderer to render EBO-Descripted Models
    - [ ] Create a VBOModelRenderer to render VBO-Descripted Models
- [ ] Rendering
    - [ ] Refactor the Shader setting-up strategy to better create / use shader programs
    - [ ] Create the concept of materials, maybe? See how this will affect the overall design of the engine
    - [ ] Improving the drawcalls, currently every object has its own drawcall which is bad. (Learn about Batching)

- [ ] Create a 3D model imported for different file formats
    - [ ] OBJ File Importer
- [ ] CI/CD of the Engine
    - [ ] Using CMake to build into desktop Standalones
    - [ ] Using CMake to build into Web builds
    - [ ] Github actions setup and integration with previous build strategies
- [ ] Engine Structure
    - [ ] Design physics engine.
    - [ ] Design how game objects will have their components.

