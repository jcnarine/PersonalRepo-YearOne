#include "HoriztontalScroll.h"

HorizontalScroll::HorizontalScroll()
{
}

void HorizontalScroll::Update()
{
	//above focus (to the right) 
	if (m_focus->GetPosition().x > m_cam->m_localPosition.x + m_offset) {
		//Calculate how far the focused has moved the camera to the right by 
		float difference = (m_focus->GetPosition().x - (m_cam->m_localPosition.x + m_offset)); 

		//adjust the camera
		m_cam->SetPosition(vec3(m_cam->GetPosition().x + difference, m_cam->GetPosition().y, m_cam->GetPosition().z)); 
	}
	//below focus (to the left) 
	if (m_focus->GetPosition().x < m_cam->m_localPosition.x - m_offset) {
		//Calculate how far the focused has moved the camera to the left by 
		float difference = (m_focus->GetPosition().x - (m_cam->m_localPosition.x - m_offset));

		//adjust the camera
		m_cam->SetPosition(vec3(m_cam->GetPosition().x + difference, m_cam->GetPosition().y, m_cam->GetPosition().z));
	}
}

Camera * HorizontalScroll::GetCam() const
{
	return m_cam;
}

Transform * HorizontalScroll::GetFocus() const
{
	return m_focus;
}

float HorizontalScroll::GetOffSet() const
{
	return m_offset;
}

void HorizontalScroll::SetCam(Camera * cam)
{
	m_cam = cam; 
}

void HorizontalScroll::SetFocus(Transform * focus)
{
	m_focus = focus; 
}

void HorizontalScroll::SetOffSet(float offset)
{
	m_offset = offset; 
}
