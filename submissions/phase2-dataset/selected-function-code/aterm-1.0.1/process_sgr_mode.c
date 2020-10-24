process_sgr_mode(unsigned int nargs, int arg[])
{
    unsigned int    i;

    if (nargs == 0) {
	scr_rendition(0, ~RS_None);
	return;
    }
    for (i = 0; i < nargs; i++)
	switch (arg[i]) {
	case 0:
	    scr_rendition(0, ~RS_None);
	    break;
	case 1:
	    scr_rendition(1, RS_Bold);
	    break;
	case 4:
	    scr_rendition(1, RS_Uline);
	    break;
	case 5:
	    scr_rendition(1, RS_Blink);
	    break;
	case 7:
	    scr_rendition(1, RS_RVid);
	    break;
	case 22:
	    scr_rendition(0, RS_Bold);
	    break;
	case 24:
	    scr_rendition(0, RS_Uline);
	    break;
	case 25:
	    scr_rendition(0, RS_Blink);
	    break;
	case 27:
	    scr_rendition(0, RS_RVid);
	    break;

	case 30:
	case 31:		/* set fg color */
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	    scr_color(minCOLOR + (arg[i] - 30), RS_Bold);
	    break;
	case 39:		/* default fg */
	    scr_color(restoreFG, RS_Bold);
	    break;

	case 40:
	case 41:		/* set bg color */
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
	    scr_color(minCOLOR + (arg[i] - 40), RS_Blink);
	    break;
	case 49:		/* default bg */
	    scr_color(restoreBG, RS_Blink);
	    break;
	}
}
