resize(void)
{
    szHint.base_width = (2 * TermWin_internalBorder);
    szHint.base_height = (2 * TermWin_internalBorder);

    szHint.base_width += (scrollbar_visible() ? (SB_WIDTH + 2 * sb_shadow) : 0);
    { /* ONLYIF MENUBAR */
	szHint.base_height += (menubar_visible() ? menuBar_TotalHeight() : 0);
    }

    szHint.min_width = szHint.base_width + szHint.width_inc;
    szHint.min_height = szHint.base_height + szHint.height_inc;

    szHint.width = szHint.base_width + TermWin.width;
    szHint.height = szHint.base_height + TermWin.height;

    szHint.flags = PMinSize | PResizeInc | PBaseSize | PWinGravity;

    XSetWMNormalHints(Xdisplay, TermWin.parent, &szHint);
    XResizeWindow(Xdisplay, TermWin.parent, szHint.width, szHint.height);

    resize_subwindows(szHint.width, szHint.height);
    scr_clear();

}
