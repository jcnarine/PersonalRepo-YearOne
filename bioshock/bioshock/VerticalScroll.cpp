#include "VerticalScroll.h"

VerticalScroll::VerticalScroll()
{
}

void VerticalScroll::Update()
{
	//above focus 
	if (m_focus->GetPosition().y > m_cam->m_localPosition.y + m_offset) {
		//Calculate how far the focused has moved the camera up by 
		float difference = m_focus->GetPosition().y - (m_cam->m_localPosition.y + m_offset);

		//adjust the camera
		m_cam->SetPosition(vec3(m_cam->GetPosition().x, m_cam->GetPosition().y + difference, m_cam->GetPosition().z));
	}
	//below focus 
	if (m_focus->GetPosition().y < m_cam->m_localPosition.y - m_offset) {
		//Calculate how far the focused has moved the camera down by 
		float difference = m_focus->GetPosition().y - (m_cam->m_localPosition.y - m_offset);

		//adjust the camera
		m_cam->SetPosition(vec3(m_cam->GetPosition().x, m_cam->GetPosition().y + difference, m_cam->GetPosition().z));
	}
}

Camera * VerticalScroll::GetCam() const
{
	return m_cam;
}

Transform * VerticalScroll::GetFocus() const
{
	return m_focus;
}

float VerticalScroll::GetOffSet() const
{
	return m_offset;
}

void VerticalScroll::SetCam(Camera * cam)
{
	m_cam = cam;
}

void VerticalScroll::SetFocus(Transform * focus)
{
	m_focus = focus;
}

void VerticalScroll::SetOffSet(float offset)
{
	m_offset = offset;
}
