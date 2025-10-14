#include "Events.h"

CEvents::CEvents()
	:
	m_type(EEventType::Invalid),
	m_key(0u)
{
}

CEvents::CEvents(const EEventType type, const unsigned char key)
	:
	m_type(type),
	m_key(key)
{
}

bool CEvents::IsPress() const
{
	return m_type == EEventType::Press;
}

bool CEvents::IsRelease() const
{
	return m_type == EEventType::Release;
}

bool CEvents::IsValid() const
{
	return m_type != EEventType::Invalid;
}

unsigned char CEvents::GetKeyCode() const
{
	return m_key;
}