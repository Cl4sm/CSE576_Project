static void win_draw_rect(void *handle, int x, int y, int w, int h, int colour)
{
    frontend *fe = (frontend *)handle;
    POINT p, q;

    if (fe->drawstatus == NOTHING)
	return;

    if (fe->drawstatus == DRAWING && w == 1 && h == 1) {
	/*
	 * Rectangle() appears to get uppity if asked to draw a 1x1
	 * rectangle, presumably on the grounds that that's beneath
	 * its dignity and you ought to be using SetPixel instead.
	 * So I will.
	 */
	SetPixel(fe->hdc, x, y, fe->colours[colour]);
    } else {
	win_set_brush(fe, colour);
	win_set_pen(fe, colour, TRUE);
	p = win_transform_point(fe, x, y);
	q = win_transform_point(fe, x+w, y+h);
	Rectangle(fe->hdc, p.x, p.y, q.x, q.y);
	win_reset_brush(fe);
	win_reset_pen(fe);
    }
}