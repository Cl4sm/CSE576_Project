void PWDrawRectangle(Widget w, Position from_x, Position from_y, 
		     Position to_x, Position to_y, int value)
#else
void PWDrawRectangle(w, from_x, from_y, to_x, to_y, value)
     Widget w;
     Position from_x, from_y, to_x, to_y;
     int value;
#endif
{
  register Position i;
  Dimension delta, width, height;
  
  QuerySwap(from_x, to_x);
  QuerySwap(from_y, to_y);
  
  width = to_x - from_x;
  height = to_y - from_y;
  
  delta = max(width, height);
  
  if (!QueryZero(width, height)) {
    for (i = 0; i < (Position)delta; i++) {
      if (i < (Position)width) {
	PWDrawPoint(w, from_x + i, from_y, value);
	PWDrawPoint(w, to_x - i, to_y, value);
      }
      if (i < (Position)height) {
	PWDrawPoint(w, from_x, to_y - i, value);
	PWDrawPoint(w, to_x, from_y + i, value);
      }
    }
  }
  else
    PWDrawLine(w, 
	       from_x, from_y, 
	       to_x, to_y, value);
}
