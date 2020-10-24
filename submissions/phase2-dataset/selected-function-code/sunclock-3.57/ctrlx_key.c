{
	switch(key | 0x60) {
		case 'c': sys_exit(0); break;		/* ^XC exit only if file saved */
		case 'd': key_delword(0); break;	/* ^XD delete word */
		case 'f': file_save(0,1); break;	/* ^XF open new file */
		case 'h': show_help(0); break;		/* ^XH show help menu */
		case 'i': block_read(); break;		/* ^XI read file into cursor pos */
		case 'k': block_line(); break;		/* ^XK delete line */
		case 'm': key_macros(1); break; 	/* ^XM record macro */
		case 'p': key_macros(0); break; 	/* ^XP play macro */
		case 'q': sys_exit(2); break;		/* ^XQ exit even if file unsaved */
		case 's': do_save(); break; 		/* ^XS write block to disk */
		case 't': tab_size(); break;		/* ^XT get tab size */
		case 'v': block_write(); break; 	/* ^XV write block to disk */
		case 'w': file_save(0,-1); break;	/* ^XW save file as */
		case 'x': goto_last(); break;		/* ^XX goto last pos */
	}
	doCtrlX = 0;
	show_top();
}
