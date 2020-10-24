static void align_and_draw_text(int index, int align, int x, int y,
				const char *text)
{
    int lb, rb, wid, asc, desc;

    /*
     * Measure vertical string extents with respect to the same
     * string always...
     */
    gdk_string_extents(fe->fonts[i].font,
		       "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
		       &lb, &rb, &wid, &asc, &desc);
    if (align & ALIGN_VCENTRE)
	y += asc - (asc+desc)/2;
    else
	y += asc;

    /*
     * ... but horizontal extents with respect to the provided
     * string. This means that multiple pieces of text centred
     * on the same y-coordinate don't have different baselines.
     */
    gdk_string_extents(fe->fonts[i].font, text,
		       &lb, &rb, &wid, &asc, &desc);

    if (align & ALIGN_HCENTRE)
	x -= wid / 2;
    else if (align & ALIGN_HRIGHT)
	x -= wid;

    /*
     * Actually draw the text.
     */
    gdk_draw_string(fe->pixmap, fe->fonts[i].font, fe->gc, x, y, text);
}