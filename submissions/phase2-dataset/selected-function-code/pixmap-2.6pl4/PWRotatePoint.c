void PWRotatePoint( w, x, y, direction )
    Widget                      w;
    Position                    *x;
    Position                    *y;
    enum RotateDirection        direction;
{
    PixmapWidget PW = (PixmapWidget) w;

    Position from_x, from_y, to_x, to_y;
    Position half_width, half_height, shift;
    Position tmp;

    if (PWQueryMarked(w)) {
      from_x = PW->pixmap.mark.from_x;
      from_y = PW->pixmap.mark.from_y;
      to_x   = PW->pixmap.mark.to_x;
      to_y   = PW->pixmap.mark.to_y;
    }
    else {
      from_x = 0;
      from_y = 0;
      to_x   = PW->pixmap.width - 1;
      to_y   = PW->pixmap.height - 1;
    }
    half_width = floor((to_x - from_x) / 2.0 + 0.5);
    half_height= floor((to_y - from_y ) / 2.0 + 0.5);
    shift      = min((Position)(to_x - from_x), (Position)(to_y - from_y )) % 2;

    *x -= half_width;
    *y -= half_height;
    if (*x >= 0) *x += shift;
    if (*y >= 0) *y += shift;
    tmp = *x;
    if (direction == Right) {
        *x = - *y;
        *y = tmp;
    } else {
        *x = *y;
        *y = - tmp;
    }
    if (*x > 0) *x -= shift;
    if (*y > 0) *y -= shift;
    *x += half_width;
    *y += half_height;
}
