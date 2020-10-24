menubar_clear(void)
{
#ifdef MENUBAR
    if (CurrentBar != NULL) {
	menu_t         *menu = CurrentBar->tail;

	while (menu != NULL) {
	    menu_t         *prev = menu->prev;

	    menu_delete(menu);
	    menu = prev;
	}
	CurrentBar->head = CurrentBar->tail = ActiveMenu = NULL;

	if (CurrentBar->title) {
	    FREE(CurrentBar->title);
	    CurrentBar->title = NULL;
	}
	menuarrow_free(0);	/* remove all arrow functions */
    }
    ActiveMenu = NULL;
#endif
}
