unsigned int
rxvt_tab_width (rxvt_t *r, const char *str)
{
#ifdef XFT_SUPPORT
    if ( ISSET_OPTION (r, Opt_xft) && r->TermWin.xftpfont)
    {
	/*
	 * With a proportionally spaced font defined, let's try and make the
	 * tabs look like firefox. All tabs have the same width. The more tabs
	 * there are, the narrower the width becomes. The width does not depend
	 * on the tab title.
	 */
	if( LTAB(r) >= 0 )
	{
	    int twidth = (TAB_SPACE - TAB_BORDER)
			    / min( LTAB(r) + 1, r->TermWin.minVisibleTabs )
			    - TAB_BORDER;
	    return min( twidth, MAX_TAB_PIXEL_WIDTH);
	}
	else return MAX_TAB_PIXEL_WIDTH;
    }
    else
#endif
    {
	int	    len;
	uint16_t    maxw = r->TermWin.maxTabWidth;

	assert (str);
	len = STRLEN (str);
	if (len > maxw)
	    len = maxw;
#ifdef XFT_SUPPORT
	if (ISSET_OPTION (r, Opt_xft) && (NULL != r->tabBar.xftwin))
	{
	    return (2 * TXT_XOFF + Width2Pixel(len));
	}
	else
#endif	/* XFT_SUPPORT */
	return (2 * TXT_XOFF + XTextWidth (r->TermWin.font, str, len));
    }
}