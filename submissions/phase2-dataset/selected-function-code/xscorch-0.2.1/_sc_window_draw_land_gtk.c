static inline void _sc_window_draw_land_gtk(sc_window_gtk *w, int x1, int y1, int x2, int y2) {
/* sc_window_draw_land_gtk
   This is a wrapper for the above two functions, which will select the
   appropriate function based on the number of pixels needing update.  This
   will also clip the input arguments to make sure they are in a valid
   range.  */

   /* Make sure X, Y are in bounds. */
   if(x1 < 0) x1 = 0;
   if(y1 < 0) y1 = 0;
   if(x2 >= w->c->fieldwidth)  x2 = w->c->fieldwidth - 1;
   if(y2 >= w->c->fieldheight) y2 = w->c->fieldheight- 1;

   /* Which subfunction to use? */
   if((y2 - y1) * (x2 - x1) <= USE_PIXMAP_MAX_AREA) { 
      _sc_window_draw_land_pixmap_gtk(w, x1, y1, x2, y2);
   } else {
      _sc_window_draw_land_image_gtk(w, x1, y1, x2, y2);
   }
   
}
