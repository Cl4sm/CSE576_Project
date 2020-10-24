void sc_window_paint(sc_window *w_, int x1, int y1, int x2, int y2, int flags) {
/* sc_window_paint
   This function updates a rectangular region of the display, using the
   coordinates (x1,y1)-(x2,y2) as a bounding box for the area needing
   update.  Flags are also given to indicate which drawing operations should
   be done -- note SC_REGENERATE_LAND is VERY expensive and should be
   avoided when the land hasn't been physically altered.  */

   sc_window_gtk *w = (sc_window_gtk *)w_;/* Window structure */
   int ox1;                   /* Overlap X1 */
   int ox2;                   /* Overlap X2 */
   int screenx;               /* Screen top-left X */
   int screeny;               /* Screen top-left Y */
   int screenw;               /* Width of area being redrawn */
   int screenh;               /* Height of area being redrawn */
   
   /* Swap boundaries if they are reversed. */
   if(x1 > x2) {
      x1 = x1 + x2;
      x2 = x1 - x2;
      x1 = x1 - x2;
   } /* X coordinates swapped? */
   if(y1 > y2) {
      y1 = y1 + y2;
      y2 = y1 - y2;
      y1 = y1 - y2;
   } /* Y coordinates swapped? */

   /* Correct X bounds */
   if(!sc_land_translate_x_range(w->c->land, &x1, &x2)) return;
   
   /* Check for overlap */
   ox1 = x1;
   ox2 = x2;
   if(sc_land_overlap_x(w->c->land, &ox1, &ox2)) {
      sc_window_paint(w_, ox1, y1, ox2, y2, flags);
   }
    
   /* Determine the screen boundaries. */
   screenx = x1;
   screeny = w->c->fieldheight - y2 - 1;
   screenw = x2 - x1 + 1;
   screenh = y2 - y1 + 1;

   /* Regeneration of land pixmap */
   if(flags & SC_REGENERATE_LAND) {
      _sc_window_draw_land_gtk(w, x1, y1, x2, y2);
   } /* Regenerate the land? */

   /* Unpaint any existing wind arrow, if clear given */
   if(flags & SC_CLEAR_WIND_ARROW || flags & SC_REDRAW_WIND_ARROW) {
      _sc_window_draw_wind_arrow(w, false);
   } /* Undraw wind arrow from screen? */

   /* Copy current land pixmap to screen. */
   if(flags & SC_REDRAW_LAND) {
      gdk_draw_drawable(sc_display_get_buffer(SC_DISPLAY(w->screen)), 
                        sc_display_get_gc(SC_DISPLAY(w->screen)),
                        w->landbuffer, 
                        screenx, screeny, screenx, screeny, screenw, screenh);
      sc_display_queue_draw(SC_DISPLAY(w->screen), screenx, screeny, screenw, screenh);
   } /* Copy land to screen? */

   /* Draw any tanks in redraw area to the screen. */
   if(flags & SC_REDRAW_TANKS) {
      _sc_window_draw_all_tanks_gtk(w, x1, y1, x2, y2);
   } /* Draw tanks to screen? */

   /* Draw wind arrow if requested */
   if(flags & SC_REDRAW_WIND_ARROW) {
      _sc_window_draw_wind_arrow(w, true);
   } /* Draw wind arrow to screen? */

}
