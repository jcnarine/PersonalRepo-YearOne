#include "Game.h"

#include <random>


Game::~Game()
{
	//If window isn't equal to nullptr
	if (m_window != nullptr)
	{
		//Delete window
		delete m_window;
		//set window to nullptr
		m_window = nullptr;
	}

	//Goes through the scenes and deletes them
	for (unsigned i = 0; i < m_scenes.size(); i++)
	{
		if (m_scenes[i] != nullptr)
		{
			delete m_scenes[i];
			m_scenes[i] = nullptr;
		}
	}
}

void Game::InitGame()
{
	//Scene names and clear colors
	m_name = "Hello World";
	m_clearColor = vec4(0.15f, 0.33f, 0.58f, 1.f);

	//Initializes the backend
	BackEnd::InitBackEnd(m_name);

	//Grabs the initialized window
	m_window = BackEnd::GetWindow();

	//Creates a new test scene
	m_scenes.push_back(new TestScene("Test Scene")); 

	//sets active scene reference to the the test scene 
	m_activeScene = m_scenes[0]; 

	//initiliazes the scene 
	m_activeScene->InitScene(float(BackEnd::GetWindowWidth()), float(BackEnd::GetWindowHeight())); 

	//Sets m_register to point to the register in the active scene 
	m_register = m_activeScene->GetScene(); 

	EffectManager::CreateEffect(Vignette, BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());; 
}

bool Game::Run()
{
	//While window is still open
	while (m_window->isOpen())
	{
		//Clear window with clearColor
		m_window->Clear(m_clearColor);
		
		//Updates the game
		Update();
		//Draws the game
		BackEnd::Draw(m_register);

		//Draws ImGUI
		if (m_guiActive)
			GUI();
		
		//Flips the windows
		m_window->Flip();
		
		//Polls events and then checks them
		BackEnd::PollEvents(m_register, &m_close, &m_motion, &m_click, &m_wheel);
		CheckEvents();

		//does the window have keyboard focus?
		if (Input::m_windowFocus)
		{
			//Accept all input
			AcceptInput();
		}
	}

	return true;
}

