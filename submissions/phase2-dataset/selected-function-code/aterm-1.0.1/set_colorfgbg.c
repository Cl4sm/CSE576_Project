set_colorfgbg(void)
{
    unsigned int    i;
    static char     colorfgbg_env[] = "COLORFGBG=default;default;bg";
    char           *p;
    int             fg = -1, bg = -1;

    for (i = Color_Black; i <= Color_White; i++) 
	{
		if (PixColors[Color_fg] == PixColors[i]) {
	    	fg = (i - Color_Black);
	    	break;
		}
    }
    for (i = Color_Black; i <= Color_White; i++) 
	{
		if (PixColors[Color_bg] == PixColors[i]) {
	    	bg = (i - Color_Black);
	    	break;
		}
    }

    p = strchr(colorfgbg_env, '=');
    p++;
    if (fg >= 0)
	sprintf(p, "%d;", fg);
    else
	STRCPY(p, "default;");
    p = strchr(p, '\0');
    if (bg >= 0)
	sprintf(p,"%d", bg);
    else
	STRCPY(p, "default");

    putenv(colorfgbg_env);

    colorfgbg = DEFAULT_RSTYLE;
    for (i = minCOLOR; i <= maxCOLOR; i++) {
	if (PixColors[Color_fg] == PixColors[i]
# ifndef NO_BOLDUNDERLINE
	    && PixColors[Color_fg] == PixColors[Color_BD]
# endif				/* NO_BOLDUNDERLINE */
    /* if we wanted boldFont to have precedence */
# if 0				/* ifndef NO_BOLDFONT */
	    && TermWin.boldFont == NULL
# endif				/* NO_BOLDFONT */
	    )
	    colorfgbg = SET_FGCOLOR(colorfgbg, i);
	if (PixColors[Color_bg] == PixColors[i])
	    colorfgbg = SET_BGCOLOR(colorfgbg, i);
    }
}
