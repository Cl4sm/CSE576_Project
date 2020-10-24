xterm_seq(int op, const char *str)
{
    int color;
    char *buf, *name;

    assert(str != NULL);
    switch (op) {
    case XTerm_name:
	set_title(str);		/* drop */
    case XTerm_iconName:
	set_iconName(str);
	break;
    case XTerm_title:
	set_title(str);
	break;
    case XTerm_Color:
	for (buf = (char *)str; buf && *buf;) {
	    if ((name = strchr(buf, ';')) == NULL)
		break;
	    *name++ = '\0';
	    color = atoi(buf);
	    if (color < 0 || color >= TOTAL_COLORS)
		break;
	    if ((buf = strchr(name, ';')) != NULL)
		*buf++ = '\0';
	    set_window_color(color + minCOLOR, name);
	}
	break;
#ifndef NO_CURSORCOLOR
    case XTerm_Color_cursor:
	set_window_color(Color_cursor, str);
	break;
#endif
    case XTerm_Color_pointer:
	set_window_color(Color_pointer, str);
	break;
#ifndef NO_BOLD_UNDERLINE
    case XTerm_Color_BD:
	set_window_color(Color_BD, str);
	break;
    case XTerm_Color_UL:
	set_window_color(Color_UL, str);
	break;
#endif

	case XTerm_EtermSeq:
		op = atoi(str);
		if (!(str = strchr(str, ';')))
			break;
		eterm_seq(op, str + 1);
		break;
    case XTerm_Menu:
    /*
     * menubar_dispatch() violates the constness of the string,
     * so DON'T do it here
     */
	break;
    case XTerm_Pixmap:
#if defined(BACKGROUND_IMAGE)
        if( SetBackgroundPixmap((char*)str) )
			request_background_update();
#endif
	break;

    case XTerm_restoreFG:
	set_window_color(Color_fg, str);
	break;
    case XTerm_restoreBG:
	set_window_color(Color_bg, str);
	break;
    case XTerm_logfile:
	break;
    case XTerm_font:
	change_font(0, str);
	break;
    }
}
