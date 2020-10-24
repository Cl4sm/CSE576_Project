menuarrow_add(char *string)
{
#ifdef MENUBAR
    int             i;
    unsigned        xtra_len;
    char           *p;

    struct {
	char           *str;
	int             len;
    } beg = {
	NULL, 0
    }, end = {
	NULL, 0
    }, *cur, parse[NARROWS];

    MEMSET(parse, 0, sizeof(parse));

/* printf ("add arrows = `%s'\n", string); */
    for (p = string; p != NULL && *p; string = p) {
	p = (string + 3);
    /* printf ("parsing at %s\n", string); */
	switch (string[1]) {
	case 'b':
	    cur = &beg;
	    break;
	case 'e':
	    cur = &end;
	    break;

	default:
	    i = menuarrow_find(string[1]);
	    if (i >= 0)
		cur = &(parse[i]);
	    else
		continue;	/* not found */
	    break;
	}

	string = p;
	cur->str = string;
	cur->len = 0;

	if (cur == &end) {
	    p = strchr(string, '\0');
	} else {
	    char           *next = string;

	    while (1) {
		p = strchr(next, '<');
		if (p != NULL) {
		    if (p[1] && p[2] == '>')
			break;
		/* parsed */
		} else {
		    if (beg.str == NULL)	/* no end needed */
			p = strchr(next, '\0');
		    break;
		}
		next = (p + 1);
	    }
	}

	if (p == NULL)
	    return;
	cur->len = (p - string);
    }

#ifdef DEBUG_MENUARROWS
    cur = &beg;
    printf("<b>(len %d) = %.*s\n",
	   cur->len, cur->len, (cur->str ? cur->str : ""));
    for (i = 0; i < NARROWS; i++) {
	cur = &(parse[i]);
	printf("<%c>(len %d) = %.*s\n",
	       Arrows[i].name,
	       cur->len,
	       cur->len,
	       (cur->str ? cur->str : ""));
    }
    cur = &end;
    printf("<e>(len %d) = %.*s\n",
	   cur->len, cur->len, (cur->str ? cur->str : ""));
#endif

    xtra_len = (beg.len + end.len);
    for (i = 0; i < NARROWS; i++) {
	if (xtra_len || parse[i].len)
	    menuarrow_free(Arrows[i].name);
    }

    for (i = 0; i < NARROWS; i++) {
	unsigned char  *str;
	unsigned int    len;

	if (!parse[i].len)
	    continue;

	str = MALLOC(parse[i].len + xtra_len + 1);
	if (str == NULL)
	    continue;

	len = 0;
	if (beg.len) {
	    STRNCPY(str + len, beg.str, beg.len);
	    len += beg.len;
	}
	STRNCPY(str + len, parse[i].str, parse[i].len);
	len += parse[i].len;

	if (end.len) {
	    STRNCPY(str + len, end.str, end.len);
	    len += end.len;
	}
	str[len] = '\0';

#ifdef DEBUG_MENUARROWS
	printf("<%c>(len %d) = %s\n", Arrows[i].name, len, str);
#endif
	if (action_type(&(CurrentBar->arrows[i]), str) < 0)
	    FREE(str);
    }
#endif
}
