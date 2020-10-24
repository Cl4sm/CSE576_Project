/* sc_window_draw_arc_gtk */

   sc_window_gtk *w = (sc_window_gtk *)data;
   GdkPixmap *buffer;
   GdkGC *gc;
   int x;
   int y;

   x = rint(tr->curx);
   y = rint(tr->cury);
   if(!sc_land_translate_xy(w->c->land, &x, &y)) {
      return(SC_TRAJ_CONTINUE);
   }
   
   buffer = sc_display_get_buffer(SC_DISPLAY(w->screen));   
   gc = sc_display_get_gc(SC_DISPLAY(w->screen));
   gdk_gc_set_foreground(gc, &w->colormap->pcolors[tr->victim]);
   gdk_draw_rectangle(buffer, gc, TRUE, x, c->fieldheight - y - 1, 1, 1);
   sc_display_queue_draw(SC_DISPLAY(w->screen), x, c->fieldheight - y - 1, 1, 1);
   return(SC_TRAJ_CONTINUE);

}