void Game::Update()
{
	//Update timer
	Timer::Update();
	//Update the backend
	BackEnd::Update(m_register);

	//display the effects 
	VignetteEffect* tempVig = (VignetteEffect*)EffectManager::GetEffect(EffectManager::GetVignetteHandle()); 
	tempVig->SetInnerRadius(0.01f); 
	tempVig->SetOuterRadius(0.6f); 
	tempVig->SetOpacity(1.f); 

	//moves the player
	mainPlayerMove();


	//grab a reference to the main player's animations
	auto &animController = ECS::GetComponent<AnimationController>(EntityIdentifier::MainPlayer());
	//WEAPON SWITCHING 

	//switches the animation for which weapon is currently running 
	if (weaponSwitch && (animController.GetActiveAnim() == 0 || animController.GetActiveAnim() == 1)) {
		//if it's on the wrench 
		if (animController.GetActiveAnim() == 0) {
			//switch to the gun 
			animController.SetActiveAnim(1); 
			currentWeapon = 1;
		}
		//otherwise it must be on the gun 
		else {
			//so switch to the wrench 
			animController.SetActiveAnim(0);
			currentWeapon = 0;
		}
		//set the switching flag, and flag to allow switch again to false 
		weaponSwitch = false; 
		allowSwitchAgain = false; 
	}

	//update the time since last switch up to 0.2 seconds, then reset it and allow the weapon to be switched again 
	if (allowSwitchAgain == false && timeSinceLastSwitch < 0.2f) {
		timeSinceLastSwitch += Timer::deltaTime; 
	}
	else if (allowSwitchAgain == false && timeSinceLastSwitch >= 0.2f) {
		allowSwitchAgain = true; 
		timeSinceLastSwitch = 0; 
	}

	//WEAPON ATTACKING

	//if the player has inputed an attack 
	if (attack) {
		//check if there's a controller connected
		bool controllerConnected = false;
		int controllerIndex = 0; 
		for (int i = 0; i < 3; i++) {
			if (XInputManager::ControllerConnected(i))
			{
				//if there is save it's index 
				controllerConnected = true;
				controllerIndex = i; 
			}
		}

		//if there is a controller connected
		if (controllerConnected) {
			//grab a reference to it 
			XInputController* tempCon;
			tempCon = XInputManager::GetController(controllerIndex);
			//and make it begin rumbling 
			tempCon->SetRumble(controllerIndex, 20000); 
		}

		//set the input flag to false
		attack = false; 
		//and the in progress flag to true 
		currentlyAttacking = true; 
		//if it's the wrench animation
		if (animController.GetActiveAnim() == 0) {
			//begin swinging wrench
			animController.SetActiveAnim(4); 
		}
		//if it's the gun animation
		else if (animController.GetActiveAnim() == 1) {
			//begin shooting gun 
			animController.SetActiveAnim(5); 
		}
	}

	//if the attack in progress flag is true, but the animation has ended
	if (currentlyAttacking && animController.GetAnimation(animController.GetActiveAnim()).GetAnimationDone()) {
		//check if there's a controller connected
		bool controllerConnected = false;
		int controllerIndex = 0;
		for (int i = 0; i < 3; i++) {
			if (XInputManager::ControllerConnected(i))
			{
				//if there is save it's index 
				controllerConnected = true;
				controllerIndex = i;
			}
		}

		//if there is a controller connected
		if (controllerConnected) {
			//grab a reference to it 
			XInputController* tempCon;
			tempCon = XInputManager::GetController(controllerIndex);
			//and make it stop rumbling 
			tempCon->SetRumble(controllerIndex, 0);
			tempCon->SetIsRumbling(false); 
		}

		animController.GetAnimation(animController.GetActiveAnim()).Reset(); 
		//set the flag to false
		currentlyAttacking = false;
		attack = false;
		//and reset to the animations for just holding the weapon 
		if (animController.GetActiveAnim() == 4) {
			animController.SetActiveAnim(0);
		}
		else {
			animController.SetActiveAnim(1);
		}
	}

	//SPECIAL ABILITIES

	//if the player has inputed a lightning attack 
	if (lightning) {
		//check if there's a controller connected
		bool controllerConnected = false;
		int controllerIndex = 0;
		for (int i = 0; i < 3; i++) {
			if (XInputManager::ControllerConnected(i))
			{
				//if there is save it's index 
				controllerConnected = true;
				controllerIndex = i;
			}
		}

		//if there is a controller connected
		if (controllerConnected) {
			//grab a reference to it 
			XInputController* tempCon;
			tempCon = XInputManager::GetController(controllerIndex);
			//and make it begin rumbling 
			tempCon->SetRumble(controllerIndex, 20000);
		}

		//set the input flag to false
		lightning = false;
		//and the in progress flag to true 
		currentlyLightning = true;
		//if it's the wrench animation
		if (animController.GetActiveAnim() == 0) {
			//begin shooting lighning 
			animController.SetActiveAnim(2);
		}
		//if it's the gun animation
		else if (animController.GetActiveAnim() == 1) {
			//begin shooting lighning 
			animController.SetActiveAnim(3);
		}
	}

	//if the lightning in progress flag is true, but the animation has ended
	if (currentlyLightning && animController.GetAnimation(animController.GetActiveAnim()).GetAnimationDone()) {
		//check if there's a controller connected
		bool controllerConnected = false;
		int controllerIndex = 0;
		for (int i = 0; i < 3; i++) {
			if (XInputManager::ControllerConnected(i))
			{
				//if there is save it's index 
				controllerConnected = true;
				controllerIndex = i;
			}
		}

		//if there is a controller connected
		if (controllerConnected) {
			//grab a reference to it 
			XInputController* tempCon;
			tempCon = XInputManager::GetController(controllerIndex);
			//and make it stop rumbling 
			tempCon->SetRumble(controllerIndex, 0);
			tempCon->SetIsRumbling(false);
		}

		animController.GetAnimation(animController.GetActiveAnim()).Reset();
		//set the flag to false
		currentlyLightning = false;
		attack = false;
		//and reset to the animations for just holding the weapon 
		if (animController.GetActiveAnim() == 2) {
			animController.SetActiveAnim(0);
		}
		else {
			animController.SetActiveAnim(1);
		}
	}
}

void Game::GUI()
{
	UI::Start(BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());

	ImGui::Text("Place your different tabs below.");

	if (ImGui::BeginTabBar(""))
	{
		BackEnd::GUI(m_register, m_activeScene);

		ImGui::EndTabBar();
	}

	UI::End();
}

void Game::CheckEvents()
{
	if (m_close)
		m_window->Close();

	if (m_motion)
		MouseMotion(BackEnd::GetMotionEvent());

	if (m_click)
		MouseClick(BackEnd::GetClickEvent());

	if (m_wheel)
		MouseWheel(BackEnd::GetWheelEvent());
}

