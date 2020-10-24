void
scr_color(unsigned int color, unsigned int Intensity)
{
    if (color == restoreFG)
	color = Color_fg;
    else if (color == restoreBG)
	color = Color_bg;
    else {
	if (Xdepth <= 2) {	/* Monochrome - ignore colour changes */
	    switch (Intensity) {
	    case RS_Bold:
		color = Color_fg;
		break;
	    case RS_Blink:
		color = Color_bg;
		break;
	    }
	} else {
#ifndef NO_BRIGHTCOLOR
	    if ((rstyle & Intensity) && color >= minCOLOR && color <= maxCOLOR)
		color += (minBrightCOLOR - minCOLOR);
	    else if (color >= minBrightCOLOR && color <= maxBrightCOLOR) {
		if (rstyle & Intensity)
		    return;
		color -= (minBrightCOLOR - minCOLOR);
	    }
#endif
	}
    }
    switch (Intensity) {
    case RS_Bold:
	rstyle = SET_FGCOLOR(rstyle, color);
	break;
    case RS_Blink:
	rstyle = SET_BGCOLOR(rstyle, color);
	break;
    }
}
