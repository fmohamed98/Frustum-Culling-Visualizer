#include "Keyboard.h"

//constructor
CKeyboard::CKeyboard()
{
	for (int i = 0; i < 256; i++)
	{
		m_keyStates[i] = false; //Initialize all key states to off (false)
	}

}

bool CKeyboard::KeyIsPressed(const unsigned char keycode)
{
	return m_keyStates[keycode];
}

bool CKeyboard::KeyBufferIsEmpty()
{
	return m_keyBuffer.empty();
}

bool CKeyboard::CharBufferIsEmpty()
{
	return m_charBuffer.empty();
}

CEvents CKeyboard::ReadKey()
{
	if (m_keyBuffer.empty()) //If no keys to be read
	{
		return CEvents(); //return empty keyboard event
	}
	else
	{
		CEvents e = m_keyBuffer.front(); //Get first Keyboard Event from queue
		m_keyBuffer.pop(); //Remove first item from queue
		return e; //Returns keyboard event
	}
}

unsigned char CKeyboard::ReadChar()
{
	if (m_charBuffer.empty()) //If no keys to be read
	{
		return 0u; //return 0 (NULL char)
	}
	else
	{
		unsigned char e = this->m_charBuffer.front(); //Get first char from queue
		m_charBuffer.pop(); //Remove first char from queue
		return e; //Returns char
	}
}

void CKeyboard::OnKeyPressed(const unsigned char key)
{
	m_keyStates[key] = true;
	m_keyBuffer.push(CEvents(CEvents::EEventType::Press, key));
}

void CKeyboard::OnKeyReleased(const unsigned char key)
{
	m_keyStates[key] = false;
	m_keyBuffer.push(CEvents(CEvents::EEventType::Release, key));
}

void CKeyboard::OnChar(const unsigned char key)
{
	m_charBuffer.push(key);
}

void CKeyboard::EnableAutoRepeatKeys()
{
	m_autoRepeatKeys = true;
}

void CKeyboard::DisableAutoRepeatKeys()
{
	m_autoRepeatKeys = false;
}

void CKeyboard::EnableAutoRepeatChars()
{
	m_autoRepeatChars = true;
}

void CKeyboard::DisableAutoRepeatChars()
{
	m_autoRepeatChars = false;
}

bool CKeyboard::IsKeysAutoRepeat()
{
	return m_autoRepeatKeys;
}

bool CKeyboard::IsCharsAutoRepeat()
{
	return m_autoRepeatChars;
}