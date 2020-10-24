char           *
menu_find_base(menu_t ** menu, char *path)
{
#ifdef MENUBAR
    menu_t         *m = NULL;
    menuitem_t     *item;

    assert(menu != NULL);
    assert(CurrentBar != NULL);

    if (path[0] == '\0')
	return path;

    if (strchr(path, '/') != NULL) {
	register char  *p = path;

	while ((p = strchr(p, '/')) != NULL) {
	    p++;
	    if (*p == '/')
		path = p;
	}
	if (path[0] == '/') {
	    path++;
	    *menu = NULL;
	}
	while ((p = strchr(path, '/')) != NULL) {
	    p[0] = '\0';
	    if (path[0] == '\0')
		return NULL;
	    if (!strcmp(path, DOT)) {
	    /* nothing to do */
	    } else if (!strcmp(path, DOTS)) {
		if (*menu != NULL)
		    *menu = (*menu)->parent;
	    } else {
		path = menu_find_base(menu, path);
		if (path[0] != '\0') {	/* not found */
		    p[0] = '/';	/* fix-up name again */
		    return path;
		}
	    }

	    path = (p + 1);
	}
    }
    if (!strcmp(path, DOTS)) {
	path += strlen(DOTS);
	if (*menu != NULL)
	    *menu = (*menu)->parent;
	return path;
    }
/* find this menu */
    if (*menu == NULL) {
	for (m = CurrentBar->tail; m != NULL; m = m->prev) {
	    if (!strcmp(path, m->name))
		break;
	}
    } else {
    /* find this menu */
	for (item = (*menu)->tail; item != NULL; item = item->prev) {
	    if (item->entry.type == MenuSubMenu &&
		!strcmp(path, (item->entry.submenu.menu)->name)) {
		m = (item->entry.submenu.menu);
		break;
	    }
	}
    }
    if (m != NULL) {
	*menu = m;
	path += strlen(path);
    }
    return path;
#else
    return NULL;
#endif
}
