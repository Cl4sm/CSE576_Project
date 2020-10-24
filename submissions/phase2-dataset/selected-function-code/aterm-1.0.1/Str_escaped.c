Str_escaped(char *str)
{
    register char  *p = str;
    int             i = 0, len, n, append = 0;

/* use 'i' to increment through destination and p through source */

    if (str == NULL || (len = strlen(str)) == 0)
	return 0;

/* Emacs convenience, replace leading `M-..' with `\E..' */
    if ((n = Str_match(p, "M-")) != 0) {
	str[i++] = '\033';	/* destination */
	len--;
	p += n;
	if (toupper(*p) == 'X') {
	/* append carriage-return for `M-xcommand' */
	    append = '\r';
	    str[i++] = 'x';	/* destination */
	    p++;
	    while (isspace(*p)) {
		p++;
		len--;
	    }
	}
    }
    for ( /*nil */ ; i < len; i++) {
	register char   ch = *p++;

	if (ch == '\\') {
	    ch = *p;
	    if (ch >= '0' && ch <= '7') {	/* octal */
		int             j, num = 0;

		for (j = 0; j < 3 && (ch >= '0' && ch <= '7'); j++) {
		    num = num * 010 + (ch - '0');
		    p++;
		    len--;
		    ch = *p;
		}
		ch = (unsigned char)num;
	    } else {
		p++;
		len--;
		switch (ch) {
		case 'a':
		    ch = 007;
		    break;	/* bell */
		case 'b':
		    ch = '\b';
		    break;	/* backspace */
		case 'E':
		case 'e':
		    ch = 033;
		    break;	/* escape */
		case 'n':
		    ch = '\n';
		    break;	/* newline */
		case 'r':
		    ch = '\r';
		    break;	/* carriage-return */
		case 't':
		    ch = '\t';
		    break;	/* tab */
		}
	    }
	} else if (ch == '^') {
	    ch = *p;
	    p++;
	    len--;
	    ch = toupper(ch);
	    ch = (ch == '?' ? 127 : (ch - '@'));
	}
	str[i] = ch;
    }

/* ESC] is an XTerm escape sequence, must be ^G terminated */
    if (str[0] == '\0' && str[1] == '\033' && str[2] == ']')
	append = 007;

/* add trailing character as required */
    if (append && str[len - 1] != append)
	str[len++] = append;

    str[len] = '\0';

    return len;
}
