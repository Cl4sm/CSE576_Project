void
rxvt_tabbar_button_release( rxvt_t *r, XButtonEvent *ev)
{
    int	    w, droppedTab;

    do	/* while( 0 ) */
    {
	if (
		ev->button != Button1		    /* Ignore everything except
						       left clicks */
		|| r->tabClicked == -1		    /* If we're not dragging a
						       tab then nothing to do */
		|| ev->y < 0
		|| ev->y > rxvt_tabbar_rheight( r ) /* If we drag off the
						       tabbar. (Coordinates in
						       ev are relative to the
						       tabbar window) */
	   )
	    break;

	/* Figure out where the user released the mouse */
	for (
		droppedTab = FVTAB(r), w=0;
		w < ev->x && droppedTab <= LVTAB(r);
		droppedTab++
	    )
	    w += TAB_WIDTH( droppedTab );

	rxvt_dbgmsg ((DBG_DEBUG, DBG_TABBAR, "Dragged tab %d to %d (%d, %d)\n", r->tabClicked, droppedTab - 1, ev->x, ev->y));

	/* Move active tab there */
	rxvt_tabbar_move_tab( r, droppedTab - 1 );
    } while( 0 );

    r->tabClicked = -1;
}