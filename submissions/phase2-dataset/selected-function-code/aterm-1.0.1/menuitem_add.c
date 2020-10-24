menuitem_add(menu_t * menu, char *name, char *name2, char *action)
{
#ifdef MENUBAR
    menuitem_t     *item;
    unsigned int    len;

    assert(name != NULL);
    assert(action != NULL);

    if (menu == NULL)
	return NULL;

    if (isSeparator(name)) {
    /* add separator, no action */
	name = "";
	action = "";
    } else {
    /*
     * add/replace existing menu item
     */
	item = menuitem_find(menu, name);
	if (item != NULL) {
	    if (item->name2 != NULL && name2 != NULL) {
		FREE(item->name2);
		item->len2 = 0;
		item->name2 = NULL;
	    }
	    switch (item->entry.type) {
	    case MenuAction:
	    case MenuTerminalAction:
		FREE(item->entry.action.str);
		item->entry.action.str = NULL;
		break;
	    }
	    goto Item_Found;
	}
    }
/* allocate a new itemect */
    if ((item = (menuitem_t *)MALLOC(sizeof(menuitem_t))) == NULL)
	return NULL;

    item->len2 = 0;
    item->name2 = NULL;

    len = strlen(name);
    item->name = MALLOC(len + 1);
    if (item->name != NULL) {
	STRCPY(item->name, name);
	if (name[0] == '.' && name[1] != '.')
	    len = 0;		/* hidden menu name */
    } else {
	FREE(item);
	return NULL;
    }
    item->len = len;

/* add to tail of list */
    item->prev = menu->tail;
    item->next = NULL;

    if (menu->tail != NULL)
	(menu->tail)->next = item;
    menu->tail = item;
/* fix head */
    if (menu->head == NULL)
	menu->head = item;

/*
 * add action
 */
  Item_Found:
    if (name2 != NULL && item->name2 == NULL) {
	len = strlen(name2);
	if (len == 0 || (item->name2 = MALLOC(len + 1)) == NULL) {
	    len = 0;
	    item->name2 = NULL;
	} else {
	    STRCPY(item->name2, name2);
	}
	item->len2 = len;
    }
    item->entry.type = MenuLabel;
    len = strlen(action);

    if (len == 0 && item->name2 != NULL) {
	action = item->name2;
	len = item->len2;
    }
    if (len) {
	unsigned char  *str = MALLOC(len + 1);

	if (str == NULL) {
	    menuitem_free(menu, item);
	    return NULL;
	}
	STRCPY(str, action);

	if (action_type(&(item->entry.action), str) < 0)
	    FREE(str);
    }
/* new item and a possible increase in width */
    if (menu->width < (item->len + item->len2))
	menu->width = (item->len + item->len2);

    return item;
#else
    return NULL;
#endif
}
