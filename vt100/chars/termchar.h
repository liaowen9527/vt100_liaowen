#pragma once
#include <memory>

class termchar
{
public:
	termchar(wchar_t ch = ' ');
	~termchar();

public:
	wchar_t get_char();
	void set_char(wchar_t ch);

	bool is_wchar();

	bool is_ucswide();
	void set_ucswide(bool val);

protected:
	wchar_t m_char;
	bool m_ucs;
};

typedef std::shared_ptr<termchar> termchar_ptr;