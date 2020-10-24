void sc_expl_cache_draw(sc_window *w_, int ptr, int centerx, int centery, int rad) {
/* sc_expl_cache_draw
   Draws the explosion to the screen buffer.  */

   GdkColor black = { 0, 0x0000, 0x0000, 0x0000 };
   GdkColor white = { 1, 0xffff, 0xffff, 0xffff };
   sc_window_gtk *w = (sc_window_gtk *)w_;/* Window structure */
   sc_expl_cache_entry_gtk *centry = &(w->explcache->cache[ptr]);
   GdkPixmap *mask;                       /* Mask to use */
   int radius;                            /* Radius of explosion */
   int size;                              /* Size (wid/hei) of expl */

   /* Calculate the screen center coordinates */
   radius = centry->radius;
   centerx = centerx;
   centery = w->c->fieldheight - centery - 1;
   if(rad > radius) rad = radius;
   size = rad + rad + 1;
   
   /* Create a new temporary mask */
   mask = gdk_pixmap_new(NULL, size, size, 1);
   if(mask != NULL) {
      gdk_gc_set_foreground(w->explcache->bitmapgc, &black);
      gdk_draw_rectangle(mask, w->explcache->bitmapgc, TRUE, 
                         0, 0, size, size);
      gdk_gc_set_foreground(w->explcache->bitmapgc, &white);
      gdk_draw_arc(mask, w->explcache->bitmapgc, TRUE, 
                   0, 0, size, size, 0, 360 * 64);
   }
         
   /* Draw the explosion to the screen buffer */
   gdk_gc_set_clip_mask(sc_display_get_gc(SC_DISPLAY(w->screen)), mask);
   gdk_gc_set_clip_origin(sc_display_get_gc(SC_DISPLAY(w->screen)), 
                          centerx - rad, centery - rad);
   gdk_draw_drawable(sc_display_get_buffer(SC_DISPLAY(w->screen)),
                     sc_display_get_gc(SC_DISPLAY(w->screen)),
                     centry->pixmap,
                     radius - rad, radius - rad,
                     centerx - rad, centery - rad,
                     size, size);
   gdk_gc_set_clip_mask(sc_display_get_gc(SC_DISPLAY(w->screen)), NULL);

   /* Update drawing */   
   sc_display_queue_draw(SC_DISPLAY(w->screen),
                         centerx - rad, centery - rad,
                         size, size);
   
   /* Uninstall the bitmap */
   if(mask != NULL) {
      g_object_unref(mask);
   }
   
}
