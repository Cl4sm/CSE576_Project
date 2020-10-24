irc_exit()
{
	do_hook(EXIT_LIST, "Exiting");
	close_server(-1, empty_string);
	logger(0);
	set_history_file((u_char *) 0);
#ifndef _Windows
	clean_up_processes();
#endif /* _Windows */
	if (!dumb)
	{
		cursor_to_input();	/* Needed so that ircII doesn't gobble
					 * the last line of the kill. */
		term_cr();
		if (term_clear_to_eol())
			term_space_erase(0);
		term_reset();
	}
#ifdef _Windows
   DestroyWindow(hwndMain);
#else
	exit(0);
#endif /* _Windows */
}
