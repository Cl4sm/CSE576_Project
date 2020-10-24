void
Gr_DestroyWindow(grwin_t * grwin)
{
#ifdef RXVT_GRAPHICS
    grcmd_t        *cmd, *next;

    if (grwin == NULL)
	return;

    for (cmd = grwin->graphics; cmd; cmd = next) {
	next = cmd->next;
	free(cmd->coords);
	if (cmd->text != NULL)
	    free(cmd->text);
	free(cmd);
    }

    XDestroyWindow(Xdisplay, grwin->win);
    if (grwin->next != NULL)
	grwin->next->prev = grwin->prev;
    if (grwin->prev != NULL)
	grwin->prev->next = grwin->next;
    else
	gr_root = grwin->next;
    free(grwin);

    graphics_up--;
#endif
}
