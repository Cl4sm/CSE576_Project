process_xterm_seq(void)
{
    unsigned char   ch, string[STRING_MAX];
    int             arg;

    ch = cmd_getc();
    for (arg = 0; isdigit(ch); ch = cmd_getc())
	arg = arg * 10 + (ch - '0');

    if (ch == ';') {
	int             n = 0;

	while ((ch = cmd_getc()) != 007) {
	    if (ch) {
		if (ch == '\t')
		    ch = ' ';	/* translate '\t' to space */
		else if (ch < ' ')
		    return;	/* control character - exit */

		if (n < sizeof(string) - 1)
		    string[n++] = ch;
	    }
	}
	string[n] = '\0';
    /*
     * menubar_dispatch() violates the constness of the string,
     * so do it here
     */
	if (arg == XTerm_Menu)
	    menubar_dispatch((char *) string);
	else
	    xterm_seq(arg, (char *) string);
    }
}
