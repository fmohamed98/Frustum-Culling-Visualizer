#pragma once

//event class for keyboard 
class CEvents
{
public:
	enum EEventType
	{
		Press,
		Release,
		Invalid
	};

	CEvents();
	CEvents(const EEventType type, const unsigned char key);
	bool IsPress() const;
	bool IsRelease() const;
	bool IsValid() const;
	unsigned char GetKeyCode() const;

private:
	EEventType m_type;
	unsigned char m_key;
};
