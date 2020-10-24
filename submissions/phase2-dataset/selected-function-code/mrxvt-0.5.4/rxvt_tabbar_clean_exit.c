void
rxvt_tabbar_clean_exit (rxvt_t* r)
{
    register int    i;


    UNSET_WIN(r->tabBar.win);	/* destroyed by XDestroySubwindows */

    /* free resource strings */
    if (r->tabBar.rsfg)
	rxvt_free ((void*) r->h->rs[Rs_tabfg]);
    if (r->tabBar.rsbg)
	rxvt_free ((void*) r->h->rs[Rs_tabbg]);
    if (r->tabBar.rsifg)
	rxvt_free ((void*) r->h->rs[Rs_itabfg]);
    if (r->tabBar.rsibg)
	rxvt_free ((void*) r->h->rs[Rs_itabbg]);

    if (IS_GC(r->tabBar.gc))
    {
	XFreeGC (r->Xdisplay, r->tabBar.gc);
	UNSET_GC(r->tabBar.gc);
    }

    for (i = 0; i < NB_XPM; i ++)
    {
#ifdef HAVE_LIBXPM
	if (IS_PIXMAP(img_e[i]))
	{
	    XFreePixmap (r->Xdisplay, img_e[i]);
	    UNSET_PIXMAP(img_e[i]);
	}
	if (IS_PIXMAP(img_emask[i]))
	{
	    XFreePixmap (r->Xdisplay, img_emask[i]);
	    UNSET_PIXMAP(img_emask[i]);
	}
	if (IS_PIXMAP(img_d[i]))
	{
	    XFreePixmap (r->Xdisplay, img_d[i]);
	    UNSET_PIXMAP(img_d[i]);
	}
	if (IS_PIXMAP(img_dmask[i]))
	{
	    XFreePixmap (r->Xdisplay, img_dmask[i]);
	    UNSET_PIXMAP(img_dmask[i]);
	}
#else
	if (IS_PIXMAP(img[i]))
	    XFreePixmap (r->Xdisplay, img[i]);
#endif
	UNSET_PIXMAP(img[i]);
    }	/* for */
}