#include "control.h"
#include <assert.h>
#include "../conf.h"
#include "../terminal.h"

control::control()
{

}

control::~control()
{

}

void control::set_term(terminal* term)
{
	m_term = term;
}

void control::cr()
{
	screen* sc = m_term->get_screen();
	sc->set_curs_col(0);
	sc->set_wrap_next(false);

	if (get_conf()->is_crhaslf())
	{
		m_term->nextline();
	}
}

void control::lf()
{
	m_term->nextline();

	screen* sc = m_term->get_screen();
	if (get_conf()->is_lfhascr())
	{
		sc->set_curs_col(0);
	}

	sc->set_wrap_next(false);
}

void control::tab()
{
	screen* sc = m_term->get_screen();
	screen_lines* sclines = sc->get_lines();
	cursor old_curs = sc->get_cursor();
	termline_ptr ldata = sclines->get_line(old_curs.row);

	int col_tab = 0;
	m_term->find_next_tab(old_curs.col, col_tab);

	int new_col = col_tab;
	int cols = m_term->get_cols();
	cols = ldata->is_width() ? cols / 2 : cols;
	if (col_tab >= cols)
	{
		new_col = cols - 1;
	}

	sc->set_curs_col(new_col);
}

void control::backspace()
{
	screen* sc = m_term->get_screen();
	cursor curs = sc->get_cursor();
	assert(curs.col >= 0 || curs.row >= 0);

	if (curs.col == 0)
	{
		if (curs.row > 0 && sc->can_wrap())
		{
			curs.row -= 1;
			curs.col = m_term->get_cols() - 1;
			sc->set_cursor(curs);
		}
	}
	else
	{
		if (sc->is_wrap_next())
		{
			sc->set_wrap_next(false);
		}
		else
		{
			sc->set_curs_col(curs.col - 1);
		}
	}
}

void control::display_graphic_char(termchar* pchr)
{
	screen* sc = m_term->get_screen();
	cursor curs = sc->get_cursor();

	termline_ptr line = sc->get_lines()->get_line(curs.row);
	curs.col = copy_termchar(line.get(), curs.col, pchr);

	sc->set_cursor(curs);
}

int control::copy_termchar(termline* line, int col, termchar* pchr)
{
	int col_temp = col;
	termchar_ptr chr = line->get_char(col_temp);
	chr->set_char(pchr->get_char());
	col_temp++;

	if (pchr->is_wchar())
	{
		chr = line->get_char(col_temp);
		chr->set_ucswide(true);
		col_temp++;
	}

	return col_temp;
}

void control::erase_lots(bool line_only, bool from_begin, bool to_end)
{

}

void control::clear_tabs()
{
	argument* arg_list = m_term->get_argument();
	char query = arg_list->get_esc_query();
	if (query == 1) 
	{
		int arg = arg_list->get_arg(0);
		if (arg == 0)
		{
			cursor curs = m_term->get_screen()->get_cursor();
			m_term->set_a_tab(curs.col, false);
		}
		else if (arg == 3)
		{
			for (int i = 0; i < m_term->get_cols(); i++)
			{
				m_term->set_a_tab(i, false);
			}
		}
	}
}

void control::set_scroll_margins()
{
	argument* arg_list = m_term->get_argument();
	char query = arg_list->get_esc_query();
	if (query > 2)
	{
		return;
	}

	int rows = m_term->get_rows();
	int arg_1 = arg_list->get_arg(1, rows, rows);

	margin marg;
	marg.top = arg_list->get_arg(0, m_term->get_rows(), 1) - 1;
	marg.bottom = std::min<int>(arg_1 - 1, rows - 1);
	
	if (marg.bottom - marg.top > 0)
	{
		screen* sc = m_term->get_screen();
		sc->set_margin(marg);
		sc->set_curs_col(0);

		int row = sc->is_dec_om() ? marg.top : 0;
		sc->set_curs_row(row);
	}
}

void control::move_cursor(int row, int col, int marg_clip)
{
	col = std::max<int>(col, 0);
	col = std::min<int>(col, m_term->get_cols() - 1);

	screen* sc = m_term->get_screen();
	if (marg_clip) 
	{
		margin marg = sc->get_margin();
		bool force_clip = (marg_clip == 2);
		if (force_clip || row >= marg.top)
		{
			row = std::max<int>(row, marg.top);
		}
		if (force_clip || row <= marg.bottom)
		{
			row = std::min<int>(row, marg.bottom);
		}
	}
	
	row = std::max<int>(row, 0);
	row = std::min<int>(row, m_term->get_rows() - 1);

	sc->set_curs_row(row);
	sc->set_curs_col(col);
	sc->set_wrap_next(false);
}

void control::save_cursor()
{
	if (m_term->check_compat(VT100))
	{
		m_term->get_screen()->save_cursor();
	}
}

void control::restore_cursor()
{
	if (m_term->check_compat(VT100))
	{
		m_term->get_screen()->restore_cursor();
	}
}

void control::toggle_modes(bool state)
{
	argument* arg_list = m_term->get_argument();
	char query = arg_list->get_esc_query();

	for (int i = 0; i < arg_list->count(); i++)
	{
		int arg = arg_list->get_arg(i);
		toggle_mode(arg, query, state);
	}
}

void control::toggle_mode(int mode, int query, bool state)
{

}

void control::set_graphics_rendition()
{

}

void control::set_pagesize()
{

}

void control::into_esc()
{
	if (m_term->get_conf()->is_vt52mode())
	{
		m_term->get_parser()->set_termstate(VT52_ESC);
	}
	else 
	{
		if (m_term->check_compat(ANSIMIN))
		{
			m_term->get_parser()->set_termstate(SEEN_ESC);
			m_term->get_argument()->set_esc_query(0);
		}
	}
}

void control::into_csi()
{
	m_term->get_parser()->set_termstate(SEEN_CSI);
	m_term->get_argument()->resize(1);
	m_term->get_argument()->set_arg(0, ARG_DEFAULT);
	m_term->get_argument()->set_esc_query(0);
}

void control::into_osc()
{
	/* Compatibility is nasty here, xterm, linux, decterm yuk! */
	if (m_term->check_compat(OTHER))
	{
		m_term->get_parser()->set_termstate(SEEN_OSC);
		m_term->get_argument()->resize(1);
		m_term->get_argument()->set_arg(0, 0);
	}	
}

void control::into_osc_p()
{
	m_term->get_parser()->set_termstate(SEEN_OSC_P);
}

conf* control::get_conf()
{
	return m_term->get_conf();
}
