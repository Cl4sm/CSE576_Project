void
menubar_dispatch(char *str)
{
#ifdef MENUBAR
    static menu_t  *BuildMenu = NULL;	/* the menu currently being built */
    int             n, cmd;
    char           *path, *name, *name2;

    if (menubar_visible() && ActiveMenu != NULL)
	menubar_expose();
    else
	ActiveMenu = NULL;

    cmd = *str;
    switch (cmd) {
    case '.':
    case '/':			/* absolute & relative path */
    case MENUITEM_BEG:		/* menuitem */
    /* add `+' prefix for these cases */
	cmd = '+';
	break;

    case '+':
    case '-':
	str++;			/* skip cmd character */
	break;

    case '<':
#if (MENUBAR_MAX > 1)
	if (CurrentBar == NULL)
	    break;
#endif				/* (MENUBAR_MAX > 1) */
	if (str[1] && str[2] == '>')	/* arrow commands */
	    menuarrow_add(str);
	break;

    case '[':			/* extended command */
	while (str[0] == '[') {
	    char           *next = (++str);	/* skip leading '[' */

	    if (str[0] == ':') {	/* [:command:] */
		do {
		    next++;
		    if ((next = strchr(next, ':')) == NULL)
			return;	/* parse error */
		}
		while (next[1] != ']');
	    /* remove and skip ':]' */
		*next = '\0';
		next += 2;
	    } else {
		if ((next = strchr(next, ']')) == NULL)
		    return;	/* parse error */
	    /* remove and skip ']' */
		*next = '\0';
		next++;
	    }

	    if (str[0] == ':') {
		int             saved;

	    /* try and dispatch it, regardless of read/write status */
		saved = menu_readonly;
		menu_readonly = 0;
		menubar_dispatch(str + 1);
		menu_readonly = saved;
	    }
	/* these ones don't require menu stacking */
	    else if (!strcmp(str, "clear")) {
		menubar_clear();
	    } else if (!strcmp(str, "done") || Str_match(str, "done:")) {
		menu_readonly = 1;
	    } else if (!strcmp(str, "show")) {
		map_menuBar(1);
		menu_readonly = 1;
	    } else if (!strcmp(str, "hide")) {
		map_menuBar(0);
		menu_readonly = 1;
	    } else if ((n = Str_match(str, "read:")) != 0) {
	    /* read in a menu from a file */
		str += n;
		menubar_read(str);
	    } else if ((n = Str_match(str, "title:")) != 0) {
		str += n;
		if (CurrentBar != NULL && !menu_readonly) {
		    if (*str) {
			name = REALLOC(CurrentBar->title, strlen(str) + 1);
			if (name != NULL) {
			    STRCPY(name, str);
			    CurrentBar->title = name;
			}
			menubar_expose();
		    } else {
			FREE(CurrentBar->title);
			CurrentBar->title = NULL;
		    }
		}
	    } else if ((n = Str_match(str, "pixmap:")) != 0) {
		str += n;
		xterm_seq(XTerm_Pixmap, str);
	    }
#if (MENUBAR_MAX > 1)
	    else if ((n = Str_match(str, "rm")) != 0) {
		str += n;
		switch (str[0]) {
		case ':':
		    str++;
		    menubar_remove(str);
		    break;

		case '\0':
		    menubar_remove(str);
		    break;

		case '*':
		    menubar_remove(str);
		    break;
		}
		menu_readonly = 1;
	    } else if ((n = Str_match(str, "menu")) != 0) {
		str += n;
		switch (str[0]) {
		case ':':
		    str++;
		/* add/access menuBar */
		    if (*str != '\0' && *str != '*')
			menubar_push(str);
		    break;
                default:
                    if (CurrentBar == NULL) {
                        menubar_push("default");
                    }
		}
                    

		if (CurrentBar != NULL)
		    menu_readonly = 0;	/* allow menu build commands */
	    } else if (!strcmp(str, "dump")) {
	    /* dump current menubars to a file */
		FILE           *fp;

	    /* enough space to hold the results */
		char            buffer[32];

		sprintf(buffer, "/tmp/" APL_SUBCLASS "-%u",
			(unsigned int)getpid());

		if ((fp = fopen(buffer, "wb")) != NULL) {
		    xterm_seq(XTerm_title, buffer);
		    menubar_dump(fp);
		    fclose(fp);
		}
	    } else if (!strcmp(str, "next")) {
		if (CurrentBar) {
		    CurrentBar = CurrentBar->next;
		    menu_readonly = 1;
		}
	    } else if (!strcmp(str, "prev")) {
		if (CurrentBar) {
		    CurrentBar = CurrentBar->prev;
		    menu_readonly = 1;
		}
	    } else if (!strcmp(str, "swap")) {
	    /* swap the top 2 menus */
		if (CurrentBar) {
		    bar_t          *prev = CurrentBar->prev;
		    bar_t          *next = CurrentBar->next;

		    prev->next = next;
		    next->prev = prev;

		    CurrentBar->next = prev;
		    CurrentBar->prev = prev->prev;

		    prev->prev->next = CurrentBar;
		    prev->prev = CurrentBar;

		    CurrentBar = prev;
		    menu_readonly = 1;
		}
	    }
#endif				/* (MENUBAR_MAX > 1) */
	    str = next;

	    BuildMenu = ActiveMenu = NULL;
	    menubar_expose();
#ifdef DEBUG_MENUBAR_STACKING
	    printf("menus are read%s\n", menu_readonly ? "only" : "/write");
#endif
	}
	return;
	break;
    }

#if (MENUBAR_MAX > 1)
    if (CurrentBar == NULL)
	return;
    if (menu_readonly) {
#ifdef DEBUG_MENUBAR_STACKING
	printf("menus are read%s\n", menu_readonly ? "only" : "/write");
#endif
	return;
    }
#endif				/* (MENUBAR_MAX > 1) */

    switch (cmd) {
    case '+':
    case '-':
	path = name = str;

	name2 = NULL;
    /* parse STR, allow spaces inside (name)  */
	if (path[0] != '\0') {
	    name = strchr(path, MENUITEM_BEG);
	    str = strchr(path, MENUITEM_END);
	    if (name != NULL || str != NULL) {
		if (name == NULL || str == NULL || str <= (name + 1)
		    || (name > path && name[-1] != '/')) {
		    print_error("menu error <%s>\n", path);
		    break;
		}
		if (str[1] == MENUITEM_BEG) {
		    name2 = (str + 2);
		    str = strchr(name2, MENUITEM_END);

		    if (str == NULL) {
			print_error("menu error <%s>\n", path);
			break;
		    }
		    name2[-2] = '\0';	/* remove prev MENUITEM_END */
		}
		if (name > path && name[-1] == '/')
		    name[-1] = '\0';

		*name++ = '\0';	/* delimit */
		*str++ = '\0';	/* delimit */

		while (isspace(*str))
		    str++;	/* skip space */
	    }
#ifdef DEBUG_MENU
	    printf("`%c' path = <%s>, name = <%s>, name2 = <%s>, action = <%s>\n",
		   cmd,
		   (path ? path : "(nil)"),
		   (name ? name : "(nil)"),
		   (name2 ? name2 : "(nil)"),
		   (str ? str : "(nil)")
		);
#endif
	}
    /* process the different commands */
	switch (cmd) {
	case '+':		/* add/replace existing menu or menuitem */
	    if (path[0] != '\0') {
		int             len;

		path = menu_find_base(&BuildMenu, path);
		len = strlen(path);

	    /* don't allow menus called `*' */
		if (path[0] == '*') {
		    menu_clear(BuildMenu);
		    break;
		} else if (len >= 2 && !strcmp((path + len - 2), "/*")) {
		    path[len - 2] = '\0';
		}
		if (path[0] != '\0')
		    BuildMenu = menu_add(BuildMenu, path);
	    }
	    if (name != NULL && name[0] != '\0') {
		if (!strcmp(name, SEPARATOR_NAME))
		    name = "";
		menuitem_add(BuildMenu, name, name2, str);
	    }
	    break;

	case '-':		/* delete menu entry */
	    if (!strcmp(path, "/*") && (name == NULL || name[0] == '\0')) {
		menubar_clear();
		BuildMenu = NULL;
		menubar_expose();
		break;
	    } else if (path[0] != '\0') {
		int             len;
		menu_t         *menu = BuildMenu;

		path = menu_find_base(&menu, path);
		len = strlen(path);

	    /* submenu called `*' clears all menu items */
		if (path[0] == '*') {
		    menu_clear(menu);
		    break;	/* done */
		} else if (len >= 2 && !strcmp(&path[len - 2], "/*")) {
		/* done */
		    break;
		} else if (path[0] != '\0') {
		    BuildMenu = NULL;
		    break;
		} else {
		    BuildMenu = menu;
		}
	    }
	    if (BuildMenu != NULL) {
		if (name == NULL || name[0] == '\0') {
		    BuildMenu = menu_delete(BuildMenu);
		} else {
		    menuitem_t     *item;

		    if (!strcmp(name, SEPARATOR_NAME))
			name = "";
		    item = menuitem_find(BuildMenu, name);

		    if (item != NULL && item->entry.type != MenuSubMenu) {
			menuitem_free(BuildMenu, item);

		    /* fix up the width */
			BuildMenu->width = 0;
			for (item = BuildMenu->head;
			     item != NULL;
			     item = item->next) {
			    if (BuildMenu->width < (item->len + item->len2))
				BuildMenu->width = (item->len + item->len2);
			}
		    }
		}
		menubar_expose();
	    }
	    break;
	}
	break;
    }
#endif
}
