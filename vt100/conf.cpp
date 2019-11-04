#include "conf.h"

conf::conf()
{
	m_cursor_on = true;
	m_use_bce = true;
	m_vt52_mode = false;

	m_crhaslf = false;
	m_lfhascr = false;
	m_no_alt_screen = false;
	m_erase_to_scrollback = true;
}

conf::~conf()
{

}

bool conf::is_vt52mode()
{
	return m_vt52_mode;
}

void conf::set_vt52mode(bool val)
{
	m_vt52_mode = val;
}

bool conf::is_cursor_on()
{
	return m_cursor_on;
}

void conf::set_cursor_on(bool val)
{
	m_cursor_on = val;
}

bool conf::is_crhaslf()
{
	return m_crhaslf;
}

bool conf::is_lfhascr()
{
	return m_lfhascr;
}

bool conf::is_no_alt_screen()
{
	return m_no_alt_screen;
}

bool conf::is_erase_to_scrollback()
{
	return m_erase_to_scrollback;
}
