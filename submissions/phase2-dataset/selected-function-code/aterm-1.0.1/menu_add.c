menu_t         *
menu_add(menu_t * parent, char *path)
{
#ifdef MENUBAR
    menu_t         *menu;

    assert(CurrentBar != NULL);

    if (strchr(path, '/') != NULL) {
	register char  *p;

	if (path[0] == '/') {
	/* shouldn't happen */
	    path++;
	    parent = NULL;
	}
	while ((p = strchr(path, '/')) != NULL) {
	    p[0] = '\0';
	    if (path[0] == '\0')
		return NULL;

	    parent = menu_add(parent, path);
	    path = (p + 1);
	}
    }
    if (!strcmp(path, DOTS))
	return (parent != NULL ? parent->parent : parent);

    if (!strcmp(path, DOT) || path[0] == '\0')
	return parent;

/* allocate a new menu */
    if ((menu = (menu_t *)MALLOC(sizeof(menu_t))) == NULL)
	return parent;

    menu->width = 0;
    menu->parent = parent;
    menu->len = strlen(path);
    menu->name = MALLOC((menu->len + 1));
    if (menu->name == NULL) {
	FREE(menu);
	return parent;
    }
    STRCPY(menu->name, path);

/* initialize head/tail */
    menu->head = menu->tail = NULL;
    menu->prev = menu->next = NULL;

    menu->win = None;
    menu->x = menu->y = menu->w = menu->h = 0;
    menu->item = NULL;

/* add to tail of list */
    if (parent == NULL) {
	menu->prev = CurrentBar->tail;
	if (CurrentBar->tail != NULL)
	    CurrentBar->tail->next = menu;
	CurrentBar->tail = menu;
	if (CurrentBar->head == NULL)
	    CurrentBar->head = menu;	/* fix head */
	if (menu->prev)
	    menu->x = (menu->prev->x + menu->prev->len + HSPACE);
    } else {
	menuitem_t     *item;

	item = menuitem_add(parent, path, "", "");
	if (item == NULL) {
	    FREE(menu);
	    return parent;
	}
	assert(item->entry.type == MenuLabel);
	item->entry.type = MenuSubMenu;
	item->entry.submenu.menu = menu;
    }

    return menu;
#else
    return NULL;
#endif
}
