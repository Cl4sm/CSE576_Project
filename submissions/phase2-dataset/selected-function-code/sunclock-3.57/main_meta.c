{
    if (key>=1 && key<=10)
    switch(key) {
		case 1: show_help(0); break;			/* F1 show help */
		case 2: do_save(); break; 			/* F2 save file and resume */
		case 3: file_save(0,1); break;			/* F3 open new file */
		case 4: find_match(); break; 			/* F4 find {} () [] match  */
		case 5: run(); break;	 			/* F5 get and run cmd */
		case 6: chgdir(); break; 			/* F6 get & change to dir */
		case 7: block_mark(); break;			/* F7 set mark block on */
		case 8: block_mark(); break;			/* F8 set mark block on */
#ifndef MINIMAL
		case 9: req_edit(); break;			/* F9 open new file in new window */
#endif
		case 10: key_binding('z'); break;			/* F10 open rxvt in cur.dir */
	}
    else
    switch(key | 0x60) {
        case 'b': block_mark(); break;          /* F7 set mark block on */
        case 'c': block_mark(); break;          /* F8 set mark block on */
        case 'd': goto_x(strlen(line_start+1)+1); break;/* End eol */
        case 'f': file_save(0,-1); break;       /* F3 open new file */
        case 'g': key_delete(); break;          /* Del delete cursor char */
        case 'n': show_flag(OVR, !flag[OVR]); break;/* Ins toggle insert mode */
        case 'r': do_save(); break;             /* F2 save file and resume */
        case 's': goto_x(1); break;             /* Home bol */
        case 'v': cursor_pageup(); break;       /* PgUp */
        case 'w': cursor_pagedown(); break;     /* PgDn */
//		case 'z': key_shell(0); break;          /* F10 execute a shell */
    }
    funckey=0;
    show_top();
}
