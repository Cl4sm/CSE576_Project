{
	if(keve->state & ShiftMask)
		switch(key) {
		case XK_Insert	: do_paste(); return; 			/* Shift Ins paste */
		case XK_Delete	: do_select(1); return;			/* Shift Del cut */
		}
	if((keve->state & ShiftMask) && !flag[BLK] &&
		key >= XK_Home && key <= XK_End) block_mark();	/* enable marked block with Shift */
	if(keve->state & ControlMask) {
		switch(key) {
		case XK_Tab		: word_mark(); break;			/* ^Tab mark cursor word */
#ifndef MINIMAL
		case XK_F1		: new_edit(DOCDIR"/MANUAL.emx"); break;			/* ^F1 open emx Manual */
#endif
		case XK_F9		: new_edit(""); break;			/* ^F3 open new emx */
		case XK_Home	: top(); break; 				/* ^Home bof */
		case XK_End 	: goto_y(ytot); break;			/* ^End eof */
		case XK_Left	: word_left(); break;			/* ^Left word left */
		case XK_Right	: word_right(); break;			/* ^Right word right */
		case XK_Insert	: do_select(0); break;			/* ^Ins copy */
		}
	} else
		switch(key) {
		case XK_F1		: show_help(0); break;			/* F1 show help */
		case XK_F2		: do_save(); break; 			/* F2 save file and resume */
		case XK_F3		: file_save(0,1); break;		/* F3 open new file */
		case XK_F4		: find_match(); break; 			/* F4 find {} () [] match  */
		case XK_F5		: run(); break; 				/* F5 get and run cmd */
		case XK_F6		: chgdir(); break; 				/* F6 get & change to dir */
		case XK_F7		: block_mark(); break;			/* F7 set mark block on */
		case XK_F8		: block_mark(); break;			/* F8 set mark block on */
#ifndef MINIMAL
		case XK_F9		: req_edit(); break;		/* F9 open new file in new window */
#endif
		case XK_F10 	: key_binding('z'); break;				/* F10 open rxvt in cur.dir */
		case XK_F12 	: show_flag(OVR, !flag[OVR]); break;/* Ins toggle insert mode */
		case XK_Return	: key_return(); break;			/* Enter newline at cursor */
		case XK_Tab 	: key_tab(0); break;			/* Tab insert tab char at cursor */
		case XK_BackSpace: key_backspace(); break;		/* BS delete prev char */
		case XK_Insert	: show_flag(OVR, !flag[OVR]); break;/* Ins toggle insert mode */
		case XK_Delete	: key_delete(); break;			/* Del delete cursor char */
		case XK_Page_Up : cursor_pageup(); break;		/* PgUp */
		case XK_Page_Down: cursor_pagedown(); break;	/* PgDn */
		case XK_End 	: goto_x(strlen(line_start+1)+1); break;/* End eol */
		case XK_Home	: goto_x(1); break; 			/* Home bol */
		case XK_Up	: cursor_up(); break;			/* up */
		case XK_Down	: cursor_down(); break; 		/* down */
		case XK_Right	: cursor_right(); break;		/* right */
		case XK_Left	: cursor_left(); break; 		/* left */
	}
	if(!(keve->state & ShiftMask) && flag[BLK]) mark_off();	 // turn off marked block
}
