#include "TestScene.h"

TestScene::TestScene(std::string name)
	: Scene(name)
{
}

void TestScene::InitScene(float windowWidth, float windowHeight)
{
	//dynamically allocate register, so it can be unloaded when we switch between scenes 
	m_sceneReg = new entt::registry;

	//Attach the register 
	ECS::AttachRegister(m_sceneReg); 

	//Set up aspect ratio 
	float aspectRatio = windowWidth / windowHeight; 

	//setup main camera 
	{
		//create the camera 
		auto entity = ECS::CreateEntity(); 
		EntityIdentifier::MainCamera(entity); 

		//Creates new orthographic camera 
		ECS::AttachComponent<Camera>(entity); 
		//attachs horizontal and vertical scrolling components 
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);

		//sets up camera
		vec4 temp = ECS::GetComponent<Camera>(entity).GetOrthoSize(); 
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Sets up scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<HorizontalScroll>(entity).SetOffSet(1.f); 
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetOffSet(1.f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::CameraBit() | EntityIdentifier::HoriScrollCameraBit() | EntityIdentifier::VertScrollCameraBit(); 
		ECS::SetUpIdentifier(entity, bitHolder, "Main Camera"); 
		ECS::SetIsMainCamera(entity, true); 
	}

	//steup new entity, mainplayer
	{
		//Our animation file 
		auto animations = File::LoadJSON("JackAnimations.json"); 

		//create new entity 
		auto entity = ECS::CreateEntity(); 
		EntityIdentifier::MainPlayer(entity); 
		
		//Add components
		ECS::AttachComponent<Sprite>(entity); 
		ECS::AttachComponent<Transform>(entity); 
		ECS::AttachComponent<AnimationController>(entity); 

		//Sets up components
		std::string fileName = "jack spritesheet.png"; 
		auto &animController = ECS::GetComponent<AnimationController>(entity); 
		animController.InitUVs(fileName);
		//Adds walking with wrench animation
		animController.AddAnimation(animations["WrenchBase"]); //index 0
		//Adds walking with gun animation
		animController.AddAnimation(animations["GunWalk"]); //index 1
		//Adds attacking power with wrench animation 
		animController.AddAnimation(animations["WrenchPower"]); //index 2 
		//Adds attacking power with gun animation 
		animController.AddAnimation(animations["GunPower"]); //index 3
		//Adds attack with wrench 
		animController.AddAnimation(animations["WrenchSwing"]); //index 4
		//Adds firing the gun 
		animController.AddAnimation(animations["GunFire"]); //index 5

		animController.SetActiveAnim(0); 

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 48, 48, true, &animController); 
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 99.f));

		//Sets up identifier 
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Jack"); 
		ECS::SetIsMainPlayer(entity, true); 
	}

	//setup a background image entity, just to test movement 
	{
		//creates entity
		auto entity = ECS::CreateEntity(); 

		//Adds components
		ECS::AttachComponent<Sprite>(entity); 
		ECS::AttachComponent<Transform>(entity);

		//Sets up components 
		std::string fileName = "HelloWorld.png"; 
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 50); 

		//Sets up identitifer
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Hello World Sign"); 
	}

	//Makes the camera focus on the main player
	//We do this at the very button to get the most accurate pointer to our transform
	ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
}
