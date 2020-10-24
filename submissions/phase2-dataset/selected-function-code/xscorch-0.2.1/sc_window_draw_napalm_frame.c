                                 const int *ylist, int size) {
/* sc_window_draw_napalm_frame */

   sc_window_gtk *w = (sc_window_gtk *)w_;
   GdkPixmap *buffer;
   GdkGC *gc;
   int boundx1;
   int boundy1;
   int boundx2;
   int boundy2;
   int height;
   int x;
   int y;

   if(xlist == NULL || ylist == NULL || w_ == NULL) return;

   boundx1 = w->c->fieldwidth;
   boundx2 = w->c->fieldheight;
   boundy1 = 0;
   boundy2 = 0;

   height = w->c->fieldheight;
   buffer = sc_display_get_buffer(SC_DISPLAY(w->screen));
   gc = sc_display_get_gc(SC_DISPLAY(w->screen));
   gdk_gc_set_foreground(gc, &w->colormap->napalm);

   while(size > 0) {
      x = *xlist;
      y = *ylist;
      if(sc_land_translate_xy(w->c->land, &x, &y)) {
         gdk_draw_point(buffer, gc, x, height - y - 1);
         if(*xlist < boundx1) boundx1 = x;
         if(*xlist > boundx2) boundx2 = x;
         if(*ylist < boundy1) boundy1 = y;
         if(*ylist > boundy2) boundy2 = y;
      }
      ++xlist;
      ++ylist;
      --size;
   }

   sc_display_queue_draw(SC_DISPLAY(w->screen),
                         boundx1, height - boundy2 - 1,
                         boundx2 - boundx1 + 1, boundy2 - boundy1 + 1);

}
