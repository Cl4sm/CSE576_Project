display_colours(fgcolour, bgcolour)
	int fgcolour;
	int bgcolour;
{
	static int current_fg = COLOUR_DEFAULT;
	static int current_bg = COLOUR_DEFAULT;
	static int current_bold = 0;
	static int current_blink = 0;

	int emit_code = 0;
	int desired_bold, desired_blink;

	if (fgcolour < 0)
		fgcolour = COLOUR_DEFAULT;
	if (bgcolour < 0)
		bgcolour = COLOUR_DEFAULT;

	if (get_int_var(COLOUR_VAR))
	{
		/* Some people will say that I should use termcap values but
		 * since:
		 * 1- iso 6429 is the only used way for colour in the unix
		 *    realm for now
		 * 2- colour information in termcap entries is still rare
		 * 3- information about colour information in termcap entries
		 *    is still rare too
		 * ... I'll stick with this way for now. But having only 8-9
		 * colour is a pity.
		 *    -- Sarayan
		 */
		 
		/* Written by Bisqwit (bisqwit@iki.fi) */
		/* Reworked to support default terminal color, allowing use of
		 * color on non-black background terminals.  20061129
		 *    -- Gregory P. Smith (greg@krypto.org)
		 */
		 
		/* mirc colours -> iso 6469 colours translation tables */
		static const u_char trans[] = "7042115332664507";
		static const u_char bolds[] = "1000100011011110";
		                            /* 0123456789ABCDEF */
		
		u_char iso[15]; /* long enough for "e[0;1;5;37;40m" */
		
		snprintf(CP(iso), sizeof iso, "\33[");
		if (fgcolour != COLOUR_DEFAULT)
			desired_bold = bolds[fgcolour&15] - '0';
		else
			desired_bold = current_bold;

		if (bgcolour != COLOUR_DEFAULT)
			desired_blink = bolds[bgcolour&15] - '0';
		else
			desired_blink = current_blink;

		/* to turn off bold or blink we have to issue a '0' for reset */
		if (current_bold != desired_bold || current_blink != desired_blink)
		{
			my_strcat(iso, "0");
			emit_code = 1;
			current_bold = 0;
			current_blink = 0;
			current_bg = COLOUR_DEFAULT;	/* XXX is this needed? */
			current_fg = COLOUR_DEFAULT;	/* XXX is this needed? */
		}
		if (desired_bold)
		{
			if (emit_code)
				my_strcat(iso, ";");
			my_strcat(iso, "1");
			emit_code = 1;
			current_bold = 1;
		}
		if (desired_blink)
		{
			if (emit_code)
				my_strcat(iso, ";");
			my_strcat(iso, "5");
			emit_code = 1;
			current_blink = 1;
		}

		if (fgcolour != current_fg)
		{
			if (emit_code)
				my_strcat(iso, ";");
			snprintf(CP(my_index(iso, 0)), 3, "3%c", trans[fgcolour&15]);
			emit_code = 1;
		}
		if (bgcolour != current_bg)
		{
			if (emit_code)
				my_strcat(iso, ";");
			snprintf(CP(my_index(iso, 0)), 3, "4%c", trans[bgcolour&15]);
			emit_code = 1;
		}
		if (emit_code)
		{
			my_strcat(iso, "m");
			fwrite(CP(iso), my_strlen(iso), 1, current_screen->fpout);
		}
	}
}
