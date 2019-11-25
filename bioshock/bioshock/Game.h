#ifndef __GAME_H__
#define __GAME_H__

#include "BackEnd.h"
#include "TestScene.h"
#include "Xinput.h"
#include "EffectManager.h"

//Our main class for running our game
class Game
{
public:
	//Empty constructor
	Game() { };
	//Deconstructor for game
	//*Unloads window
	~Game();

	//Initiaiizes game
	//*Seeds random
	//*Initializes SDL
	//*Creates Window
	//*Initializes GLEW
	//*Create Main Camera Entity
	//*Creates all other entities and adds them to register
	void InitGame();

	//Runs the game
	//*While window is open
	//*Clear window
	//*Update 
	//*Draw
	//*Poll events
	//*Flip window
	//*Accept input
	bool Run();
	
	//Updates the game
	//*Update timer
	//*Update the rendering system
	//*Update the animation system
	void Update();

	//Runs the GUI
	//*Uses ImGUI for this
	void GUI();

	//Check events
	//*Checks the results of the events that have been polled
	void CheckEvents();

	/*Input Functions*/
	void AcceptInput();
	void GamepadInput(); 

	void GamepadStroke(XInputController* con); 
	void GamepadUp(XInputController* con); 
	void GamepadDown(XInputController* con); 
	void GamepadStick(XInputController* con); 
	void GamepadTrigger(XInputController* con);
	void KeyboardHold();
	void KeyboardDown();
	void KeyboardUp();

	//Mouse input
	void MouseMotion(SDL_MouseMotionEvent evnt);
	void MouseClick(SDL_MouseButtonEvent evnt);
	void MouseWheel(SDL_MouseWheelEvent evnt);

	//player actions (needed to support both controller and keyboard 
	void mainPlayerMove(); 
private:
	//The window
	Window *m_window = nullptr;

	//Scene name
	std::string m_name;
	//Clear color for when we clear the window
	vec4 m_clearColor;
	
	//The main register for our ECS
	entt::registry* m_register;

	//Scenes
	Scene* m_activeScene;
	std::vector<Scene*> m_scenes;
	
	//Imgui stuff
	bool m_guiActive = false;

	//Hooks for events
	bool m_close = false;
	bool m_motion = false;
	bool m_click = false;
	bool m_wheel = false;

	//main player velocity 
	vec2 m_velocity = vec2(0.f, 0.f);

	//main player direction
	int directionx = 0; 
	int directiony = 0; 

	//stuff for switching weapons 
	bool weaponSwitch = false; 
	bool allowSwitchAgain = true;
	float timeSinceLastSwitch = 0.f;
	int currentWeapon = 0; 

	//stuff for making attacks
	bool attack = false; 
	bool currentlyAttacking = false; 

	//Stuff for using abilities 
	bool lightning = false; 
	bool currentlyLightning = false; 
};



#endif // !__GAME_H__

