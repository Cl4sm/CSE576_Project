static void win_set_pen(frontend *fe, int colour, int thin)
{
    HPEN pen;
    assert(fe->drawstatus != NOTHING);

    if (fe->drawstatus == PRINTING) {
	int hatch;
	float r, g, b;
	int width = thin ? 0 : fe->linewidth;

	if (fe->linedotted)
	    width = 0;

	print_get_colour(fe->dr, colour, fe->printcolour, &hatch, &r, &g, &b);
	/*
	 * Stroking in hatched colours is not permitted.
	 */
	assert(hatch < 0);
	pen = CreatePen(fe->linedotted ? PS_DOT : PS_SOLID,
			width, RGB(r * 255, g * 255, b * 255));
    } else {
	pen = fe->pens[colour];
    }
    fe->oldpen = SelectObject(fe->hdc, pen);
}