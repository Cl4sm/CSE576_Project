{
	switch(tolower(key)) {
		case ' ':
		case '.': block_mark(); break;		/* Esc-BLANK set mark block on */
#ifndef MINIMAL
		case '!': doEscap = 2; show_note(ALT_KEY_BINDINGS); return;	/* Esc-! = Alt-key bindings */
#endif
		case '_': redo(); break;		/* Esc-_ redo */
		case '%': goto_replace(); break;	/* Esc-% replace */

		case '<': top(); break;			/* Esc-< bof */
		case '>': y = screen_height-1; goto_ptr(file_end); break; 	/* Esc-> eof */
		case 'b': word_left(); break;		/* Esc-B word left */
		case 'f': word_right(); break;		/* Esc-F word right */
		case 'g': keve->state & ControlMask ?
					goto_line() :	/* goto line# */
					key_return(); break;	/* newline */
#ifdef JUSTIFY
		case 'j': block_format(1); break;	/* Esc-J reformat & fill block */
#endif
		case 'l': chg_case(0); break; 		/* Esc-L lower case block */
		case 'm': window_size(); break; 	/* Esc-M get right margin */
                case 'n': scroll_up(); break;		/* Esc-N scroll down (!) */
		case 'p': scroll_down(); break;		/* Esc-P scroll up (!) */
		case 'q': block_format(0); break;	/* Esc-Q reformat block */
		case 'u': chg_case(1); break; 		/* Esc-U upper case block */
		case 'v': cursor_pageup(); break;	/* Esc-V pgup */

		case 'w': reset_mark(); block_copy(0); 
                          reset_mark(); break; 		/* Esc-W copy block to buffer */
		case 'x': switch_marks(); break;	/* Esc-X switch between block marks */
	}
	doEscap = 0;
	show_top();
}
