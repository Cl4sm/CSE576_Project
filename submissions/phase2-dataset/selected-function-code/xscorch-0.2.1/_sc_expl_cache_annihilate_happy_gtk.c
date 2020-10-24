                                                       int radius) {
/* sc_expl_cache_annihilate_happy_gtk
   It's not a bug... it's a ``feature''...  I wonder if anyone has actually
   found this egg in practice.  It's quite fun once it's activated :)  */

   int size = radius + radius + 1;
   int eyesize = (radius >> 2) + (radius >> 4);
   int eyecoff = radius >> 2;
   GdkGC *gc = gdk_gc_new(pixmap);

   gdk_gc_set_foreground(gc, &w->colormap->yellow);
   gdk_draw_arc(pixmap, gc, TRUE, 0, 0, size, size, 0, 360 * 64);
   gdk_gc_set_foreground(gc, &w->colormap->black);
   gdk_gc_set_line_attributes(gc, 3, GDK_LINE_SOLID, GDK_CAP_ROUND, GDK_JOIN_ROUND);
   gdk_draw_arc(pixmap, gc, FALSE, 0, 0, size, size, 0, 360 * 64);
   gdk_draw_arc(pixmap, gc, TRUE, 
                radius - eyecoff - eyesize, radius - eyecoff - eyesize, 
                eyesize, eyesize, 0 * 64, 360 * 64);
   gdk_draw_arc(pixmap, gc, TRUE, 
                radius + eyecoff, radius - eyecoff - eyesize, 
                eyesize, eyesize, 0 * 64, 360 * 64);
   gdk_draw_arc(pixmap, gc, FALSE,
                radius - eyecoff - eyesize, radius + (eyecoff >> 1),
                (eyecoff + eyesize) << 1, eyesize << 1, 200 * 64, 140 * 64);
   g_object_unref(gc);

}
