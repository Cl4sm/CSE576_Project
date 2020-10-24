{
	if(!(keve->state & ShiftMask) && flag[BLK]) mark_off();
	switch(key|0x60) {
		case 'a': goto_x(1); break; 		/* bol */
		case 'b': cursor_left(); break; 	/* left */
#ifdef GREEK
		case 'c': doCtrlC = 1; break; 		/* ^C switch */
#endif
		case 'd': key_delete(); break;		/* delete cursor char */
		case 'e': goto_x(strlen(line_start+1)+1); break;	/* goto eol */
		case 'f': cursor_right(); break;	/* right */
		case 'h': key_backspace(); break;	/* destructive backspace */
		case 'j': key_return(); break;		/* newline at cursor */
		case 'k': key_delword(1); break;	/* delete to eol */
		case 'l': expose=1; scr_update(); expose=0; break;		/* refresh screen */
		case 'n': cursor_down(); break; 	/* down */
		case 'o': show_mode(); break; 		/* change modes */
		case 'p': cursor_up(); break;		/* up */
		case 'q': literal = 1; break;		/* literal */
		case 'r': goto_search(1); break;	/* ^R find backward */
		case 's': goto_search(0); break;	/* ^S find foreword */
		case 't': transpose(); break;		/* ^T transpose */
		case 'u': if (search_mode<=1)		/* repeat search */
				goto_find(cur_pos, search_mode); 
			  else				/* repeat replace */
				ask_replace();
			  break; /* search or replace again */
		case 'v': cursor_pagedown(); break; 	/* ^V pgdn */
		case 'w': reset_mark();
                          block_copy(1); break; 	/* ^W cut block at buffer */
		case 'x': doCtrlX = 1; break;		/* ^X switch */
		case 'y': block_paste(); break; 	/* ^Y copy buffer at cursor */
		case 'z': iconify(); break;		/* ^Z iconify window */
		case 0x7f: undo(0); break;		/* ^_ undo */
							/* user typed Ctrl-_ */
	}
	show_top();
}
