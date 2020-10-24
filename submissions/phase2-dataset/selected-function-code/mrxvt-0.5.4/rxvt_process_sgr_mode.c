void
rxvt_process_sgr_mode(rxvt_t* r, int page, unsigned int nargs, const int *arg)
{
    unsigned int    i;
    short	    rendset;
    int		    rendstyle = 0;

    if (nargs == 0)
    {
	rxvt_scr_rendition(r, page, 0, ~RS_None);
	return;
    }
    for (i = 0; i < nargs; i++)
    {
	rendset = -1;
	switch (arg[i])
	{
	    case 0:
		rendset = 0, rendstyle = ~RS_None;
		break;
	    case 1:
		rendset = 1, rendstyle = RS_Bold;
		break;
	    case 4:
		rendset = 1, rendstyle = RS_Uline;
		break;
	    case 5:
		rendset = 1, rendstyle = RS_Blink;
		break;
	    case 7:
		rendset = 1, rendstyle = RS_RVid;
		break;
	    case 22:
		rendset = 0, rendstyle = RS_Bold;
		break;
	    case 24:
		rendset = 0, rendstyle = RS_Uline;
		break;
	    case 25:
		rendset = 0, rendstyle = RS_Blink;
		break;
	    case 27:
		rendset = 0, rendstyle = RS_RVid;
		break;
	}
	if (rendset != -1)
	{
	    rxvt_scr_rendition(r, page, rendset, rendstyle);
	    continue;	    /* for(;i;) */
	}

	switch (arg[i])
	{
	    case 30:
	    case 31:	    /* set fg color */
	    case 32:
	    case 33:
	    case 34:
	    case 35:
	    case 36:
	    case 37:
		rxvt_scr_color(r, page,
		    (unsigned int)(minCOLOR+(arg[i]-30)), Color_fg);
		break;
#ifdef TTY_256COLOR
	    case 38:
		if (nargs > i + 2 && arg[i + 1] == 5)
		{
		    rxvt_scr_color(r, page,
			(unsigned int)(minCOLOR+arg[i+2]), Color_fg);
		    i += 2;
		}
		break;
#endif
	    case 39:	    /* default fg */
		rxvt_scr_color(r, page, Color_fg, Color_fg);
		break;

	    case 40:
	    case 41:	    /* set bg color */
	    case 42:
	    case 43:
	    case 44:
	    case 45:
	    case 46:
	    case 47:
		rxvt_scr_color(r, page,
		    (unsigned int)(minCOLOR+(arg[i]-40)), Color_bg);
		break;
#ifdef TTY_256COLOR
	    case 48:
		if (nargs > i + 2 && arg[i + 1] == 5)
		{
		    rxvt_scr_color(r, page,
			(unsigned int)(minCOLOR+arg[i+2]), Color_bg);
		    i += 2;
		}
		break;
#endif
	    case 49:	    /* default bg */
		rxvt_scr_color(r, page, Color_bg, Color_bg);
		break;

#ifndef NO_BRIGHTCOLOR
	    case 90:
	    case 91:	    /* set bright fg color */
	    case 92:
	    case 93:
	    case 94:
	    case 95:
	    case 96:
	    case 97:
		rxvt_scr_color(r, page,
		    (unsigned int)(minBrightCOLOR+(arg[i]-90)), Color_fg);
		break;
	    case 100:
	    case 101:	    /* set bright bg color */
	    case 102:
	    case 103:
	    case 104:
	    case 105:
	    case 106:
	    case 107:
		rxvt_scr_color(r, page,
		    (unsigned int)(minBrightCOLOR+(arg[i]-100)), Color_bg);
		break;
#endif
	}
    }
}