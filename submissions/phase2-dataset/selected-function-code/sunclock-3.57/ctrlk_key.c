void ctrlk_key(int key)
{
	switch(key | 0x60) {
		case 'b': block_mark(); break;		/* ^K^B set mark block on */
		case 'c': set_selection();			/* ^K^C block copy to X clipboard */
				  block_copy(0); break; 	/*	and to bbuffer */
		case 'd': flag[CHG]=0; break;		/* ^K^D say file not changed */
		case 'f': file_save(0,-1); break;	/* ^K^F new file */
#ifdef GREEK
		case 'g': doCtrlC = 1; break;		/* ^K^G Greek char */
#endif /* GREEK */
		case 'h': show_help(0); break;		/* ^K^H show help menu */
		case 'k': block_mark(); break;		/* ^K^K set mark block on */
		case 'm': key_macros(1); break; 	/* ^K^M record macro */
		case 'p': key_macros(0); break; 	/* ^K^P play macro */
		case 'q': sys_exit(0); break;		/* ^K^Q exit only if file saved */
		case 'r': block_read(); break;		/* ^K^R read file into cursor pos */
		case 's': do_save(); break; 		/* ^K^S save file and resume */
		case 't': tab_size(); break;		/* ^K^T get tab size */
		case 'u': redo(); break;	 	/* ^K^U redo */
		case 'v': block_paste(); break; 	/* ^K^V copy buffer to cursor */
		case 'w': block_write(); break; 	/* ^K^W write block to disk */
		case 'x': sys_exit(0); break;		/* ^K^X exit only if file saved */
		case 'y': block_copy(1); break; 	/* ^K^Y cut block to buffer */
#ifdef MINIMAL
		case 'z': mterm(); break;		/* ^K^Z open an rxvt term */
#endif

	}
	doCtrlK = 0;
	show_top();
}
