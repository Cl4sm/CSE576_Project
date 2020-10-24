{
	switch(key | 0x60) {
		case 'a': goto_replace(); break;	/* ^Q^A replace */
#ifdef JUSTIFY
		case 'b': block_format(1); break; 	/* ^Q^B right justify */
#endif
		case 'c': y = screen_height-1; goto_ptr(file_end); break;	/* ^Q^C eof */
		case 'd': goto_x(strlen(line_start+1)+1); break;	/* ^Q^D eol */
		case 'f': goto_search(0); break;	/* ^Q^F find */
		case 'i': goto_line(); break;		/* ^Q^I goto line */
		case 'l': chg_case(0); break; 		/* ^Q^L lower case block */
		case 'm': window_size(); break; 	/* ^Q^M get right margin */
		case 'p': goto_last(); break;		/* ^Q^P goto last pos */
		case 'r': top();					/* ^Q^R bof */
		case 's': goto_x(1); break; 		/* ^Q^S bol */
		case 't': transpose(); break;		/* ^Q^T transpose chars */
		case 'u': chg_case(1); break; 		/* ^Q^L upper case block */
		case 'x': switch_marks(); break;	/* ^Q^X switch marks */

		case 'y': key_delword(1); break;	/* ^Q^Y delete to end of line */
	}
	doCtrlQ = 0;
	show_top();
}
