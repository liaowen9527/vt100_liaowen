#include "terminal.h"

terminal::terminal()
{
	m_compatibility_level = VT100;
	m_alt_which = 1;
	
	m_ctrl.set_term(this);
	m_parser.set_term(this);	
	resize(24, 80);
}

terminal::~terminal()
{

}

bool terminal::check_compat(int vttype)
{
	return false;
}

bool terminal::is_compat(int vttype)
{
	return true;
}

int terminal::get_compat()
{
	return m_compatibility_level;
}

void terminal::set_compat(int vttype)
{
	m_compatibility_level = vttype;
}

parser* terminal::get_parser()
{
	return &m_parser;
}

control* terminal::get_control()
{
	return &m_ctrl;
}

conf* terminal::get_conf()
{
	return &m_conf;
}

argument* terminal::get_argument()
{
	return get_parser()->get_argument();
}

screen_lines* terminal::get_sclines()
{
	return get_screen()->get_lines();
}

scrollback* terminal::get_sb()
{
	return &m_sb;
}

screen* terminal::get_screen()
{
	return m_alt_which == 0 ? &m_screens[1] : &m_screens[0];
}

bool terminal::had_alt_screen()
{
	return m_alt_which == 0;
}

int terminal::get_rows()
{
	return m_rows;
}

int terminal::get_cols()
{
	return m_cols;
}

int terminal::get_charset(int index)
{
	return 0;
}

void terminal::set_charset(int index, int charset)
{

}

termchar_ptr terminal::get_last_graphic_char()
{
	return m_last_graphic_char;
}

void terminal::set_last_graphic_char(termchar* chr)
{
	if (nullptr == m_last_graphic_char)
	{
		m_last_graphic_char = std::make_shared<termchar>();
	}

	*m_last_graphic_char = *chr;
}

termchar* terminal::get_erase_char()
{
	return &m_erase_char;
}

bool terminal::find_next_tab(int from_col, int& end_col)
{
	end_col = from_col;
	int cols = get_cols();

	//the last of col can not be tab
	do
	{
		end_col++;
		if (m_tabs[end_col])
		{
			return true;
		}
	} while (end_col < cols - 1);

	return false;
}

void terminal::set_a_tab(int col, bool val)
{
	m_tabs[col] = val;
}

bool terminal::is_support_utf()
{
	return get_screen()->get_utf();
}

void terminal::support_utf(bool val)
{
	get_screen()->set_utf(val);
}

void terminal::resize(int rows, int cols)
{
	m_rows = rows;
	m_cols = cols;
	
	m_screens.resize(2);
	for (int i = 0; i < m_screens.size(); ++i)
	{
		screen* sc = &m_screens[i];
		sc->get_lines()->resize(rows, cols);
	}
}

void terminal::swap_screen(int witch, bool reset, bool keep_cur_pos)
{
	if (witch == m_alt_which)
	{
		return;
	}

	m_alt_which = witch;
	if (witch == 0)
	{
		reset = false;
	}

	
}

void terminal::preline()
{
	screen* sc = get_screen();
	cursor curs = sc->get_cursor();
	margin marg = sc->get_margin();
	if (curs.row == marg.top)
	{
		scroll(marg, -1, true);
		return;
	}

	if (curs.row > 0)
	{
		sc->set_curs_row(curs.row - 1);
	}
}

void terminal::nextline()
{
	screen* sc = get_screen();
	cursor curs = sc->get_cursor();
	margin marg = sc->get_margin();
	if (curs.row == marg.bottom)
	{
		scroll(marg, 1, true);
		return;
	}

	if (curs.row < get_rows() - 1)
	{
		sc->set_curs_row(curs.row + 1);
	}
}

void terminal::inset_lines(int nlines)
{
	screen* sc = get_screen();
	cursor curs = sc->get_cursor();
	margin marg = sc->get_margin();
	if (curs.row <= marg.bottom)
	{
		margin new_marg = marg;
		new_marg.top = curs.row;
		scroll(new_marg, -nlines, false);
	}
}

void terminal::delete_lines(int nlines)
{
	screen* sc = get_screen();
	cursor curs = sc->get_cursor();
	margin marg = sc->get_margin();
	if (curs.row <= marg.bottom)
	{
		margin new_marg = marg;
		new_marg.top = curs.row;
		scroll(new_marg, nlines, true);
	}
}

void terminal::scroll(margin marg, int lines, bool sb)
{
	if (lines < 0)
	{
		return scroll_up(marg, -lines);
	}

	//if magin.top is not 0, we not need to scroll
	//if alt screen, can not save lines to scroll back
	if (marg.top != 0 || had_alt_screen())
	{
		sb = false;
	}

	//scroll down
	screen* sc = get_screen();
	screen_lines* sclines = sc->get_lines();

	int count = marg.bottom - marg.top;
	lines = std::min<int>(lines, count + 1);

	while (lines-- > 0)
	{
		termline_ptr line = sclines->pop_front();
		if (sb)
		{
			m_sb.push_back(line);
		}
	}
}

void terminal::scroll_up(margin marg, int lines)
{
	screen* sc = get_screen();
	screen_lines* sclines = sc->get_lines();

	int count = marg.bottom - marg.top;
	lines = std::min<int>(lines, count + 1);

	while (lines-- > 0)
	{
		sclines->pop_back();
	}
}

void terminal::insch(int n)
{

}

