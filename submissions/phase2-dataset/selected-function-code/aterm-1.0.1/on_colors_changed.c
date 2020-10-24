void
on_colors_changed(int idx)
{
    colors_changed = 1 ;
    if ( idx == Color_bg &&
#ifdef BACKGROUND_IMAGE 
		 TermWin.background.trgPixmapSet == 0 &&
#endif
		 !(Options & Opt_transparent))
    {
		XSetWindowBackground(Xdisplay, TermWin.vt, PixColors[Color_bg]);
	} else if (idx == Color_tint) {
		XColor tint;

		tint.pixel = PixColors[Color_tint];
		XQueryColor(Xdisplay, Xcmap, &tint);
		if (TermWin.background.Shading.tintColor.red == tint.red &&
				TermWin.background.Shading.tintColor.green == tint.green &&
				TermWin.background.Shading.tintColor.blue == tint.blue) {
			return;
		}
		TermWin.background.Shading.tintColor = tint;
		RenderPixmap(0);
	}


/* handle Color_BD, scrollbar background, etc. */

    set_colorfgbg();
    set_cursor_color();
/* the only reasonable way to enforce a clean update : */
    scr_clear();
    scr_touch();
}
