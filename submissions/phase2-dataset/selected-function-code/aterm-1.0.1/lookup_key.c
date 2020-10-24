void
lookup_key(XEvent * ev)
{
    static int      numlock_state = 0;

#ifdef DEBUG_CMD
    static int      debug_key = 1;	/* accessible by a debugger only */
#endif
#ifdef GREEK_SUPPORT
    static short    greek_mode = 0;
#endif
    static XComposeStatus compose =
    {NULL, 0};
    static unsigned char kbuf[KBUFSZ];
    int             ctrl, meta, shft, len;
    KeySym          keysym = 0;

/*
 * use Num_Lock to toggle Keypad on/off.  If Num_Lock is off, allow an
 * escape sequence to toggle the Keypad.
 *
 * Always permit `shift' to override the current setting
 */
    shft = (ev->xkey.state & ShiftMask);
    ctrl = (ev->xkey.state & ControlMask);
    meta = (ev->xkey.state & ModXMask);

    if (numlock_state || (ev->xkey.state & ModNumLockMask)) {
 	numlock_state = (ev->xkey.state & ModNumLockMask);	/* numlock toggle */
	PrivMode((!numlock_state), PrivMode_aplKP);
    }
#if defined(USE_XIM) || !defined(NO_XLOCALE)
    len = 0;
    if (!XFilterEvent(ev, *(&ev->xkey.window))) {
        if (Input_Context != NULL) {
            Status          status_return;

            kbuf[0] = '\0';
            len = XmbLookupString(Input_Context, &ev->xkey, kbuf,
                    sizeof(kbuf), &keysym,
                    &status_return);
        } else {
            len = XLookupString(&ev->xkey, kbuf,
                    sizeof(kbuf), &keysym,
                    &compose);
        }
    }
#else                         /* USE_XIM */
    len = XLookupString(&ev->xkey, (char *) kbuf, sizeof(kbuf), &keysym, &compose);
/*
 * have unmapped Latin[2-4] entries -> Latin1
 * good for installations  with correct fonts, but without XLOCAL
 */
    if (!len && (keysym >= 0x0100) && (keysym < 0x0400)) {
	len = 1;
	kbuf[0] = (keysym & 0xFF);
    }
#endif                                /* USE_XIM */

    if (len && (Options & Opt_scrollKeypress))
	TermWin.view_start = 0;

/* for some backwards compatibility */
#if defined (HOTKEY_CTRL) || defined (HOTKEY_META)
# ifdef HOTKEY_CTRL
#  define HOTKEY	ctrl
# else
#  ifdef HOTKEY_META
#   define HOTKEY	meta
#  endif
# endif
    if (HOTKEY) {
	if (keysym == ks_bigfont) {
	    change_font(0, FONT_UP);
	    return;
	} else if (keysym == ks_smallfont) {
	    change_font(0, FONT_DN);
	    return;
	}
    }
# undef HOTKEY
#endif

    if (shft) {
    /* Shift + F1 - F10 generates F11 - F20 */
	if (keysym >= XK_F1 && keysym <= XK_F10) {
	    keysym += (XK_F11 - XK_F1);
	    shft = 0;		/* turn off Shift */
	} else if (!ctrl && !meta && (PrivateModes & PrivMode_ShiftKeys)) {
	    int             lnsppg = TermWin.nrow * 4 / 5;

#ifdef PAGING_CONTEXT_LINES
	    lnsppg = TermWin.nrow - PAGING_CONTEXT_LINES;
#endif

	    switch (keysym) {
	    /* normal XTerm key bindings */
	    case XK_Prior:	/* Shift+Prior = scroll back */
		if (TermWin.saveLines) {
		    scr_page(UP, lnsppg);
		    return;
		}
		break;

	    case XK_Up:		/* Shift+XK_Up = scroll up one line */
		if (TermWin.saveLines) {
		    scr_page(UP, 1);
		    return;
		}
		break;

	    case XK_Down:	/* Shift+XK_Down = scroll down one line */
		if (TermWin.saveLines) {
		    scr_page(DN, 1);
		    return;
		}
		break;

	    case XK_Next:	/* Shift+Next = scroll forward */
		if (TermWin.saveLines) {
		    scr_page(DN, lnsppg);
		    return;
		}
		break;

	    case XK_Insert:	/* Shift+Insert = paste mouse selection */
		selection_request(ev->xkey.time, ev->xkey.x, ev->xkey.y);
		return;
		break;

	    /* rxvt extras */
	    case XK_KP_Add:	/* Shift+KP_Add = bigger font */
		change_font(0, FONT_UP);
		return;
		break;

	    case XK_KP_Subtract:	/* Shift+KP_Subtract = smaller font */
		change_font(0, FONT_DN);
		return;
		break;
	    }
	}
    }
#ifdef UNSHIFTED_SCROLLKEYS
    else if (!ctrl && !meta) {
	switch (keysym) {
	case XK_Prior:
	    if (TermWin.saveLines) {
		scr_page(UP, TermWin.nrow * 4 / 5);
		return;
	    }
	    break;

	case XK_Next:
	    if (TermWin.saveLines) {
		scr_page(DN, TermWin.nrow * 4 / 5);
		return;
	    }
	    break;
	}
    }
#endif

    switch (keysym) {
    case XK_Print:
#ifdef PRINTPIPE
	scr_printscreen(ctrl | shft);
	return;
#endif
	break;

    case XK_Mode_switch:
#ifdef GREEK_SUPPORT
	greek_mode = !greek_mode;
	if (greek_mode) {
	    xterm_seq(XTerm_title, (greek_getmode() == GREEK_ELOT928 ?
				    "[Greek: iso]" : "[Greek: ibm]"));
	    greek_reset();
	} else
	    xterm_seq(XTerm_title, APL_NAME "-" VERSION);
	return;
#endif
	break;
    }

    if (keysym >= 0xFF00 && keysym <= 0xFFFF) {
#ifdef KEYSYM_RESOURCE
	if (!(shft | ctrl) && KeySym_map[keysym - 0xFF00] != NULL) {
	    const unsigned char *kbuf;
	    unsigned int    len;

	    kbuf = (KeySym_map[keysym - 0xFF00]);
	    len = *kbuf++;

	/* escape prefix */
	    if (meta
# ifdef META8_OPTION
		&& (meta_char == 033)
# endif
		) {
		const unsigned char ch = '\033';

		tt_write(&ch, 1);
	    }
	    tt_write(kbuf, len);
	    return;
	} else
#endif
	    switch (keysym) {
#ifndef NO_BACKSPACE_KEY
	    case XK_BackSpace:
		if (PrivateModes & PrivMode_HaveBackSpace) {
		    len = 1;
		    kbuf[0] = (((PrivateModes & PrivMode_BackSpace) ?
				!(shft | ctrl) : (shft | ctrl)) ? '\b' : '\177');
		} else
		    len = strlen(STRCPY(kbuf, rs_backspace_key));
		break;
#endif
#ifndef NO_DELETE_KEY
	    case XK_Delete:
		len = strlen(STRCPY(kbuf, rs_delete_key));
		break;
#endif
	    case XK_Tab:
		if (shft) {
		    len = 3;
		    STRCPY(kbuf, "\033[Z");
		}
		break;

#ifdef XK_KP_Home
	    case XK_KP_Home:
	    /* allow shift to override */
		if ((PrivateModes & PrivMode_aplKP) ? !shft : shft) {
		    len = 3;
		    STRCPY(kbuf, "\033Ow");
		    break;
		}
	    /* -> else FALL THROUGH */
#endif
	    case XK_Home:
		len = strlen(STRCPY(kbuf, KS_HOME));
		break;

#ifdef XK_KP_Left
	    case XK_KP_Left:	/* \033Ot or standard */
	    case XK_KP_Up:	/* \033Ox or standard */
	    case XK_KP_Right:	/* \033Ov or standard */
	    case XK_KP_Down:	/* \033Or or standard */
		if ((PrivateModes & PrivMode_aplKP) ? !shft : shft) {
		    len = 3;
		    STRCPY(kbuf, "\033OZ");
		    kbuf[2] = ("txvr"[keysym - XK_KP_Left]);
		    break;
		} else {
		/* translate to std. cursor key */
		    keysym = XK_Left + (keysym - XK_KP_Left);
		}
	    /* FALL THROUGH */
#endif
	    case XK_Left:	/* "\033[D" */
	    case XK_Up:	/* "\033[A" */
	    case XK_Right:	/* "\033[C" */
	    case XK_Down:	/* "\033[B" */
		len = 3;
		STRCPY(kbuf, "\033[@");
		kbuf[2] = ("DACB"[keysym - XK_Left]);
	    /* do Shift first */
		if (shft) {
		    kbuf[2] = ("dacb"[keysym - XK_Left]);
		} else if (ctrl) {
		    kbuf[1] = 'O';
		    kbuf[2] = ("dacb"[keysym - XK_Left]);
		} else if (PrivateModes & PrivMode_aplCUR) {
		    kbuf[1] = 'O';
		}
		break;

#ifndef UNSHIFTED_SCROLLKEYS
# ifdef XK_KP_Prior
	    case XK_KP_Prior:
	    /* allow shift to override */
		if ((PrivateModes & PrivMode_aplKP) ? !shft : shft) {
		    len = 3;
		    STRCPY(kbuf, "\033Oy");
		    break;
		}
	    /* -> else FALL THROUGH */
# endif
	    case XK_Prior:
		len = 4;
		STRCPY(kbuf, "\033[5~");
		break;
# ifdef XK_KP_Next
	    case XK_KP_Next:
	    /* allow shift to override */
		if ((PrivateModes & PrivMode_aplKP) ? !shft : shft) {
		    len = 3;
		    STRCPY(kbuf, "\033Os");
		    break;
		}
	    /* -> else FALL THROUGH */
# endif
	    case XK_Next:
		len = 4;
		STRCPY(kbuf, "\033[6~");
		break;
#endif
#ifdef XK_KP_End
	    case XK_KP_End:
	    /* allow shift to override */
		if ((PrivateModes & PrivMode_aplKP) ? !shft : shft) {
		    len = 3;
		    STRCPY(kbuf, "\033Oq");
		    break;
		}
	    /* -> else FALL THROUGH */
#endif
	    case XK_End:
		len = strlen(STRCPY(kbuf, KS_END));
		break;

	    case XK_Select:
		len = 4;
		STRCPY(kbuf, "\033[4~");
		break;
#ifdef DXK_Remove		/* support for DEC remove like key */
	    case DXK_Remove:	/* drop */
#endif
	    case XK_Execute:
		len = 4;
		STRCPY(kbuf, "\033[3~");
		break;
	    case XK_Insert:
		len = 4;
		STRCPY(kbuf, "\033[2~");
		break;

	    case XK_Menu:
		len = 5;
		STRCPY(kbuf, "\033[29~");
		break;
	    case XK_Find:
		len = 4;
		STRCPY(kbuf, "\033[1~");
		break;
	    case XK_Help:
		len = 5;
		STRCPY(kbuf, "\033[28~");
		break;

	    case XK_KP_Enter:
	    /* allow shift to override */
		if ((PrivateModes & PrivMode_aplKP) ? !shft : shft) {
		    len = 3;
		    STRCPY(kbuf, "\033OM");
		} else {
		    len = 1;
		    kbuf[0] = '\r';
		}
		break;

#ifdef XK_KP_Begin
	    case XK_KP_Begin:
		len = 3;
		STRCPY(kbuf, "\033Ou");
		break;

	    case XK_KP_Insert:
		len = 3;
		STRCPY(kbuf, "\033Op");
		break;

	    case XK_KP_Delete:
		len = 3;
		STRCPY(kbuf, "\033On");
		break;
#endif

	    case XK_KP_F1:	/* "\033OP" */
	    case XK_KP_F2:	/* "\033OQ" */
	    case XK_KP_F3:	/* "\033OR" */
	    case XK_KP_F4:	/* "\033OS" */
		len = 3;
		STRCPY(kbuf, "\033OP");
		kbuf[2] += (keysym - XK_KP_F1);
		break;

	    case XK_KP_Multiply:	/* "\033Oj" : "*" */
	    case XK_KP_Add:	/* "\033Ok" : "+" */
	    case XK_KP_Separator:	/* "\033Ol" : "," */
	    case XK_KP_Subtract:	/* "\033Om" : "-" */
	    case XK_KP_Decimal:	/* "\033On" : "." */
	    case XK_KP_Divide:	/* "\033Oo" : "/" */
	    case XK_KP_0:	/* "\033Op" : "0" */
	    case XK_KP_1:	/* "\033Oq" : "1" */
	    case XK_KP_2:	/* "\033Or" : "2" */
	    case XK_KP_3:	/* "\033Os" : "3" */
	    case XK_KP_4:	/* "\033Ot" : "4" */
	    case XK_KP_5:	/* "\033Ou" : "5" */
	    case XK_KP_6:	/* "\033Ov" : "6" */
	    case XK_KP_7:	/* "\033Ow" : "7" */
	    case XK_KP_8:	/* "\033Ox" : "8" */
	    case XK_KP_9:	/* "\033Oy" : "9" */
	    /* allow shift to override */
		if ((PrivateModes & PrivMode_aplKP) ? !shft : shft) {
		    len = 3;
		    STRCPY(kbuf, "\033Oj");
		    kbuf[2] += (keysym - XK_KP_Multiply);
		} else {
		    len = 1;
		    kbuf[0] = ('*' + (keysym - XK_KP_Multiply));
		}
		break;
		
		case XK_F1:		/* "\033OP" */
	    case XK_F2:		/* "\033OQ" */
	    case XK_F3:		/* "\033OR" */
	    case XK_F4:		/* "\033OS" */
		len = 3;
		STRCPY(kbuf, "\033OP");
		kbuf[2] += (keysym - XK_F1);
		break;

#define FKEY(n, fkey)							\
    len = 5;								\
    sprintf((char *) kbuf,"\033[%02d~", (int)((n) + (keysym - fkey)))
#if 0                          /* old style keymappings : */
	    case XK_F1:		/* "\033[11~" */
	    case XK_F2:		/* "\033[12~" */
	    case XK_F3:		/* "\033[13~" */
	    case XK_F4:		/* "\033[14~" */
			FKEY(11, XK_F1);
		break;
#endif			
	    case XK_F5:		/* "\033[15~" */
			FKEY(15, XK_F5);
		break;

	    case XK_F6:		/* "\033[17~" */
	    case XK_F7:		/* "\033[18~" */
	    case XK_F8:		/* "\033[19~" */
	    case XK_F9:		/* "\033[20~" */
	    case XK_F10:	/* "\033[21~" */
		FKEY(17, XK_F6);
		break;

	    case XK_F11:	/* "\033[23~" */
	    case XK_F12:	/* "\033[24~" */
	    case XK_F13:	/* "\033[25~" */
	    case XK_F14:	/* "\033[26~" */
		FKEY(23, XK_F11);
		break;

	    case XK_F15:	/* "\033[28~" */
	    case XK_F16:	/* "\033[29~" */
		FKEY(28, XK_F15);
		break;

	    case XK_F17:	/* "\033[31~" */
	    case XK_F18:	/* "\033[32~" */
	    case XK_F19:	/* "\033[33~" */
	    case XK_F20:	/* "\033[34~" */
	    case XK_F21:	/* "\033[35~" */
	    case XK_F22:	/* "\033[36~" */
	    case XK_F23:	/* "\033[37~" */
	    case XK_F24:	/* "\033[38~" */
	    case XK_F25:	/* "\033[39~" */
	    case XK_F26:	/* "\033[40~" */
	    case XK_F27:	/* "\033[41~" */
	    case XK_F28:	/* "\033[42~" */
	    case XK_F29:	/* "\033[43~" */
	    case XK_F30:	/* "\033[44~" */
	    case XK_F31:	/* "\033[45~" */
	    case XK_F32:	/* "\033[46~" */
	    case XK_F33:	/* "\033[47~" */
	    case XK_F34:	/* "\033[48~" */
	    case XK_F35:	/* "\033[49~" */
		FKEY(31, XK_F17);
		break;
#undef FKEY
	    }
    /*
     * Pass meta for all function keys, if 'meta' option set
     */
#ifdef META8_OPTION
	if (meta && (meta_char == 0x80) && len > 0) {
	    kbuf[len - 1] |= 0x80;
	}
#endif
    } else if (ctrl && keysym == XK_minus) {
	len = 1;
	kbuf[0] = '\037';	/* Ctrl-Minus generates ^_ (31) */
    } else {
#ifdef META8_OPTION
    /* set 8-bit on */
	if (meta && (meta_char == 0x80)) {
	    unsigned char  *ch;

	    for (ch = kbuf; ch < kbuf + len; ch++)
		*ch |= 0x80;
	    meta = 0;
	}
#endif
#ifdef GREEK_SUPPORT
	if (greek_mode)
	    len = greek_xlat(kbuf, len);
#endif
    /* nil */ ;
    }

    if (len <= 0)
	return;			/* not mapped */

/*
 * these modifications only affect the static keybuffer
 * pass Shift/Control indicators for function keys ending with `~'
 *
 * eg,
 *   Prior = "ESC[5~"
 *   Shift+Prior = "ESC[5~"
 *   Ctrl+Prior = "ESC[5^"
 *   Ctrl+Shift+Prior = "ESC[5@"
 * Meta adds an Escape prefix (with META8_OPTION, if meta == <escape>).
 */
    if (kbuf[0] == '\033' && kbuf[1] == '[' && kbuf[len - 1] == '~')
	kbuf[len - 1] = (shft ? (ctrl ? '@' : '$') : (ctrl ? '^' : '~'));

/* escape prefix */
    if (meta
#ifdef META8_OPTION
	&& (meta_char == 033)
#endif
	) {
	const unsigned char ch = '\033';

	tt_write(&ch, 1);
    }
#ifdef DEBUG_CMD
    if (debug_key) {		/* Display keyboard buffer contents */
	char           *p;
	int             i;

	fprintf(stderr, "key 0x%04X [%d]: `", (unsigned int)keysym, len);
	for (i = 0, p = kbuf; i < len; i++, p++)
	    fprintf(stderr, (*p >= ' ' && *p < '\177' ? "%c" : "\\%03o"), *p);
	fprintf(stderr, "'\n");
    }
#endif				/* DEBUG_CMD */
    tt_write(kbuf, len);
}
