int
rxvt_0xffxx_keypress (rxvt_t* r, KeySym keysym,
	int ctrl, int meta, int shft, unsigned char* kbuf)
{
    /*
     * 2006-04-08 gi1242:  The KeyPad is treated as follows:
     *
     * 	    1. If NumLock is on: Unmodified keys send the regular char. Modified
     * 	       keys send the DEC sequence in private mode, and the regular char
     * 	       in normal model.
     *
     * 	    2. If NumLock is off: Keys always send the DEC private mode
     * 	       sequence.
     */

    /*
     * B: beginning of a {} body
     */
    unsigned int	newlen = 1;

    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "0xffxx_keypress: (C%d, M%d, S%d) %lx ...", ctrl, meta, shft, keysym));

    switch (keysym)
    {

#ifndef NO_BACKSPACE_KEY
	case XK_BackSpace:
	    {
		unsigned char *bsKbuf = kbuf;

		if (
			meta
# ifdef META8_OPTION
			&& (r->h->meta_char == C0_ESC)
# endif
		   )
		{
		    /* 7bit mode. Prefix with C0_ESC */
		    *(bsKbuf++)	= C0_ESC;
		    newlen++;

		    meta = 0;
		}

		if (ISSET_PMODE(r, ATAB(r), PrivMode_HaveBackSpace))
		{
		    bsKbuf[0] = (!!(ISSET_PMODE(r, ATAB(r),PrivMode_BackSpace))
				    ^ !!ctrl)
				? '\b' : '\177';
		    bsKbuf[1] = '\0';
		}
		else
		    /*
		     * If PrivMode_HaveBackSpace is unset, then h->key_backspace
		     * will be a one character string.
		     */
		    STRCPY( bsKbuf, r->h->key_backspace);

# ifdef META8_OPTION
		if( meta )  /* We must be in 8bit mode if meta is set here */
		    *bsKbuf |= r->h->meta_char;
# endif

# ifdef MULTICHAR_SET
		if (ISSET_OPTION(r, Opt_mc_hack) && AVTS(r)->screen.cur.col > 0)
		{
		    int		 col, row;

		    newlen = STRLEN(kbuf);
		    col = AVTS(r)->screen.cur.col - 1;
		    row = AVTS(r)->screen.cur.row + AVTS(r)->saveLines;
		    if (IS_MULTI2(AVTS(r)->screen.rend[row][col]))
			MEMMOVE(kbuf + newlen, kbuf, newlen + 1);
		}
# endif	/* MULTICHAR_SET */

		break;
	    }
#endif	/* !NO_BACKSPACE_KEY */



	case XK_Tab:
	    /*
	     * 2006-04-07 gi1242: Shift tab is sometimes sent as ISO_Left_Tab,
	     * which has keysym 0xfe20 (outside the 0xff00 -- 0xffff range). So
	     * we have to process shift tab elsewhere.
	     */
	    if (shft)
		STRCPY(kbuf, "\033[Z");
	    else
	    {
#ifdef CTRL_TAB_MAKES_META
		if (ctrl)
		    meta = 1;
#endif	/* CTRL_TAB_MAKES_META */
#ifdef MOD4_TAB_MAKES_META
		if (ev->state & Mod4Mask)
		    meta = 1;
#endif	/* MOD4_TAB_MAKES_META */
		newlen = 0;
	    }
	    break;

	case XK_Return:
	    newlen = 0;
#ifdef META8_OPTION
	    if (r->h->meta_char == 0x80)
		/*
		 * 2006-12-12 gi1242: In 8 bit mode, xterm sends 0x8d for
		 * Alt+enter. Enter sends 0x0d.
		 */
		kbuf[newlen++] = meta ? (r->h->meta_char|C0_CR) : C0_CR;
	    else
#endif	/* META8_OPTION */
	    {
		if( meta )
		    kbuf[newlen++] = C0_ESC;
		kbuf[newlen++] = C0_CR;
	    }
	    kbuf[newlen] = '\0';
	    break;

#ifdef XK_KP_Left
	case XK_KP_Up:	    /* \033Ox or standard */
	case XK_KP_Down:    /* \033Or or standard */
	case XK_KP_Right:   /* \033Ov or standard */
	case XK_KP_Left:    /* \033Ot or standard */
	    /*
	     * If numlock is on, we got here by pressing Shift + Keypad 8.
	     * Cancel the Shift, and send the DEC terminal sequence.
	     */
	    if( r->numlock_state ) shft = 0;

	    if (ISSET_PMODE(r, ATAB(r), PrivMode_aplKP))
	    {
		set_xterm_key_seq( kbuf, "\033O", "\033O%d%c",
			"txvr"[keysym - XK_KP_Left], ctrl, meta, shft );
		break;
	    }

	    /* translate to std. cursor key */
	    keysym = XK_Left + (keysym - XK_KP_Left);

	    /* FALLTHROUGH */
#endif	/* XK_KP_Left */

	case XK_Up:	/* "\033[A" */
	case XK_Down:	/* "\033[B" */
	case XK_Right:	/* "\033[C" */
	case XK_Left:	/* "\033[D" */
#ifdef XTERM_KEYS
	    /*
	     * Modified cursor keys are \e[1;%d%c, where %d is the modifier
	     * number and %c is D, A, C or B for Up, Dn, Left / Right
	     * resply.
	     */
	    set_xterm_key_seq( kbuf, "\033[", "\033[1;%d%c",
		    "DACB"[keysym - XK_Left], ctrl, meta, shft);

	    if (ISSET_PMODE(r, ATAB(r), PrivMode_aplCUR) &&
		!ctrl && !meta && !shft)
		/*
		 * 2006-04-07 gi1242 XXX: Should we ignore the modifiers in
		 * the private mode?
		 */
		kbuf[1] = 'O';
#else
	    STRCPY(kbuf, "\033[Z");
	    kbuf[2] = ("DACB"[keysym - XK_Left]);
	    /* do Shift first */
	    if (shft)
		kbuf[2] = ("dacb"[keysym - XK_Left]);
	    else if (ctrl)
	    {
		kbuf[1] = 'O';
		kbuf[2] = ("dacb"[keysym - XK_Left]);
	    }
	    else if (ISSET_PMODE(r, ATAB(r), PrivMode_aplCUR))
		kbuf[1] = 'O';
#endif

#ifdef MULTICHAR_SET
	    if (ISSET_OPTION(r, Opt_mc_hack))
	    {
		/*
		 * If we're on a multibyte char, and we move left / right, then
		 * duplicate the cursor key string.
		 */
		int	     col, row, m;

		col = AVTS(r)->screen.cur.col;
		row = AVTS(r)->screen.cur.row + AVTS(r)->saveLines;
		m = 0;
		if (keysym == XK_Right &&
		    IS_MULTI1(AVTS(r)->screen.rend[row][col]))
		    m = 1;
		else if (keysym == XK_Left)
		{
		    if (col > 0)
		    {
			if (IS_MULTI2(AVTS(r)->screen.rend[row][col - 1]))
			    m = 1;
		    }
		    else if (AVTS(r)->screen.cur.row > 0)
		    {
			col = AVTS(r)->screen.tlen[--row];
			if (col == -1)
			    col = r->TermWin.ncol - 1;
			else
			    col--;
			if (col > 0 &&
			    IS_MULTI2(AVTS(r)->screen.rend[row][col]))
			    m = 1;
		    }
		}

		if (m)
		{
		    /* Duplicate the escape sequence in kbuf */
		    int len = STRLEN( kbuf );

		    MEMMOVE( kbuf + len, kbuf, len + 1);
		}
	    }
#endif	/* MULTICHAR_SET */

	    break;


# ifdef XK_KP_Prior
	case XK_KP_Prior:	/* \033[Oy */
	    if( r->numlock_state ) shft = 0;

	    if (ISSET_PMODE(r, ATAB(r), PrivMode_aplKP))
	    {
		set_xterm_key_seq( kbuf, "\033O", "\033O%d%c",
			'y', ctrl, meta, shft );
		break;
	    }
	    /* FALLTHROUGH */
# endif	/* XK_KP_Prior */
	case XK_Prior:
	    SET_TILDE_KEY_SEQ( kbuf, "\033[5");
	    break;


# ifdef XK_KP_Next
	case XK_KP_Next:	/* \033Os */
	    if( r->numlock_state ) shft = 0;

	    if (ISSET_PMODE(r, ATAB(r), PrivMode_aplKP))
	    {
		set_xterm_key_seq( kbuf, "\033O", "\033O%d%c",
			's', ctrl, meta, shft );
		break;
	    }
	    /* FALLTHROUGH */

# endif	/* XK_KP_Next */
	case XK_Next:
	    SET_TILDE_KEY_SEQ(kbuf, "\033[6");
	    break;


	case XK_KP_Enter:   /* \033OM */

	    if (ISSET_PMODE(r, ATAB(r), PrivMode_aplKP) &&
		(!r->numlock_state || ctrl || meta || shft))
	    {
#ifdef NUMLOCK_ALWAYS_IGNORES_SHIFT
		if( r->numlock_state ) shft = 0;
#endif
		set_xterm_key_seq( kbuf, "\033O", "\033O%d%c", 'M',
			ctrl, meta, shft );
	    }
	    else
	    {
		kbuf[0] = '\r';
		kbuf[1] = '\0';
	    }
	    break;

#ifdef XK_KP_Begin
	case XK_KP_Begin:   /* \033Ou (This is keypad 5) */
	    if( r->numlock_state ) shft = 0;

	    if (ISSET_PMODE(r, ATAB(r), PrivMode_aplKP))
	    {
		set_xterm_key_seq( kbuf, "\033O", "\033O%d%c", 'u',
			ctrl, meta, shft );
	    }
	    else
	    {
		kbuf[0] = '5';
		kbuf[1] = '\0';
	    }

	    break;
#endif	/* XK_KP_Begin */


#ifdef XK_KP_Insert
	case XK_KP_Insert:  /* \033Op */
	    if( r->numlock_state ) shft = 0;

	    if (ISSET_PMODE(r, ATAB(r), PrivMode_aplKP))
	    {
		set_xterm_key_seq( kbuf, "\033O", "\033O%d%c", 'p',
			ctrl, meta, shft );
		break;
	    }
	    /* FALLTHROUGH */
#endif /* XK_KP_Insert */

	case XK_Insert:
	    SET_TILDE_KEY_SEQ( kbuf, "\033[2" );
	    break;


#ifdef XK_KP_Delete
	case XK_KP_Delete:  /* \033On */
	    if( r->numlock_state ) shft = 0;

	    if (ISSET_PMODE(r, ATAB(r), PrivMode_aplKP))
	    {
		set_xterm_key_seq( kbuf, "\033O", "\033O%d%c", 'n',
			ctrl, meta, shft );
		break;
	    }
	    /* FALLTHROUGH */
#endif

	case XK_Delete:
#ifndef NO_DELETE_KEY
	    STRCPY( kbuf, r->h->key_delete);
#else
	    SET_TILDE_KEY_SEQ( kbuf, "\033[3" );
#endif

#ifdef MULTICHAR_SET
	    if (ISSET_OPTION(r, Opt_mc_hack))
	    {
		int	     col, row;

		newlen = STRLEN(kbuf);
		col = AVTS(r)->screen.cur.col;
		row = AVTS(r)->screen.cur.row + AVTS(r)->saveLines;

		if (IS_MULTI1(AVTS(r)->screen.rend[row][col]))
		    MEMMOVE(kbuf + newlen, kbuf, newlen + 1);
	    }
#endif	/* MULTICHAR_SET */
	    break;


	case XK_KP_Multiply:	/* "\033Oj" : "*" */
	case XK_KP_Add:		/* "\033Ok" : "+" */
	case XK_KP_Separator:	/* "\033Ol" : "," */
	case XK_KP_Subtract:	/* "\033Om" : "-" */
	case XK_KP_Divide:	/* "\033Oo" : "/" */

	case XK_KP_Decimal:	/* "\033On" : "." */
	case XK_KP_0:		/* "\033Op" : "0" */
	case XK_KP_1:		/* "\033Oq" : "1" */
	case XK_KP_2:		/* "\033Or" : "2" */
	case XK_KP_3:		/* "\033Os" : "3" */
	case XK_KP_4:		/* "\033Ot" : "4" */
	case XK_KP_5:		/* "\033Ou" : "5" */
	case XK_KP_6:		/* "\033Ov" : "6" */
	case XK_KP_7:		/* "\033Ow" : "7" */
	case XK_KP_8:		/* "\033Ox" : "8" */
	case XK_KP_9:		/* "\033Oy" : "9" */

	    /*
	     * If numlock is on, and no modifiers are presed, then we should
	     * send the actual number key. If modifiers are pressed, send the
	     * DEC sequence (after canceling shift ofcourse).
	     *
	     * If numlock is off, then send the DEC terminal sequence.
	     */
	    if (ISSET_PMODE(r, ATAB(r), PrivMode_aplKP) &&
		(!r->numlock_state || ctrl || meta || shft))
	    {
		if(
			r->numlock_state
#ifndef NUMLOCK_ALWAYS_IGNORES_SHIFT
			&& keysym >= XK_KP_Decimal && keysym <= XK_KP_9
#endif
		  )
		    /*
		     * Cancel shift if numlock is pressed, but only cancel it
		     * for keys where shift has some meaning: 0-9 and Decimal.
		     * If NUMLOCK_ALWAYS_IGNORES_SHIFT is defined, then shift is
		     * always cancelled when numlock is on.
		     */
		    shft = 0;

		set_xterm_key_seq( kbuf, "\033O", "\033O%d%c",
			'j' + (keysym - XK_KP_Multiply), ctrl, meta, shft );
	    }
	    else
	    {
		kbuf[0] = ('*' + (keysym - XK_KP_Multiply));
		kbuf[1] = '\0';
	    }
	    break;

	case XK_Find:
	    SET_TILDE_KEY_SEQ(kbuf, "\033[1");
	    break;

#ifdef DXK_Remove	/* support for DEC remove like key */
	case DXK_Remove:
	    /* FALLTHROUGH */
#endif	/* DXK_Remove */
	case XK_Execute:
	    SET_TILDE_KEY_SEQ(kbuf, "\033[3");
	    break;

	case XK_Select:
	    SET_TILDE_KEY_SEQ(kbuf, "\033[4");
	    break;

#ifdef XK_KP_End
	case XK_KP_End:	/* \033Oq */
	    if( r->numlock_state ) shft = 0;

	    if (ISSET_PMODE(r, ATAB(r), PrivMode_aplKP))
	    {
		set_xterm_key_seq( kbuf, "\033O", "\033O%d%c", 'q',
			ctrl, meta, shft );
		break;
	    }
	    /* FALLTHROUGH */

#endif	/* XK_KP_End */

	case XK_End:
	    if (ISSET_OPTION(r, Opt2_linuxHomeEndKey))
		SET_TILDE_KEY_SEQ( kbuf, KS_END_LINUX);
	    else
		SET_TILDE_KEY_SEQ( kbuf, KS_END);
	    break;

#ifdef XK_KP_Home
	case XK_KP_Home: /* \033Ow */
	    if( r->numlock_state ) shft = 0;

	    if (ISSET_PMODE(r, ATAB(r), PrivMode_aplKP))
	    {
		set_xterm_key_seq( kbuf, "\033O", "\033O%d%c", 'w',
			ctrl, meta, shft );
		break;
	    }
	    /* FALLTHROUGH */

#endif	/* XK_KP_Home */
	case XK_Home:
	    if (ISSET_OPTION(r, Opt2_linuxHomeEndKey))
		SET_TILDE_KEY_SEQ( kbuf, KS_HOME_LINUX);
	    else
		SET_TILDE_KEY_SEQ( kbuf, KS_HOME);
	    break;

#define FKEY_SEQ_START( n, fkey)    \
	sprintf( (char*) kbuf, "\033[%2d", (n) + (int) (keysym - (fkey)) );


	case XK_KP_F1:	/* "\033OP" */
	case XK_KP_F2:	/* "\033OQ" */
	case XK_KP_F3:	/* "\033OR" */
	case XK_KP_F4:	/* "\033OS" */
	    /* XXX 2006-04-08 gi1242: Should we cancel shift here? */

	    if (ISSET_PMODE(r, ATAB(r), PrivMode_aplKP))
	    {
		set_xterm_key_seq( kbuf, "\033O", "\033O%d%c",
			'P' + (keysym - XK_KP_F1), ctrl, meta, shft);
		break;
	    }
	    else
		keysym = XK_F1 + (keysym - XK_KP_F1);
	    /* FALLTHROUGH */

	case XK_F1: /* "\033[11~" */
	case XK_F2: /* "\033[12~" */
	case XK_F3: /* "\033[13~" */
	case XK_F4: /* "\033[14~" */
	    if (TERMENV_XTERM == AVTS(r)->termenv)
	    {
		/* Xterm sends \eOP, \eOQ, \eOR, \eOS for f1 -- f4 */
		set_xterm_key_seq( kbuf, "\033O", "\033O%d%c",
			'P' + (keysym - XK_F1), ctrl, meta, shft);
		break;
	    }
	    /* else FALL THROUGH */
	case XK_F5: /* "\033[15~" */
	    FKEY_SEQ_START( 11, XK_F1);
	    SET_TILDE_KEY_SEQ( kbuf + 4, "" );
	    break;

	case XK_F6:	/* "\033[17~" */
	case XK_F7:	/* "\033[18~" */
	case XK_F8:	/* "\033[19~" */
	case XK_F9:	/* "\033[20~" */
	case XK_F10:	/* "\033[21~" */
	    FKEY_SEQ_START(17, XK_F6);
	    SET_TILDE_KEY_SEQ( kbuf + 4, "" );
	    break;

	case XK_F11:	/* "\033[23~" */
	case XK_F12:	/* "\033[24~" */
	case XK_F13:	/* "\033[25~" */
	case XK_F14:	/* "\033[26~" */
	    FKEY_SEQ_START(23, XK_F11);
	    SET_TILDE_KEY_SEQ( kbuf + 4, "" );
	    break;

	case XK_F15:	/* "\033[28~" */
	case XK_F16:	/* "\033[29~" */
	    FKEY_SEQ_START(28, XK_F15);
	    SET_TILDE_KEY_SEQ( kbuf + 4, "" );
	    break;

	case XK_Help:	/* "\033[28~" */
	    FKEY_SEQ_START(28, XK_Help);
	    SET_TILDE_KEY_SEQ( kbuf + 4, "" );
	    break;

	case XK_Menu:	/* "\033[29~" */
	    FKEY_SEQ_START(29, XK_Menu);
	    SET_TILDE_KEY_SEQ( kbuf + 4, "" );
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
	    FKEY_SEQ_START(31, XK_F17);
	    SET_TILDE_KEY_SEQ( kbuf + 4, "" );
	    break;

#undef FKEY_SEQ_START
	default:
	    newlen = 0;
	    break;

    }	/* switch (keysym) */


    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Returning %d\n", newlen ? (int) STRLEN(kbuf) : -1 ));

    return newlen ? STRLEN(kbuf) : -1;
    /*
     * B: end of a {} body
     */
}