void
menuitem_free(menu_t * menu, menuitem_t * item)
{
#ifdef MENUBAR
/* disconnect */
    menuitem_t     *prev, *next;

    assert(menu != NULL);

    prev = item->prev;
    next = item->next;
    if (prev != NULL)
	prev->next = next;
    if (next != NULL)
	next->prev = prev;

/* new head, tail */
    if (menu->tail == item)
	menu->tail = prev;
    if (menu->head == item)
	menu->head = next;

    switch (item->entry.type) {
    case MenuAction:
    case MenuTerminalAction:
	FREE(item->entry.action.str);
	break;
    case MenuSubMenu:
	(void)menu_delete(item->entry.submenu.menu);
	break;
    }
    if (item->name != NULL)
	FREE(item->name);
    if (item->name2 != NULL)
	FREE(item->name2);
    FREE(item);
#endif
}
