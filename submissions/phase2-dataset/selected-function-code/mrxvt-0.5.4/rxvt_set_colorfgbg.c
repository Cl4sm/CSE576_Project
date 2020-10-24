void
rxvt_set_colorfgbg(rxvt_t *r)
{
    unsigned int    i;
    const char	 *xpmb = "\0";
    char	    fstr[sizeof("default") + 1], bstr[sizeof("default") + 1];

    r->h->env_colorfgbg = rxvt_malloc(sizeof("COLORFGBG=default;default;bg")
			+ 1);
    STRCPY(fstr, "default");
    STRCPY(bstr, "default");
    for (i = Color_Black; i <= Color_White; i++)
	if (r->pixColorsFocus[Color_fg] == r->pixColorsFocus[i])
	{
	    sprintf(fstr, "%d", (i - Color_Black));
	    break;
	}
    for (i = Color_Black; i <= Color_White; i++)
	if (r->pixColorsFocus[Color_bg] == r->pixColorsFocus[i])
	{
	    sprintf(bstr, "%d", (i - Color_Black));
#ifdef BACKGROUND_IMAGE
	    xpmb = "default;";
#endif
	    break;
	}
    sprintf(r->h->env_colorfgbg, "COLORFGBG=%s;%s%s", fstr, xpmb, bstr);
    putenv(r->h->env_colorfgbg);

#ifndef NO_BRIGHTCOLOR
    r->h->colorfgbg = DEFAULT_RSTYLE;
    for (i = minCOLOR; i <= maxCOLOR; i++)
    {
	if (
		r->pixColorsFocus[Color_fg] == r->pixColorsFocus[i]
# ifndef NO_BOLD_UNDERLINE_REVERSE
		&& r->pixColorsFocus[Color_fg] == r->pixColorsFocus[Color_BD]
# endif
		/* if we wanted boldFont to have precedence */
# if 0	/* was ifndef NO_BOLDFONT */
		&& IS_NULL(r->TermWin.bfont)
# endif	
	   )
	    r->h->colorfgbg = SET_FGCOLOR(r->h->colorfgbg, i);
	if (r->pixColorsFocus[Color_bg] == r->pixColorsFocus[i])
	    r->h->colorfgbg = SET_BGCOLOR(r->h->colorfgbg, i);
    }
#endif	/* NO_BRIGHTCOLOR */
}