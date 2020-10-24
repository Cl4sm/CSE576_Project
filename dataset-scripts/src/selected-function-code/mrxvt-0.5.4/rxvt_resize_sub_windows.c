void
rxvt_resize_sub_windows (rxvt_t* r)
{
    register int    i;


    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "rxvt_resize_sub_windows\n"));

#ifdef HAVE_SCROLLBARS
    rxvt_scrollbar_resize(r);
#endif
#ifdef HAVE_MENUBAR
    rxvt_menubar_resize(r);
#endif
    rxvt_tabbar_resize (r);

    for (i = 0; i <= LTAB(r); i ++)
    {
	XMoveResizeWindow (r->Xdisplay, PVTS(r, i)->vt,
	    r->h->window_vt_x, r->h->window_vt_y,
	    VT_WIDTH(r), VT_HEIGHT(r));
    }
}