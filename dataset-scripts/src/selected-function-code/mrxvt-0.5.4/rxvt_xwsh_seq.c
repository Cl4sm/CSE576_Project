void
rxvt_xwsh_seq(rxvt_t* r, int op, const char *str)
{
    assert(NOT_NULL(str));
    switch (op)
    {
	case Xwsh_title:
	    rxvt_set_term_title (r, (const unsigned char*) str);
	    break;
	case Xwsh_iconName:
	    rxvt_set_icon_name (r, (const unsigned char*) str);
	    break;
	case Xwsh_textColor:
	    break;
	case Xwsh_pageColor:
	    break;
	case Xwsh_selTextColor:
	    break;
	case Xwsh_selPageColor:
	    break;
	case Xwsh_cursorTextColor:
	    break;
	case Xwsh_cursorPageColor:
	    break;
	case Xwsh_halfIntColor:
	    break;
	case Xwsh_boldIntColor:
	    break;
	case Xwsh_bindStrKeyVal:
	    break;
	case Xwsh_bindStrKeyFunc:
	    break;
    }
}