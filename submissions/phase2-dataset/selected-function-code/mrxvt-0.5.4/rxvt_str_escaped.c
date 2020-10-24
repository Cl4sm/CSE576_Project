int
rxvt_str_escaped(char *str)
{
    char            ch, *s, *d;
    int             i, num;

    if (IS_NULL(str) || *str == '\0') return 0;

    d = s = str;

    /*
     * 2006-03-23 gi1242: Disabled, as the user has no 'easy' way to send
     * strings beginning with M-x to mrxvt.
     *
     * 2006-05-24 gi1242: With macros, the user has an 'easy' way to send
     * strings to mrxvt. However emacs users should use emacs macros do do such
     * things, and not require code bloat in mrxvt for these random features.
     * Besides, mrxvt users should use Vim anyway ... :)
     */
#if 0
    if (*s == 'M' && s[1] == '-')
    {
	/* Emacs convenience, replace leading `M-..' with `\E..' */
	*d++ = C0_ESC;
	s += 2;
	if (toupper((int) *s) == 'X')
	    /* append carriage-return for `M-xcommand' */
	    for (*d++ = 'x', append = '\r', s++;
		isspace((int) *s);
		s++)
		;
    }
#endif
    for (; (ch = *s++);)
    {
	if (ch == '\\')
	{
	    ch = *s++;
	    if (ch >= '0' && ch <= '7')		/* octal */
	    {
		num = ch - '0';
		for (i = 0; i < 2; i++, s++)
		{
		    ch = *s;
		    if (ch < '0' || ch > '7')
			break;
		    num = num * 8 + ch - '0';
		}
		ch = (char)num;
	    }
	    else if (ch == 'a')
		ch = C0_BEL;	/* bell */
	    else if (ch == 'b')
		ch = C0_BS; /* backspace */
	    else if (ch == 'E' || ch == 'e')
		ch = C0_ESC;	/* escape */
	    else if (ch == 'n')
		ch = '\n';  /* newline */
	    else if (ch == 'r')
		ch = '\r';  /* carriage-return */
	    else if (ch == 't')
		ch = C0_HT; /* tab */

	    else if (ch != '\\' && ch != '^' )
		*d++ = '\\'; /* Copy over backslash */
	}

	else if (ch == '^')
	{
	    ch = *s++;

	    if( ch == '?' )
		ch = 127;
	    else if( toupper(ch) >= 'A' && toupper(ch) <= 'Z' )
		ch = toupper(ch) - '@';
	    else
		*d++ = '^'; /* Copy over caret */
	}

	*d++ = ch;
    }

#if 0 /* Users can terminate their own stinking strings */
    /* ESC] is an XTerm escape sequence, must be terminated */
    if (*str == '\0' && str[1] == C0_ESC && str[2] == ']')
	append = CHAR_ST;

    /* add trailing character as required */
    if (append && d[-1] != append)
	*d++ = append;
#endif
    *d = '\0';

    return (d - str);
}