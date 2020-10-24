/* sc_window_draw_weapon
   Draw the indicated weapon to the screen.  */

   sc_window_gtk *w = (sc_window_gtk *)w_;
   int size = w->c->weapons->bombiconsize;
   GdkPixmap *buffer;
   GdkGC *gc;
   int x;
   int y;
   
   x = rint(wp->tr->curx) - (size >> 1);
   y = rint(wp->tr->cury) - (size >> 1);
   if(!sc_land_translate_xy(w->c->land, &x, &y)) return;
   x = x;
   y = w->c->fieldheight - y - 1;

   buffer = sc_display_get_buffer(SC_DISPLAY(w->screen));   
   gc = sc_display_get_gc(SC_DISPLAY(w->screen));
   gdk_gc_set_foreground(gc, &w->colormap->white);
   gdk_draw_rectangle(buffer, gc, TRUE, x, y, size, size);
   sc_display_queue_draw(SC_DISPLAY(w->screen), x, y, size, size);

}
