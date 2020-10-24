static void
init_tabbar (rxvt_t* r)
{
    r->tabBar.state = 0;    /* not mapped yet */

    LTAB(r) = -1;   /* the last tab */
    r->tabBar.atab = 0;	/* the active tab */
    FVTAB(r) = 0;   /* first visiable tab */
    LVTAB(r) = 0;   /* last visiable tab */
    r->tabBar.ptab = 0;	    /* previous active tab */

    /* Make sure that font has been initialized */
#ifdef XFT_SUPPORT
    if (ISSET_OPTION (r, Opt_xft))
	assert (NULL != r->TermWin.xftfont);
    else
#endif
    assert (NULL != r->TermWin.font);
    assert (r->TermWin.FHEIGHT > 0);

    /* resource string are static, needn't to free */
    r->tabBar.rsfg =
    r->tabBar.rsbg =
    r->tabBar.rsifg =
    r->tabBar.rsibg = 0;
}