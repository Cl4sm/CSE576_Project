menuitem_find(menu_t * menu, char *name)
{
#ifdef MENUBAR
    menuitem_t     *item;

    assert(name != NULL);
    assert(menu != NULL);

/* find the last item in the menu, this is good for separators */
    for (item = menu->tail; item != NULL; item = item->prev) {
	if (item->entry.type == MenuSubMenu) {
	    if (!strcmp(name, (item->entry.submenu.menu)->name))
		break;
	} else if ((isSeparator(name) && isSeparator(item->name)) ||
		   !strcmp(name, item->name))
	    break;
    }
    return item;
#else
    return NULL;
#endif
}
