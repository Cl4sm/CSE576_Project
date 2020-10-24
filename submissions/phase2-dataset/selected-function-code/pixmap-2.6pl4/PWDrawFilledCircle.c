			Position point_x, Position point_y, int value)
#else
void PWDrawFilledCircle(w, origin_x, origin_y, point_x, point_y, value)
     Widget w;
     Position origin_x, origin_y, point_x, point_y;
     int value;
#endif
{
  register Position i, j, delta;
  Dimension dx, dy;
  double radius;
  
  dx = abs(point_x - origin_x);
  dy = abs(point_y - origin_y);
  radius = sqrt((double) (dx * dx + dy * dy));
  for(j = origin_x - (Position) floor(radius); 
      j <= origin_x + (Position) floor(radius); j++)
    PWDrawPoint(w, j, origin_y, value);
  for(i = 1; i <= (Position) floor(radius); i++) {
    delta = sqrt(radius * radius - i * i);
    for(j = origin_x - delta; j <= origin_x + delta; j++) {
      PWDrawPoint(w, j, origin_y - i, value);
      PWDrawPoint(w, j, origin_y + i, value);
    }
  }
}