void Game::AcceptInput()
{
	XInputManager::Update(); 

	//Just calls all the other input functions 
	GamepadInput(); 

	KeyboardHold();
	KeyboardDown();
	KeyboardUp();

	//Resets the key flags
	//Must be done once per frame for input to work
	Input::ResetKeys();
}

void Game::GamepadInput()
{
	XInputController* tempCon;
	//Gamepad button stroked (pressed)
	for (int i = 0; i < 3; i++) {
		if (XInputManager::ControllerConnected(i))
		{
			tempCon = XInputManager::GetController(i); 
			tempCon->SetStickDeadZone(0.1f); 

			//If the controller is connected, we run the different input types 
			GamepadStroke(tempCon); 
			GamepadUp(tempCon); 
			GamepadDown(tempCon); 
			GamepadStick(tempCon); 
			GamepadTrigger(tempCon); 
		}
	}
}

void Game::GamepadStroke(XInputController * con)
{
	//Gamepad button button stroke (pressed)

	//right bumper changes between weapons
	if (con->IsButtonPressed(Buttons::RB) && allowSwitchAgain) {
		//switch weapons
		weaponSwitch = true;
	}
	//left bumper changes between special abilities 
	if (con->IsButtonPressed(Buttons::LB)) {
		//switch special abilites 
	}
}

void Game::GamepadUp(XInputController * con)
{
	//Gamepad button up 
}

void Game::GamepadDown(XInputController * con)
{
	//Gamepad button down 


}

void Game::GamepadStick(XInputController * con)
{
	//Gamepad stick stuffs
	Stick sticks[2];
	con->GetSticks(sticks);

	//Left stick, movement 
	//move left
	if (sticks[0].x < -0.1f) {
		directionx = 1;
	}
	//move right
	else if (sticks[0].x > 0.1f) {
		directionx = 2;
	}
	//move down
	if (sticks[0].y < -0.1f) {
		directiony = 1;
	}
	//move up
	else if (sticks[0].y > 0.1f) {
		directiony = 2;
	}

	//If there's no input, slow down until we stop moving
	if (sticks[0].x > -0.1f && sticks[0].x < 0.1f) {
		directionx = 0; 
	}
	if (sticks[0].y > -0.1f && sticks[0].y < 0.1f) {
		directiony = 0;
	}

	//Right stick, rotation
	if (sticks[1].x < -0.1f) {
		//rotate left
	}
	else if (sticks[1].x > 0.1f) {
		//rotate right 
	}
	else if (sticks[1].y < -0.1f) {
		//rotate "up"
	}
	else if (sticks[1].y > 0.1f) {
		//rotate "down" 
	}
}

void Game::GamepadTrigger(XInputController * con)
{
	//Gamepad trigger stuff
	Triggers triggers;
	con->GetTriggers(triggers);

	if (triggers.RT > 0.5f && !attack && !currentlyAttacking) {
		//shoot gun or swing wrench 
		attack = true; 
	}
	if (triggers.LT > 0.5f) {
		//fire off special ability 
		lightning = true; 
	}
}

void Game::KeyboardHold()
{
	//Keyboard button held

	//move up 
	if (Input::GetKey(Key::W) || Input::GetKey(Key::UpArrow))
	{
		directiony = 2;
	}
	//move down
	else if (Input::GetKey(Key::S) || Input::GetKey(Key::DownArrow))
	{
		directiony = 1;
	}
	//move left 
	if (Input::GetKey(Key::A) || Input::GetKey(Key::LeftArrow))
	{
		directionx = 1;
	}
	//move right 
	else if (Input::GetKey(Key::D) || Input::GetKey(Key::RightArrow))
	{
		directionx = 2;
	}

	//check if there's a controller connected
	bool controllerConnected = false; 
	for (int i = 0; i < 3; i++) {
		if (XInputManager::ControllerConnected(i))
		{
			controllerConnected = true; 
		}
	}

	//If there isn't, begin slowing down if there's no keyboard input (if there is a controller, the controller code does this for us already)
	if (controllerConnected == false) {
		if (!Input::GetKey(Key::D) && !Input::GetKey(Key::A) && !Input::GetKey(Key::LeftArrow) && !Input::GetKey(Key::RightArrow)) {
			directionx = 0;
		}

		if (!Input::GetKey(Key::W) && !Input::GetKey(Key::S) && !Input::GetKey(Key::UpArrow) && !Input::GetKey(Key::DownArrow)) {
			directiony = 0;
		}
	}
}

