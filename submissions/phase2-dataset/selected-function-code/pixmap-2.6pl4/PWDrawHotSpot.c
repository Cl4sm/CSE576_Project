void PWDrawHotSpot(Widget w, Position x, Position y, int value)
#else
void PWDrawHotSpot(w, x, y, value)
    Widget w;
    Position x, y;
    int value;
#endif
{
    PixmapWidget PW = (PixmapWidget) w;
    
    if (QueryInPixmap(PW, x, y)) {
	if (QuerySet(PW->pixmap.hot.x, PW->pixmap.hot.y) &&
	    ((PW->pixmap.hot.x == x) && (PW->pixmap.hot.y == y))) {
	    if ((value == Clear) || (value == Invert)) {
		PWClearHotSpot(w);
	    }
	}
	else
	    if ((value == Set) || (value == Invert)) {
		PWClearHotSpot(w);
		DrawHotSpot(PW, x, y);
		PW->pixmap.hot.x = x;
		PW->pixmap.hot.y = y;
	    }
	
	if (value == Highlight)
	    HighlightHotSpot(PW, x, y); 
    }
}
