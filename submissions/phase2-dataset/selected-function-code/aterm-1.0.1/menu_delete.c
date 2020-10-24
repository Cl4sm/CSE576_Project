menu_t         *
menu_delete(menu_t * menu)
{
#ifdef MENUBAR
    menu_t         *parent = NULL, *prev, *next;
    menuitem_t     *item;

    assert(CurrentBar != NULL);

/* delete the entire menu */
    if (menu == NULL)
	return NULL;

    parent = menu->parent;

/* unlink MENU */
    prev = menu->prev;
    next = menu->next;
    if (prev != NULL)
	prev->next = next;
    if (next != NULL)
	next->prev = prev;

/* fix the index */
    if (parent == NULL) {
	const int       len = (menu->len + HSPACE);

	if (CurrentBar->tail == menu)
	    CurrentBar->tail = prev;
	if (CurrentBar->head == menu)
	    CurrentBar->head = next;

	for (next = menu->next; next != NULL; next = next->next)
	    next->x -= len;
    } else {
	for (item = parent->tail; item != NULL; item = item->prev) {
	    if (item->entry.type == MenuSubMenu &&
		item->entry.submenu.menu == menu) {
		item->entry.submenu.menu = NULL;
		menuitem_free(menu->parent, item);
		break;
	    }
	}
    }

    item = menu->tail;
    while (item != NULL) {
	menuitem_t     *p = item->prev;

	menuitem_free(menu, item);
	item = p;
    }

    if (menu->name != NULL)
	FREE(menu->name);
    FREE(menu);

    return parent;
#else
    return NULL;
#endif
}
