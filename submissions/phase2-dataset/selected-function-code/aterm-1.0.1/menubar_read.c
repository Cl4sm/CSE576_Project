menubar_read(const char *filename)
{
#ifdef MENUBAR
/* read in a menu from a file */
    FILE           *fp;
    char            buffer[256];
    char           *p, *tag = NULL;
    const char     *file;

    file = File_find(filename, ".menu");
    if (file == NULL || (fp = fopen(file, "rb")) == NULL)
	return;

#if (MENUBAR_MAX > 1)
/* semi-colon delimited */
    if ((tag = strchr(filename, ';')) != NULL) {
	tag++;
	if (*tag == '\0')
	    tag = NULL;
    }
#endif				/* (MENUBAR_MAX > 1) */
#ifdef DEBUG_MENU
    printf("[read:%s]\n", p);
    if (tag)
	printf("looking for [menu:%s]\n", tag);
#endif

    while ((p = fgets(buffer, sizeof(buffer), fp)) != NULL) {
	int             n;

	if ((n = Str_match(p, "[menu")) != 0) {
	    if (tag) {
	    /* looking for [menu:tag] */
		if (p[n] == ':' && p[n + 1] != ']') {
		    n++;
		    n += Str_match(p + n, tag);
		    if (p[n] == ']') {
#ifdef DEBUG_MENU
			printf("[menu:%s]\n", tag);
#endif
			break;
		    }
		}
	    } else if (p[n] == ':' || p[n] == ']')
		break;
	}
    }

/* found [menu], [menu:???] tag */
    while (p != NULL) {
	int             n;

#ifdef DEBUG_MENU
	printf("read line = %s\n", p);
#endif

    /* looking for [done:tag] or [done:] */
	if ((n = Str_match(p, "[done")) != 0) {
	    if (p[n] == ']') {
		menu_readonly = 1;
		break;
	    } else if (p[n] == ':') {
		n++;
		if (p[n] == ']') {
		    menu_readonly = 1;
		    break;
		} else if (tag) {
		    n += Str_match(p + n, tag);
		    if (p[n] == ']') {
#ifdef DEBUG_MENU
			printf("[done:%s]\n", tag);
#endif
			menu_readonly = 1;
			break;
		    }
		} else {
		/* what? ... skip this line */
		    p[0] = COMMENT_CHAR;
		}
	    }
	}
    /*
     * remove leading/trailing space
     * and strip-off leading/trailing quotes
     * skip blank or comment lines
     */
	p = Str_trim(p);
	if (p != NULL && *p && *p != '#') {
	    menu_readonly = 0;	/* if case we read another file */
	    menubar_dispatch(p);
	}
    /* get another line */
	p = fgets(buffer, sizeof(buffer), fp);
    }

    fclose(fp);
#endif
}
