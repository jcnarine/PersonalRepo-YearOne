#pragma once

#include "Scene.h"

class TestScene : public Scene
{
public:
	TestScene(std::string name); 

	void InitScene(float windowWidth, float windowHeight) override; 
};