void Game::KeyboardDown()
{
	//Keyboard button down
	if (Input::GetKeyDown(Key::One))
	{
		//make the active special lightning 
	}
	if (Input::GetKeyDown(Key::Two)) {
		//make the active special fire
	}

}

void Game::KeyboardUp()
{
	//ImGui stuff 
	if (Input::GetKeyUp(Key::F1))
	{
		if (!UI::m_isInit)
		{
			UI::InitImGUI();
		}
		m_guiActive = !m_guiActive;
	}
}

void Game::MouseMotion(SDL_MouseMotionEvent evnt)
{
	//Rotate the player to match the position of the mouse 

	if (m_guiActive)
	{
		ImGui::GetIO().MousePos = ImVec2(float(evnt.x), float(evnt.y));

		if (!ImGui::GetIO().WantCaptureMouse)
		{

		}
	}

	//Resets the enabled flag
	m_motion = false;
}

void Game::MouseClick(SDL_MouseButtonEvent evnt)
{

	//left click fire gun or swing wrench 
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && !attack && !currentlyAttacking) {
		//fire gun / swing wrench 
		attack = true; 
	}
	//right click fire off the special ability
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
		//fire off special ability 
		lightning = true; 
	}

	if (m_guiActive)
	{
		ImGui::GetIO().MousePos = ImVec2(float(evnt.x), float(evnt.y));
		ImGui::GetIO().MouseDown[0] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT));
		ImGui::GetIO().MouseDown[1] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT));
		ImGui::GetIO().MouseDown[2] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE));
	}

	//Resets the enabled flag
	m_click = false;
}

void Game::MouseWheel(SDL_MouseWheelEvent evnt)
{
	if (evnt.y != 0 && allowSwitchAgain) {
		//Set the main weapon to change
		weaponSwitch = true; 
	}

	if (m_guiActive)
	{
		ImGui::GetIO().MouseWheel = float(evnt.y);
	}
	//Resets the enabled flag
	m_wheel = false;
}

void Game::mainPlayerMove()
{
	
	vec3 position = m_register->get<Transform>(EntityIdentifier::MainPlayer()).GetPosition();

	//calculates accleration 
	vec2 acceleration = vec2(0.f, 0.f);

	//Left stick, movement 
	if (directionx == 1) {
		acceleration.x = -20.f;
	}
	else if (directionx == 2) {
		acceleration.x = 20.f;
	}
	//stop moving left/right
	else {
		acceleration.x = 0;
		if (m_velocity.x > 0 && m_velocity.x >= 0.8f) {
			m_velocity.x -= 0.8f;
		}
		else if (m_velocity.x < 0 && m_velocity.x <= -0.8f) {
			m_velocity.x += 0.8f;
		}
		else {
			m_velocity.x = 0;
		}
	}

	if (directiony == 1) {
		acceleration.y = -20.f;
	}
	else if (directiony == 2) {
		acceleration.y = 20.f;
	}
	//stop moving up/down
	else {
		acceleration.y = 0;
		if (m_velocity.y > 0 && m_velocity.y >= 0.8f) {
			m_velocity.y -= 0.8f;
		}
		else if (m_velocity.y < 0 && m_velocity.y <= -0.8f) {
			m_velocity.y += 0.8f;
		}
		else {
			m_velocity.y = 0;
		}
	}

	//Update velocity if not over max 
	//x-axis 
	if (m_velocity.x < -31.f)
	{
		m_velocity.x = -30.8f;
	}
	else if (m_velocity.x > 31.f)
	{
		m_velocity.x = 30.8f;
	}
	else {
		m_velocity.x = m_velocity.x + (acceleration.x * Timer::deltaTime);
	}
	//y-axis 
	if (m_velocity.y < -31.f)
	{
		m_velocity.y = -30.8f;
	}
	else if (m_velocity.y > 31.f)
	{
		m_velocity.y = 30.8f;
	}
	else {
		m_velocity.y = m_velocity.y + (acceleration.y * Timer::deltaTime);
	}

	//Updates position 
	position = position + (vec3(m_velocity.x, m_velocity.y, 0.f) * Timer::deltaTime) +
		(vec3(acceleration.x, acceleration.y, 0.f) * (0.5f) * (Timer::deltaTime * Timer::deltaTime));

	//Sets updated position 
	m_register->get<Transform>(EntityIdentifier::MainPlayer()).SetPosition(position);
}
