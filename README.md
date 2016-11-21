# Cyber Ball
#### Android NDK Test Project

APK is availiable for download via that link:
<a href="https://www.dropbox.com/s/i8cls1t81za58or/CyberBall.apk?dl=0">CyberBall.apk</a> 

Task Description: Make your own version of very popular (and frustrating) game using Android NDK. 
- "Bird" is flying to the right side affected by gravity. Touching the screen throws the "bird" up.
- Obstacles are columns of varying heights sticking out of the floor and ceiling.
- If the "bird" hits the column, floor or ceiling, the game is over.
- The Player's goal: fly as far as possible, earn as many points as possible

Technical Requirements:
- The game must be made using Android NDK
- Only C++ is allowed (with minimal doze of Java) 
- OpenGL ES 2.0 or higher must be used for rendering
- The game should be written *without the use* of any third-party graphic, or physical game engines and libraries. 
- NOT ALLOWED *Cocos2D, GLKit, SpriteKit, QT, GLUT, SDL, Box2d, BulletPhysics* NOT ALLOWED

I took these restrictions very seriously. So none of this libraries are present in this project, even no png loaders.

Textures and some part of geometry is hard-coded in code, which works for small demos like this, but doesn't scale well to real games.

*This game uses Android CMake plugin*

* * *
### Watch Gameplay Video!

[![youtube](https://cloud.githubusercontent.com/assets/15881137/19723195/cdd01ff0-9b82-11e6-935b-c8b27ae9edc9.png)](https://www.youtube.com/watch?v=Mx1A6CMf_cs)

### The game contains 2 main scenes:
- WelcomeScene

The game's main menu scene. It renders "Play" and "About" buttons on the interface and manages the navigation.
- PlayScene 

It is the main gameplay scene. The whole game logic is contained in the PlayScene class. I recommend to start reading from
the DoFrame() method and it should become clear. It's a standard game loop that handles input, updates the world, checks for collisions and renders.
<br>In addition to gameplay, an HUD with score and signs is presented. <br>
Best Score is stored inside the local memory of device, and loads while PlayScene is initializing. <br>
When the game is over, stats table is shown alongside with "Restart" and "Quit" buttons.

### Screenshots from the PlayScene:

<p align="center">
  <img src="https://cloud.githubusercontent.com/assets/15881137/19723429/e70e04cc-9b83-11e6-8bd3-e3f618d7ebf5.png" width="250"/>
  <img src="https://cloud.githubusercontent.com/assets/15881137/19723494/28b5970a-9b84-11e6-9e1f-e886a8c0a7ae.png" width="250"/>
  <img src="https://cloud.githubusercontent.com/assets/15881137/19723507/37b9e170-9b84-11e6-8750-854d73679903.png" width="250"/>
</p>

## Walkthrough

*Android CMake plugin is required for this game*

This game uses the OpenGL Mathematics (GLM) library, available at: http://glm.g-truc.net/ <br>
The only purpose of this library in this project was to provide vectors/matrix data structures and operations with them.

Source code contains only C++ code, which is sitting at its default location app/src/main/cpp

The game starts at the android_main function.

### Scenes/Scene Manager
SceneManager is a singleton class. There's always an active Scene, and the scene determines what's visible onscreen and how the game reacts to input.
UIScene is a modification of the basic Scene class, it handles widgets like buttons or text titles onscreen.

As the game runs, SceneManager corresponds to the events like app losing focus and being brought back into focus using StartGraphics() and KillGraphics() methods.
Everything that has to do with graphics context is initialized in StartGraphics(), and torn down in KillGraphics().

### Rendering
Everything in the game is rendered by a Shader. Geometry is represented by VertexBuffer (VBO) and IndexBuffer (IBO). <br>
Geometry coordinates array translated to VBO contain:
- X,Y,Z postions of vertices
- U,V texture coordinates*
- R,G,B colors and ALPHA channel

*more complex OurShader class which is used for rendering game objects needs texture coordinates.

Hard-coded geometry coordinates for a "cube" (the main building block for an obstacle) and "tunnel" objects are located in /Data/ folder.
Geomerty coordinates for the ball are generated in the /GameObjects/Ball class via transition to spherical coordinates. 
One can change radius of the ball and amount of "sclices" (which is in fact resolutuion).

Some basic movement like translation and rotation is applied by using transformation matrix.







