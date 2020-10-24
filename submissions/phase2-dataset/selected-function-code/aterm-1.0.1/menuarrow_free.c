void
menuarrow_free(char name)
{
#ifdef MENUBAR
    int             i;

    if (name) {
	i = menuarrow_find(name);
	if (i >= 0) {
	    action_t       *act = &(CurrentBar->arrows[i]);

	    switch (act->type) {
	    case MenuAction:
	    case MenuTerminalAction:
		FREE(act->str);
		act->str = NULL;
		act->len = 0;
		break;
	    }
	    act->type = MenuLabel;
	}
    } else {
	for (i = 0; i < NARROWS; i++)
	    menuarrow_free(Arrows[i].name);
    }
#endif
}
