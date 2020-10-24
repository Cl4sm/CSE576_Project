static void _sc_window_draw_flames_gtk(sc_window_gtk *w, const int *xlist,
                                       const int *ylist, int size) {
/* sc_window_draw_flames_gtk */

   GdkPixmap *buffer;
   GdkGC *gc;
   int *flamemap;
   int *heights;
   int *heightp;
   int boundx1;
   int boundy1;
   int boundx2;
   int boundy2;
   int height;
   int px;
   int py;
   int x;
   int y;

   heights = _sc_window_height_map_gtk(w->c, xlist, ylist, size);
   if(heights == NULL) return;

   height = w->c->fieldheight;
   buffer = sc_display_get_buffer(SC_DISPLAY(w->screen));
   gc = sc_display_get_gc(SC_DISPLAY(w->screen));
   gdk_gc_set_foreground(gc, &w->colormap->napalm);

   boundx1 = w->c->fieldwidth;
   boundx2 = w->c->fieldheight;
   boundy1 = 0;
   boundy2 = 0;

   for(heightp = heights, x = 0; x < w->c->fieldwidth; ++heightp, ++x) {
      y = *heightp;
      if(y >= 0 && sys_drand() < SC_NAPALM_FLAME_PROB) {
         flamemap = _sc_window_flame_map_gtk(w->c);
         if(flamemap != NULL) {
            for(py = 0; py < SC_NAPALM_FLAME_RAD * 2 + 1; ++py) {
               for(px = 0; px < SC_NAPALM_FLAME_RAD * 2 + 1; ++px) {
                  if(*(flamemap + py * (SC_NAPALM_FLAME_RAD * 2 + 1) + px) >= 0) {
                     gdk_gc_set_foreground(gc, &w->colormap->gradient[SC_GRAD_FLAMES][*(flamemap + py * (SC_NAPALM_FLAME_RAD * 2 + 1) + px)]);
                     gdk_draw_point(buffer, gc, x + px - SC_NAPALM_FLAME_RAD, height - y - py);
                  }
               }
            }
            free(flamemap);
         }
         if(x - SC_NAPALM_FLAME_RAD < boundx1) boundx1 = x - SC_NAPALM_FLAME_RAD;
         if(x + SC_NAPALM_FLAME_RAD > boundx2) boundx2 = x + SC_NAPALM_FLAME_RAD;
         if(y                           < boundy1) boundy1 = y;
         if(y + 2 * SC_NAPALM_FLAME_RAD > boundy2) boundy2 = y + 2 * SC_NAPALM_FLAME_RAD;
      }
   }

   free(heights);

   sc_display_queue_draw(SC_DISPLAY(w->screen),
                         boundx1, height - boundy2 - 1,
                         boundx2 - boundx1 + 1, boundy2 - boundy1 + 1);

}
