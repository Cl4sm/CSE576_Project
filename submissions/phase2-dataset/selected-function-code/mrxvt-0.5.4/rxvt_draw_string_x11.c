 */
/* EXTPROTO */
void
rxvt_draw_string_x11 (rxvt_t* r, Window win, GC gc, Region refreshRegion,
	int x, int y, char* str, int len, int (*draw_string)())
{
# ifdef TEXT_SHADOW
    while (r->h->rs[Rs_textShadow] && SHADOW_NONE != r->TermWin.shadow_mode)
    {
	int	sx, sy;
	XGCValues   gcvalue;

	int	    (*xtextextents)();
	int	    unused_dir, ascent, descent;
	XCharStruct charstruct;
	GContext    gid = XGContextFromGC( gc );
	XFontStruct *font = XQueryFont( r->Xdisplay, gid);

	if( font == NULL ) break;
	rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "handling text shadow for %s (%d)\n", str, len));

	/*
	 * Save the old GC values foreground.
	 */
	XGetGCValues (r->Xdisplay, gc,
		GCForeground | GCBackground | GCFillStyle, &gcvalue);

	/*
	 * Get the bounding box of the rectangle we would draw, and clip to it.
	 */
	if( draw_string == XDrawImageString || draw_string == XDrawString)
	    xtextextents = XTextExtents;
	else if ( draw_string == XDrawImageString16 ||
		draw_string == XDrawString16)
	    xtextextents = XTextExtents16;
	else
		assert(0); /* Shouldn't happen */

	xtextextents( font, str, len,
		&unused_dir, &ascent, &descent, &charstruct);

	/*
	 * If we're using XDrawImageString, then when we draw the actual text,
	 * the shadow will be erased. Clear the rectangle ourselves, and change
	 * to XDrawString.
	 */
	if( draw_string == XDrawImageString ||
		draw_string == XDrawImageString16)
	{
	    XSetForeground( r->Xdisplay, gc, gcvalue.background);
	    XSetFillStyle( r->Xdisplay, gc, FillSolid);
	    XFillRectangle( r->Xdisplay, win, gc,
		    x, y - font->ascent,
		    charstruct.width, font->ascent + font->descent);

	    if( draw_string == XDrawImageString )
		draw_string = XDrawString;
	    else draw_string = XDrawString16;
	}


	/*
	 * Restrict output to the above bounding box.
	 */
	rxvt_set_clipping( r, NULL, gc, refreshRegion,
		x, y - font->ascent,
		charstruct.width, font->ascent + font->descent,
		&sx, &sy);

	/*
	 * Draw the shadow at the appropriate offset.
	 */
	XSetForeground (r->Xdisplay, gc, r->TermWin.shadow);
	draw_string (r->Xdisplay, win, gc, x+sx, y+sy, str, len);

	/*
	 * Restore old GC values.
	 */
	XChangeGC( r->Xdisplay, gc,
		GCForeground | GCBackground | GCFillStyle,
		&gcvalue);

	/*
	 * Unclip drawing for remaining drawing.
	 */
	rxvt_free_clipping (r, NULL, gc, refreshRegion);

	XFreeFontInfo( NULL, font, 1);
	break;
    }
# endif	/* TEXT_SHADOW */

    rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "output entire string: %s\n", str));