/* sc_console_draw_vert_scroll
   Draws the vertical scrollbar.  */

   GtkWidget *widget = (GtkWidget *)cons;
   GdkPoint points[3];
   GdkGC *foreground;
   gint arrowh;
   gint startx;
   gint starty;
   gint height;
   gint width;
   gint size;
   gint pos;

   assert(IS_SC_CONSOLE(cons));

   /* Can we even draw yet? */
   if(sc_drawbuf_get_buffer(SC_DRAWBUF(cons)) == NULL) return;
   if(cons->style == CONSOLE_BORDERLESS) return;

   /* Request a GC */
   foreground = gdk_gc_new(widget->window);

   /* Determine vertical scrollbar extents */
   _sc_console_vert_scroll_extents(cons, &startx, &starty, &width, &height, &arrowh);
   _sc_console_vert_trough_extents(cons, starty, height, &pos, &size);

   /* erase any original bars */
   gdk_gc_set_foreground(foreground, &cons->colors.backscroll);
   gdk_draw_rectangle(sc_drawbuf_get_buffer(SC_DRAWBUF(cons)),
                      foreground,
                      TRUE,
                      startx, starty - arrowh,
                      width, height + 2 * arrowh);

   /* setup gc colors for bar */
   gdk_gc_set_foreground(foreground, &cons->colors.forescroll);

   /* Draw vertical slider */
   gdk_draw_rectangle(sc_drawbuf_get_buffer(SC_DRAWBUF(cons)),
                      foreground,
                      TRUE,
                      startx, pos,
                      width, size);

   /* Determine if up-arrow is required */
   if(_sc_console_can_scroll_up(cons)) {
      points[0].x = startx + width / 2;
      points[0].y = starty - arrowh;
      points[1].x = points[0].x - width / 2;
      points[1].y = starty - 2;
      points[2].x = points[0].x + width / 2;
      points[2].y = starty - 2;
      gdk_draw_polygon(sc_drawbuf_get_buffer(SC_DRAWBUF(cons)),
                       foreground,
                       TRUE,
                       points, 3);
   } /* Up arrow? */

   /* Determine if down-arrow is required */
   if(_sc_console_can_scroll_down(cons)) {
      points[0].x = startx + width / 2;
      points[0].y = starty + height + arrowh;
      points[1].x = points[0].x - width / 2;
      points[1].y = starty + height + 2;
      points[2].x = points[0].x + width / 2;
      points[2].y = starty + height + 2;
      gdk_draw_polygon(sc_drawbuf_get_buffer(SC_DRAWBUF(cons)),
                       foreground,
                       TRUE,
                       points, 3);
   } /* Down arrow? */

   /* Release the GC's */
   g_object_unref(foreground);

   /* Make sure everything is queued for draw */
   sc_drawbuf_queue_draw(SC_DRAWBUF(widget), startx, starty - arrowh,
                         width, height + 2 * arrowh);

}
