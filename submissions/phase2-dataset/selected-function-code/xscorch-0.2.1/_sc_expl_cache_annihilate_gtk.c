                                                 int radius, sc_explosion_type type) {
/* sc_expl_annihilate_rad_gtk
   Annihilate a region centered at (cx,cy) for a radius r.  This function
   uses the type parameter to dispatch to the appropriate drawing
   subfunction.  */

   GdkImage *image;
   int size;

   /* ...wonder why this is here... :) */
   if(w->state < 4) {
      /* Create images */
      size = radius + radius + 1;
      image = gdk_image_new(GDK_IMAGE_FASTEST, gtk_widget_get_visual(w->app), size, size);
      if(image == NULL) return;
      
      /* Draw the appropriate type of explosion. */
      switch(type) {
         case SC_EXPLOSION_NORMAL:
            _sc_expl_cache_annihilate_rad2_gtk(image, w->colormap->gradient[SC_GRAD_EXPLOSION], 
                                               w->c->colors->gradsize[SC_GRAD_EXPLOSION], radius);
            break;
         case SC_EXPLOSION_PLASMA:
            _sc_expl_cache_annihilate_plasmoid_gtk(image, w->colormap->gradient[SC_GRAD_EXPLOSION], 
                                                   w->c->colors->gradsize[SC_GRAD_EXPLOSION], radius);
            break;
         case SC_EXPLOSION_SPIDER:
            _sc_expl_cache_annihilate_rad_gtk(image, w->colormap->gradient[SC_GRAD_FUNKY_EXPLOSION],
                                              w->c->colors->gradsize[SC_GRAD_FUNKY_EXPLOSION], radius);
            break;
         default:
            /* do nothing */;
      } /* Which type of explosion to draw? */

      /* Commit data to pixmap/bitmap buffers */
      gdk_draw_image(pixmap, sc_display_get_gc(SC_DISPLAY(w->screen)), 
                     image, 0, 0, 0, 0, size, size);
      
      /* Destroy the local images */
      g_object_unref(image);
   } else {
      _sc_expl_cache_annihilate_happy_gtk(w, pixmap, radius);
   }

}
