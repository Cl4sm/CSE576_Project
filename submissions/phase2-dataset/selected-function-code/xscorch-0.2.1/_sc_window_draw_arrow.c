                                  int size, bool right) {
/* sc_window_draw_arrow
   Draws an arrow on the screen whose RIGHT coordinate of the mainline
   is at (x, y), and whose mainline is <size> pixels long.  The <right>
   flag is true iff the arrow cap should be drawn on the right-hand
   side of the arrow.  The coordinates MUST be real coordinates.  */

   /* Draw the arrow mainline */
   gdk_draw_line(buffer, gc, x, y, x - size, y);

   /* Draw the arrow cap */
   if(right) {
      gdk_draw_line(buffer, gc, x, y,
                    x - SC_WIND_ARROW_SIZE, y - SC_WIND_ARROW_SIZE);
      gdk_draw_line(buffer, gc, x, y,
                    x - SC_WIND_ARROW_SIZE, y + SC_WIND_ARROW_SIZE);
   } else {
      gdk_draw_line(buffer, gc, x - size, y, 
                    x - size + SC_WIND_ARROW_SIZE, y - SC_WIND_ARROW_SIZE);
      gdk_draw_line(buffer, gc, x - size, y, 
                    x - size + SC_WIND_ARROW_SIZE, y + SC_WIND_ARROW_SIZE);
   }

}
