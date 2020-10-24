static bool _sc_load_images(sc_window_gtk *w) {

   const char *filename;

   filename = SC_GLOBAL_DIR "/" SC_IMAGE_DIR "/xscorch-logo.xpm";
   w->logo = gdk_pixmap_colormap_create_from_xpm(w->app->window, NULL,
                                                 &w->logo_m, NULL,
                                                 filename);
   if(w->logo == NULL) {
      fprintf(stderr, "Cannot load \"%s\", aborting.\n", filename);
      return(false);
   }

   filename = SC_GLOBAL_DIR "/" SC_IMAGE_DIR "/xscorch-icon.xpm";
   w->icon = gdk_pixmap_colormap_create_from_xpm(w->app->window, NULL,
                                                 &w->icon_m, NULL,
                                                 filename);
   if(w->icon == NULL) {
      fprintf(stderr, "Cannot load \"%s\", aborting.\n", filename);
      return(false);
   }

   return(true);

}
