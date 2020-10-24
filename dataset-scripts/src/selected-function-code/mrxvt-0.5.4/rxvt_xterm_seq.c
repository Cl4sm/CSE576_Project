void
rxvt_xterm_seq(rxvt_t* r, int page, int op, const char *str, unsigned char resp __attribute__((unused)))
{
    int		color;
    char	*buf, *name;
#if defined(TRANSPARENT) || defined(BACKGROUND_IMAGE)
    int		changed = 0;
# ifdef TINTING_SUPPORT
    int		shade;
# endif
#endif


    assert(NOT_NULL(str));
    switch (op)
    {
	case XTerm_title:   /* Set tab / term title */
	case XTerm_name:    /* also set icon title */
#ifdef SET_TAB_TITLE_ON_XTERM_SEQUENCE
	    rxvt_tabbar_set_title (r, page, (const unsigned char TAINTED*) str);
#ifndef SET_TAB_TITLE_NOT_WIN_TITLE
	    /*
	     * Set both the tab title and win title. However if -stt is used,
	     * then the window title will already be set by
	     * rxvt_tabbar_set_title(), so we only have to set it here if +stt.
	     */
	    if( NOTSET_OPTION(r, Opt2_syncTabTitle ) )
		rxvt_set_term_title(r, (const unsigned char*) str);
#endif
#else
	    rxvt_set_term_title(r, (const unsigned char*) str);
#endif
	    if( op == XTerm_title )
		break;	/* Don't set the icon name */
	    /* else FALL THROUGH */

	case XTerm_iconName:
	    rxvt_set_icon_name(r, (const unsigned char*) str);
	    /*
	     * 2006-05-30 gi1242: -sti means sync the icon name to the tab
	     * title, and NOT the other way around.
	     */
#if 0
	    if (ISSET_OPTION(r, Opt2_syncTabIcon))
		rxvt_tabbar_set_title (r, ATAB(r),
			(const unsigned char TAINTED*) str);
#endif
	    break;

	case XTerm_Color:
	    for (buf = (char *)str; buf && *buf;)
	    {
		if (IS_NULL(name = STRCHR(buf, ';')))
		    break;
		*name++ = '\0';
		color = atoi(buf);
		if (color < 0 || color >= TOTAL_COLORS)
		    break;
		if (NOT_NULL(buf = STRCHR(name, ';')))
		    *buf++ = '\0';
		rxvt_set_window_color(r, page, color + minCOLOR, name);
	    }
	    break;
#ifndef NO_CURSORCOLOR
	case XTerm_Color_cursor:
	    rxvt_set_window_color(r, page, Color_cursor, str);
	    break;
#endif
	case XTerm_Color_pointer:
	    rxvt_set_window_color(r, page, Color_pointer, str);
	    break;
#ifndef NO_BOLD_UNDERLINE_REVERSE
	case XTerm_Color_BD:
	    rxvt_set_window_color(r, page, Color_BD, str);
	    break;
	case XTerm_Color_UL:
	    rxvt_set_window_color(r, page, Color_UL, str);
	    break;
	case XTerm_Color_RV:
	    rxvt_set_window_color(r, page, Color_RV, str);
	    break;
#endif

	case XTerm_Menu:
	    /*
	     * rxvt_menubar_dispatch() violates the constness of the string,
	     * so DON'T do it here.
	     *
	     * 2006-03-13 gi1242: Plus it's a security risk. Only interactive
	     * users should control menus.
	     */
	    break;

#ifdef BACKGROUND_IMAGE
	case XTerm_Pixmap:
	    if (*str != ';')
	    {
		/* reset to default scaling */
		rxvt_scale_pixmap(r, page, "");
		/* change pixmap */
		rxvt_load_bg_pixmap(r, page, str);
		rxvt_scr_touch(r, page, True);
	    }
	    while (NOT_NULL(str = STRCHR(str, ';')))
	    {
		str++;
		changed += rxvt_scale_pixmap(r, page, str);
	    }
	    if (changed)
	    {
		rxvt_resize_pixmap(r, page);
		rxvt_scr_touch(r, page, True);
	    }
	    break;
#endif

	case XTerm_restoreFG:
	    rxvt_set_window_color(r, page, Color_fg, str);
	    break;
	case XTerm_restoreBG:
	    rxvt_set_window_color(r, page, Color_bg, str);
	    break;
	case XTerm_logfile:
	    /* TODO Write code */
	    break;
	case XTerm_font:
	    rxvt_resize_on_font (r, (char*) str);
	    break;

	/* 2006-03-14 gi1242 XXX: Why is this commented again? */
#if 0
	case XTerm_dumpscreen:
	    {
		int	     fd;
		if ((fd=open(str, O_RDWR | O_CREAT | O_EXCL, 0600))>=0)
		{
		    rxvt_scr_dump(r, page, fd);
		    close(fd);
		}
	    }
	    break;
#endif

	case MRxvt_tabterm:	/* Set window and tab title */
	    rxvt_tabbar_set_title (r, page, (const unsigned char TAINTED*) str);
	    if( ISSET_OPTION(r, Opt2_syncTabTitle))
		/*
		 * Window title will automatically be synced, so setting it
		 * again is wasteful.
		 */
		break;
	    /* else FALL THROUGH */

	case MRxvt_term:	/* Set window title */
	    rxvt_set_term_title(r, (const unsigned char*) str);
	    break;


	case MRxvt_tab:
	    rxvt_tabbar_set_title (r, page, (const unsigned char TAINTED*) str);
	    break;

	case MRxvt_tformat:
	{
	    int len = STRLEN(str);

	    if(
		  IS_NULL( PVTS(r, page)->title_format )	||
		  len != STRLEN( PVTS(r, page)->title_format )
	      )
	    {
		rxvt_free( PVTS(r, page)->title_format );
		PVTS(r, page)->title_format = STRDUP(str);
	    }
	    else
		STRCPY( PVTS(r, page)->title_format, str );

	    /* Redraw the tab title. */
	    refresh_tabbar_tab( r, page );

	    break;
	}

	case MRxvt_wformat:
	{
	    int len = STRLEN(str);
	    
	    if(
		 IS_NULL( PVTS(r, page)->winTitleFormat )	||
		 len != STRLEN( PVTS(r,page)->winTitleFormat )
	      )
	    {
		rxvt_free( PVTS(r,page)->winTitleFormat );
		PVTS(r,page)->winTitleFormat = STRDUP(str);
	    }
	    else
		STRCPY( PVTS(r,page)->winTitleFormat, str );

	    if( ISSET_OPTION( r, Opt2_syncTabTitle ) )
		sync_tab_title( r, ATAB(r) );

	    break;
	}


	/*
	 * 2006-02-20 gi1242: These escape sequences are disabled for a possible
	 * security flaw. A malicious user might be able to send a few escape
	 * sequences through a text file, giving him more control than he should
	 * have over the tabs.
	 *
	 * On that note, even the escape sequences for hiding or showing the
	 * tabbar / scroll bar / menubar have been disabled. These aren't really
	 * a security risk, however the user can access these functions through
	 * either keyboard macros, or the popup menu, so these are really just
	 * code bloat :).
	 *
	 * Only the local user should be able to create / close / move tabs.
	 *
	 * NOTE: The disabled code uses the old hotkey code (from mrxvt-0.4.2).
	 * To enable the escape sequences below, you have to uncomment the code
	 * below AND port it to use the macro feature from 0.5.0 upward.
	 */
#if 0 /* {{{ DISABLED FOR SECURITY REASONS */
	case MRxvt_hide:
#ifdef HAVE_SCROLLBARS
	    if ('s' == *str || 'S' == *str) 	    /* show/hide scrollbar */
	    {
		rxvt_hotkey_hide_scrollbar (r, 0);
	    }
	    else 
#endif	/* HAVE_SCROLLBARS */
#ifdef HAVE_MENUBAR
	    if ('m' == *str || 'M' == *str)	    /* show/hide menubar */
	    {
		rxvt_hotkey_hide_menubar (r, 0);
	    }
	    else
#endif	/* HAVE_MENUBAR */
	    {
		rxvt_hotkey_hide_tabbar (r, 0);
	    }
	    break;

	case MRxvt_tabbtn:
	    rxvt_hotkey_hide_button (r, 0);
	    break;

	case MRxvt_saveconfig:
	    rxvt_hotkey_save_config (r, 0);
	    break;

	case MRxvt_newtab:
	    rxvt_append_page (r, 0, str, NULL);
	    break;
	case MRxvt_prevtab:
	    if (0 != page)
		rxvt_activate_page (r, page-1);
	    else if (0 != LTAB(r))
		rxvt_activate_page (r, LTAB(r));
	    break;
	case MRxvt_nexttab:
	    if (page != LTAB(r))
		rxvt_activate_page (r, page+1);
	    else if (0 != LTAB(r))
		rxvt_activate_page (r, 0);
	    break;

	case MRxvt_moveleft:
	    rxvt_tabbar_move_tab (r, 0);
	    break;
	case MRxvt_moveright:
	    rxvt_tabbar_move_tab (r, 1);
	    break;

	case MRxvt_closewin:
	    if( *str )
	    {
		int tabno = atoi(str) - 1;

		if( tabno == -1 )
		{
		    rxvt_hotkey_kill_tab(r, NULL);
		}
		else if( tabno >=0 && tabno <=LTAB(r)
		    && ( NOTSET_OPTION(r, Opt2_protectSecondary) ||
			 PVTS(r, tabno)->current_screen == PRIMARY))
		{
		    rxvt_kill_page (r, tabno);
		}
		break;
	    }

	    rxvt_hotkey_close_window (r, NULL);
	    break;

	case MRxvt_switchtab:
	    {
		int tabno = atoi(str) - 1;
		if( tabno == -1)
		    rxvt_activate_page( r, PTAB(r));
		else if( tabno >= 0 && tabno <= LTAB(r))
		    rxvt_activate_page( r, tabno);

		break;
	    }
#endif /* }}} */

	/*
	 * 2006-03-13 gi1242: The following are not security risks, but are
	 * unnecessary. Only an interactive user needs such control, and he can
	 * have it using macros / menus.
	 */
#if 0 /* {{{ DISABLED because macros are more useful */
	case MRxvt_verybold:
	    /* rxvt_hotkey_verybold (r, 0); */
	    rxvt_toggle_verybold(r);
	    break;

#ifdef TRANSPARENT
	case MRxvt_trans:
	    rxvt_toggle_transparency (r);
	    break;
#endif	/* TRANSPARENT */

#endif /* }}} */

#ifdef MULTICHAR_SET
	case MRxvt_encode:
	    /* We only change encoding method, but not font ;-) */
	    rxvt_set_multichar_encoding (r, str);   
	    break;
#endif	/* MULTICHAR_SET */

	case MRxvt_opacity:
	    if (IS_ATOM(r->h->xa[XA_NET_WM_WINDOW_OPACITY]))
	    {
		int	oldopacity = r->TermWin.opacity;
		int	tmp;

		if ('-' == *str && (char) 0 == *(str+1))
		{
		    /* handle '-' only, remember it's opposite  */
		    tmp = (r->h->rs[Rs_opacityDegree]) ?
			    r->TermWin.opacity_degree : 1;
		}
		else if ('+' == *str && (char) 0 == *(str+1))
		{
		    /* handle '+' only, remember it's opposite  */
		    tmp = (r->h->rs[Rs_opacityDegree]) ?
			    -(r->TermWin.opacity_degree) : -1;
		}
		else
		{
		    /* other cases, remember it's opposite  */
		    tmp = 100 - atoi (str);
		    /* only change opacity when it is valid */
		    if (tmp < 0 || tmp > 100)
			tmp = oldopacity;
		}
		if ('-' == *str || '+' == *str)
		    /* use input as offset */
		    r->TermWin.opacity += tmp;
		else
		    /* use input as absolute value */
		    r->TermWin.opacity = tmp;
		if (r->TermWin.opacity < 0)
		    r->TermWin.opacity = 0;
		if (r->TermWin.opacity > 100)
		    r->TermWin.opacity = 100;
		/* only change opacity when it has changed */
		if (r->TermWin.opacity != oldopacity)
		    rxvt_set_opacity (r);
	    }
	    break;

	case MRxvt_tabfg:
	case MRxvt_tabbg:
	case MRxvt_itabfg:
	case MRxvt_itabbg:
	    rxvt_tabbar_change_color (r, op, str);
	    break;

#if defined(TRANSPARENT) || defined(BACKGROUND_IMAGE)
# ifdef TINTING_SUPPORT
	case MRxvt_tint:
	    if (ISSET_PIXCOLOR (r->h, Color_tint) &&
		r->h->rs[Rs_shade])
		rxvt_set_window_color(r, page, Color_tint, str);
	    break;

	case MRxvt_bgfade:  /* Make bgfade behave like shade */
	case MRxvt_shade:
	    if (!ISSET_PIXCOLOR (r->h, Color_tint) ||
		!r->h->rs[Rs_shade])
		break;

	    if( *str == '+' || *str == '-' )
	    {
		/* Relative shade level */
		shade = (100 - r->TermWin.shade) + atoi(str);
	    }
	    else
		/* Absolute shade level */
		shade = atoi (str);

	    if (shade >=0 && shade <= 100)
	    {
		shade = 100 - shade;	/* reverse it */
		changed = (r->TermWin.shade != shade);
		r->TermWin.shade = shade;
	    }

	    if (changed)
	    {
		/* shade value is changed, need to refresh terminals */
		rxvt_refresh_bg_image (r, page, False);
	    }
	    break;
# endif	/* TINTING_SUPPORT */
#endif	/* TRANSPARENT || BACKGROUND_IMAGE */

	case MRxvt_termenv:
	    PVTS(r, page)->termenv = rxvt_get_termenv ((const char*) str);
	    break;

	default:
	    break;
    }
}