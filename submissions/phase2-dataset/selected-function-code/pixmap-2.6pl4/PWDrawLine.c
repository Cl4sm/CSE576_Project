void PWDrawLine(Widget w, Position from_x, Position from_y, 
		Position to_x, Position to_y, int value)
#else
void PWDrawLine(w, from_x, from_y, to_x, to_y, value)
     Widget  w;
     Position from_x, from_y, to_x, to_y;
     int value;
#endif
{
  Position i;
  register double x, y;
  double dx, dy, delta;
  
  dx = to_x - from_x;
  dy = to_y - from_y;
  x = from_x + 0.5;
  y = from_y + 0.5;
  delta = max(abs(dx), abs(dy));
  if (delta > 0) {
    dx /= delta;
    dy /= delta;
    for(i = 0; i <= delta; i++) {
      PWDrawPoint(w, (Position) x, (Position) y, value);
      x += dx;
      y += dy;
    }
  }
  else
    PWDrawPoint(w, from_x, from_y, value);
}
