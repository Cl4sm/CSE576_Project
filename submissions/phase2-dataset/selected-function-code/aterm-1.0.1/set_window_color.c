void
set_window_color(int idx, const char *color)
{
    const char     *const msg = "can't load color \"%s\"";
    XColor          xcol;
    int             i;

    if (color == NULL || *color == '\0')
	return;

/* handle color aliases */
    if (isdigit(*color)) {
	i = atoi(color);
	if (i >= 8 && i <= 15) {	/* bright colors */
	    i -= 8;
# ifndef NO_BRIGHTCOLOR
	    PixColorsFocused[idx] = PixColorsFocused[minBrightCOLOR + i];
	    goto Done;
# endif
	}
	if (i >= 0 && i <= 7) {	/* normal colors */
	    PixColorsFocused[idx] = PixColorsFocused[minCOLOR + i];
	    goto Done;
	}
    }
    if (!XParseColor(Xdisplay, Xcmap, color, &xcol) ||
	!XAllocColor(Xdisplay, Xcmap, &xcol)) {
	print_error(msg, color);
	return;
    }
/* XStoreColor (Xdisplay, Xcmap, XColor*); */

/*
 * FIXME: should free colors here, but no idea how to do it so instead,
 * so just keep gobbling up the colormap
 */
# if 0
    for (i = Color_Black; i <= Color_White; i++)
	if (PixColorsFocused[idx] == PixColorsFocused[i])
	    break;
    if (i > Color_White) {
    /* fprintf (stderr, "XFreeColors: PixColors [%d] = %lu\n", idx, PixColors [idx]); */
	XFreeColors(Xdisplay, Xcmap, (PixColorsFocused + idx), 1,
		    DisplayPlanes(Xdisplay, Xscreen));
    }
# endif

    PixColorsFocused[idx] = xcol.pixel;

/* XSetWindowAttributes attr; */
/* Cursor cursor; */
  Done:
#ifdef OFF_FOCUS_FADING
    PixColorsUnFocused[idx] = fade_color(PixColorsFocused[idx]);
#endif
    on_colors_changed(idx);
}
