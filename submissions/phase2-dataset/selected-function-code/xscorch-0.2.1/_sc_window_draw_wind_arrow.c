static void _sc_window_draw_wind_arrow(sc_window_gtk *w, bool drawing) {
/* sc_window_draw_wind_arrow
   Draws the wind arrow in the upper-right corner of the screen.  If
   drawing is set, we will draw an arrow; otherwise we will erase the
   last arrow that was drawn to the screen.  */

   GdkPixmap *buffer;         /* Screen buffer */
   GdkGC *gc;                 /* Screen gc */
   int size;                  /* Size of wind arrow */
   int x;                     /* Anchor X (screen coords) */
   int y;                     /* Anchor Y (screen coords) */

   /* Get the buffer and GC to use in drawing. */
   buffer = sc_display_get_buffer(SC_DISPLAY(w->screen));   
   gc = sc_display_get_gc(SC_DISPLAY(w->screen));

   /* Calculate the arrow size */
   if(drawing) {
      size = abs((int)(w->c->physics->curwind * 5000 / SC_PHYSICS_WIND_MAX));
   } else {
      size = w->windarrowsize;
   }
   x = w->c->fieldwidth - SC_WIND_ARROW_MARGIN;
   y = SC_WIND_ARROW_SIZE + SC_WIND_ARROW_MARGIN;
   if(size > SC_WIND_MAXIMUM_ARROW) size = SC_WIND_MAXIMUM_ARROW;

   /* Drawing or clearing? */
   if(drawing) {
      /* Update the current windarrow size */
      w->windarrowsize = size;

      if(size < SC_WIND_MINIMUM_ARROW) {
         size = SC_WIND_ARROW_SIZE + SC_WIND_ARROW_SIZE;

         /* Draw the SHADOW */
         gdk_gc_set_foreground(gc, &w->colormap->black);
         _sc_window_draw_x(buffer, gc, x + 1, y + 1);
         _sc_window_draw_x(buffer, gc, x + 1, y + 2);
      
         /* Draw the arrow mainline and cap */
         gdk_gc_set_foreground(gc, &w->colormap->windar);
         _sc_window_draw_x(buffer, gc, x, y);
      } else {
         /* Draw the arrow SHADOW */
         gdk_gc_set_foreground(gc, &w->colormap->black);
         _sc_window_draw_arrow(buffer, gc, x + 1, y + 1, size, w->c->physics->curwind >= 0);
         _sc_window_draw_arrow(buffer, gc, x + 1, y + 2, size, w->c->physics->curwind >= 0);
         
         /* Draw the arrow mainline and cap */
         gdk_gc_set_foreground(gc, &w->colormap->windar);
         _sc_window_draw_arrow(buffer, gc, x, y, size, w->c->physics->curwind >= 0);
      }
   } else {
      /* Clearing an existing arrow */
      if(size < SC_WIND_MINIMUM_ARROW) size = SC_WIND_ARROW_SIZE + SC_WIND_ARROW_SIZE;
      gdk_draw_drawable(buffer, gc, w->landbuffer, 
                        x - size, y - SC_WIND_ARROW_SIZE,
                        x - size, y - SC_WIND_ARROW_SIZE,
                        size + 3, SC_WIND_ARROW_SIZE + SC_WIND_ARROW_SIZE + 3);
   } /* Drawing or clearing? */
   
   /* Update the display */
   sc_display_queue_draw(SC_DISPLAY(w->screen), 
                         x - size, y - SC_WIND_ARROW_SIZE,
                         size + 3, SC_WIND_ARROW_SIZE + SC_WIND_ARROW_SIZE + 3);

}
