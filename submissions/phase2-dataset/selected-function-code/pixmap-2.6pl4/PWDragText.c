void PWDragText(Widget w, Position at_x, Position at_y, int value)
#else
void PWDragText(w, at_x, at_y, value)
     Widget	w;
     Position	at_x, at_y;	/* text origin */
     int        value;		/* unused */
#endif
{
/*
  PixmapWidget PW = (PixmapWidget) w;

  calculate text extent 
  translate from text origin to upper left of text extent 
  draw rectangle from upper left to lower right of text extent

  Dimension width  = PW->pixmap.text_rbearing - PW->pixmap.text_lbearing;
  Dimension height = PW->pixmap.text_ascent + PW->pixmap.text_descent;
  Position x = at_x + PW->pixmap.text_lbearing;
  Position y = at_y - PW->pixmap.text_ascent + 1;
  PWDrawRectangle(w, 
		  x, y,
                  x + width, y + height,
		  value);
*/

  PWDrawText(w, at_x, at_y, value);
}
