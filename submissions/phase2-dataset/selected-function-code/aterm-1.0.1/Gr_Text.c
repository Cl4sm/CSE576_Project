void
Gr_Text(grwin_t * grwin, grcmd_t * data)
{
#ifdef RXVT_GRAPHICS
    int             x, y, align;

    if (data->ncoords < 4 || data->text == NULL || *(data->text) == '\0')
	return;

    x = data->coords[0] * grwin->w / GRX_SCALE;
    y = data->coords[1] * grwin->h / GRX_SCALE;
    align = data->coords[2];

    if (align & RIGHT_TEXT)
	x -= XTextWidth(TermWin.font, data->text, data->coords[3]);
    else if (align & HCENTER_TEXT)
	x -= (XTextWidth(TermWin.font, data->text, data->coords[3]) >> 1);

    if (align & TOP_TEXT)
	y += TermWin.font->ascent;
    else if (align & BOTTOM_TEXT)
	y -= TermWin.font->descent;

    if (align & VCENTER_TEXT)
	y -= TermWin.font->descent
	     + ((TermWin.font->ascent + TermWin.font->descent) >> 1);
    if (align & VCAPS_CENTER_TEXT)
	y += (TermWin.font->ascent >> 1);

    XPMClearArea(Xdisplay, grwin->win, x, y, Width2Pixel(data->coords[3]),
		 Height2Pixel(1), 0);
    XDrawString(Xdisplay, grwin->win, TermWin.gc, x, y,
		data->text, data->coords[3]);
#endif
}
