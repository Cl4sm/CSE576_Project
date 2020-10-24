int
rxvt_set_opacity (rxvt_t* r)
{
    int		    k;
    unsigned int    n;
    Window	    wintree[PARENT_NUMBER];
    Window	    root;
    Window*	    list;
    CARD32	    opacity;


    /* do not set opacity */
    if (IS_NULL(r->h->rs[Rs_opacity]) ||
	NOT_ATOM(r->h->xa[XA_NET_WM_WINDOW_OPACITY]))
	return 0;

#ifdef TRANSPARENT
    /* Override pseudo-transparent in case */
    if (ISSET_OPTION(r, Opt_transparent))
	UNSET_OPTION(r, Opt_transparent);
    XSetWindowBackground(r->Xdisplay, r->TermWin.parent, VTBG(r,0) );
#endif

    opacity = (CARD32) (r->TermWin.opacity * (0xffffffff / 100));

    /*
    ** look for parent tree of top level window because the window
    ** manager may embed our window into some frame windows
    */
    wintree[0] = r->TermWin.parent;
    for (k = 1; k < PARENT_NUMBER; k++)
    {
	XQueryTree (r->Xdisplay, wintree[k-1], &root,
	    &(wintree[k]), &list, &n);
	XFree (list);
	if (wintree[k] == XROOT)
	    break;
    }

    /* Set opacity for all windows */
    if (k != PARENT_NUMBER)
    {
	while (k-- > 0)
	    XChangeProperty (r->Xdisplay, wintree[k],
		r->h->xa[XA_NET_WM_WINDOW_OPACITY], XA_CARDINAL, 32,
		PropModeReplace, (unsigned char*) &opacity, 1L);
    }

    XSync(r->Xdisplay, False);

    return 1;
}