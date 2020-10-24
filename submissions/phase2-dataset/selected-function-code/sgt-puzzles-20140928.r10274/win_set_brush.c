static void win_set_brush(frontend *fe, int colour)
{
    HBRUSH br;
    assert(fe->drawstatus != NOTHING);

    if (fe->drawstatus == PRINTING) {
	int hatch;
	float r, g, b;
	print_get_colour(fe->dr, colour, fe->printcolour, &hatch, &r, &g, &b);

	if (hatch < 0) {
	    br = CreateSolidBrush(RGB(r * 255, g * 255, b * 255));
	} else {
#ifdef _WIN32_WCE
	    /*
	     * This is only ever required during printing, and the
	     * PocketPC port doesn't support printing.
	     */
	    fatal("CreateHatchBrush not supported");
#else
	    br = CreateHatchBrush(hatch == HATCH_BACKSLASH ? HS_FDIAGONAL :
				  hatch == HATCH_SLASH ? HS_BDIAGONAL :
				  hatch == HATCH_HORIZ ? HS_HORIZONTAL :
				  hatch == HATCH_VERT ? HS_VERTICAL :
				  hatch == HATCH_PLUS ? HS_CROSS :
				  /* hatch == HATCH_X ? */ HS_DIAGCROSS,
				  RGB(0,0,0));
#endif
	}
    } else {
	br = fe->brushes[colour];
    }
    fe->oldbr = SelectObject(fe->hdc, br);
}