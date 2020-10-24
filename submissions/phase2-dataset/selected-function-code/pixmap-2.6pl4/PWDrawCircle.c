void PWDrawCircle(Widget w, Position origin_x, Position origin_y, 
		  Position point_x, Position point_y, int value)
#else
void PWDrawCircle(w, origin_x, origin_y, point_x, point_y, value)
     Widget w;
     Position origin_x, origin_y, point_x, point_y;
     int value;
#endif
{
  register Position i, delta;
  Dimension dx, dy, half;
  double radius;
  
  dx = abs(point_x - origin_x);
  dy = abs(point_y - origin_y);
  radius = sqrt((double) (dx * dx + dy * dy));
  if (radius < 1.0) {
    PWDrawPoint(w, origin_x, origin_y, value);
  }
  else {
    PWDrawPoint(w, origin_x - (Position) floor(radius), origin_y, value);
    PWDrawPoint(w, origin_x + (Position) floor(radius), origin_y, value);
    PWDrawPoint(w, origin_x, origin_y - (Position) floor(radius), value);
    PWDrawPoint(w, origin_x, origin_y + (Position) floor(radius), value);
  }
  half = radius / sqrt(2.0);
  for(i = 1; i <= (Position)half; i++) {
    delta = sqrt(radius * radius - i * i);
    PWDrawPoint(w, origin_x - delta, origin_y - i, value);
    PWDrawPoint(w, origin_x - delta, origin_y + i, value);
    PWDrawPoint(w, origin_x + delta, origin_y - i, value);
    PWDrawPoint(w, origin_x + delta, origin_y + i, value);
    if (i != delta) {
      PWDrawPoint(w, origin_x - i, origin_y - delta, value);
      PWDrawPoint(w, origin_x - i, origin_y + delta, value);
      PWDrawPoint(w, origin_x + i, origin_y - delta, value);
      PWDrawPoint(w, origin_x + i, origin_y + delta, value);
    }
  }
}
