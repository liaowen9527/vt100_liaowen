#pragma once

class terminal;
class conf;
class termline;
class termchar;

class control
{
public:
	control();
	~control();

	void set_term(terminal* term);

public:
	void cr();
	void lf();
	void tab();
	void backspace();

	void display_graphic_char(termchar* pchr);
	int copy_termchar(termline* line, int col, termchar* pchr);
	void erase_lots(bool line_only, bool from_begin, bool to_end);

	void clear_tabs();
	void set_scroll_margins();

public:
	void move_cursor(int row, int col, int marg_clip);

	void save_cursor();
	void restore_cursor();

public:
	void toggle_modes(bool state);
	void toggle_mode(int mode, int query, bool state);

	void set_graphics_rendition();
	void set_pagesize();

public:
	void into_esc();
	void into_csi();
	void into_osc();
	void into_osc_p();

protected:
	conf* get_conf();

protected:
	terminal* m_term;
};