#pragma once

class conf
{
public:
	conf();
	~conf();

public:
	bool is_vt52mode();
	void set_vt52mode(bool val);

	bool is_cursor_on();
	void set_cursor_on(bool val);

public:
	bool is_crhaslf();
	bool is_lfhascr();
	bool is_no_alt_screen();
	bool is_erase_to_scrollback();

protected:
	bool m_cursor_on;
	bool m_use_bce;	//use back color to erase

	bool m_vt52_mode;

	bool m_crhaslf;	
	bool m_lfhascr;

	bool m_no_alt_screen;
	bool m_erase_to_scrollback;
};