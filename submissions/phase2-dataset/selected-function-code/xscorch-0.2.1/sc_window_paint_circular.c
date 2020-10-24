void sc_window_paint_circular(sc_window *w_, int centerx, int centery, int rad, int flags) {
/* sc_window_paint_circular
   Like the above function, but this updates a circular region of the
   physical display (it will still update a rectangular region of the
   INTERNAL data structures, however).  (centerx, centery) are specified
   in the usual virtual coordinates. 
   
   It is NOT advised that you call this during a SC_REGENERATE_LAND. */
   
   /* TEMP:  I don't play the wraparound game, yet. */
   /* TEMP:  I don't play with wind arrows yet. */

   GdkGC *gc = NULL;
   GdkGC *displaygc;
   GdkPixmap *mask = NULL;
   GdkColor black = { 0, 0x0000, 0x0000, 0x0000 };
   GdkColor white = { 1, 0xffff, 0xffff, 0xffff };
   sc_window_gtk *w = (sc_window_gtk *)w_;/* Window structure */
   int size;
   int minx;
   int miny;
   
   /* Make sure radius is sane. */
   if(rad < 0) return;
   
   /* Construct the clipping mask.  This code is modelled from the code in
      sc_expl_cache_draw().  In fact, it pretty much is the same code... */

   /* Get the display GC */
   displaygc = sc_display_get_gc(SC_DISPLAY(w->screen));

   /* Calculate the screen center coordinates */
   size = rad + rad + 1;
   minx = centerx - rad;
   miny = (w->c->fieldheight - centery - 1) - rad;
   
   /* Create a new temporary mask */
   mask   = gdk_pixmap_new(NULL, size, size, 1);
   if(mask != NULL) gc = gdk_gc_new(mask);
   if(mask != NULL && gc != NULL) {
      gdk_gc_set_foreground(gc, &black);
      gdk_draw_rectangle(mask, gc, TRUE, 0, 0, size, size);
      gdk_gc_set_foreground(gc, &white);
      gdk_draw_arc(mask, gc, TRUE, 0, 0, size, size, 0, 360 * 64);
      gdk_gc_set_clip_mask(displaygc, mask);
      gdk_gc_set_clip_origin(displaygc, minx, miny);
   }

   /* Call the main painter */
   sc_window_paint(w_, centerx - rad, centery - rad, centerx + rad, centery + rad, flags);
   
   /* Uninstall the clipping mask */
   gdk_gc_set_clip_mask(displaygc, NULL);
   if(mask != NULL) g_object_unref(mask);
   if(gc != NULL) g_object_unref(gc);
   
}
