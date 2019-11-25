#pragma once

#include "Camera.h"

class HorizontalScroll
{
public:
	HorizontalScroll(); 

	//Make sure the camera is following the focus 
	void Update(); 

	//Getters 
	Camera* GetCam() const; 
	Transform* GetFocus() const; 
	float GetOffSet() const; 

	//Setters
	void SetCam(Camera* cam); 
	void SetFocus(Transform* focus); 
	void SetOffSet(float offset); 



private:
	Camera* m_cam = nullptr; 
	Transform* m_focus = nullptr; 

	float m_offset = 5.f; 
};

//Sends horizontal scrolling camera TO json file 
inline void to_json(nlohmann::json& j, const HorizontalScroll& horiScroll) {
	j["Offset"] = horiScroll.GetOffSet(); 
}

//Reads horizontal scrolling camera FROM json file 
inline void from_json(const nlohmann::json& j, HorizontalScroll& horiScroll) {
	horiScroll.SetOffSet(j["Offset"]); 
}
