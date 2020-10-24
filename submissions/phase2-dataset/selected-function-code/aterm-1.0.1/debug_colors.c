debug_colors(void)
{
    int             color;
    char           *name[] =
    {"fg", "bg",
     "black", "red", "green", "yellow", "blue", "magenta", "cyan", "white"};

    fprintf(stderr, "Color ( ");
    if (rstyle & RS_RVid)
	fprintf(stderr, "rvid ");
    if (rstyle & RS_Bold)
	fprintf(stderr, "bold ");
    if (rstyle & RS_Blink)
	fprintf(stderr, "blink ");
    if (rstyle & RS_Uline)
	fprintf(stderr, "uline ");
    fprintf(stderr, "): ");

    color = GET_FGCOLOR(rstyle);
#ifndef NO_BRIGHTCOLOR
    if (color >= minBrightCOLOR && color <= maxBrightCOLOR) {
	color -= (minBrightCOLOR - minCOLOR);
	fprintf(stderr, "bright ");
    }
#endif
    fprintf(stderr, "%s on ", name[color]);

    color = GET_BGCOLOR(rstyle);
#ifndef NO_BRIGHTCOLOR
    if (color >= minBrightCOLOR && color <= maxBrightCOLOR) {
	color -= (minBrightCOLOR - minCOLOR);
	fprintf(stderr, "bright ");
    }
#endif
    fprintf(stderr, "%s\n", name[color]);
}
