/* sc_window_draw_land_image_gtk
   This code draws land using a GdkImage object (stored client-side).  I
   have found for pixel-by-pixel images, it is faster to construct a client-
   side image and send the entire image across as a single command, as
   opposed to sending a large number of draw-pixel commands to the server. 
   Note this does have some overhead associated, and in general the classic
   pixel-by-pixel approach should be used if only a small rectangular area
   is being updated.  The bounding box MUST be in real coordinates.  */

   GdkImage *image;           /* Local image to draw into */
   sc_color_gtk *colormap;    /* Colormap data */
   GdkColor *color;           /* Current color */
   const int *lpointer;       /* Pointer into land */
   int fheight;               /* Plotter field height */
   int height;                /* Image height */
   int width;                 /* Image width */
   int x;                     /* Current X offset (land coords) */
   int y;                     /* Current Y offset (land coords) */
   
   /* Get the colormap data, pixmap, and graphics context */
   fheight = w->c->fieldheight - 1;
   colormap = w->colormap;
   
   /* Create the local image to draw into */
   height= y2 - y1 + 1;
   width = x2 - x1 + 1;
   image = gdk_image_new(GDK_IMAGE_FASTEST, gtk_widget_get_visual(w->app), width, height);
   if(image == NULL) return;

   /* All points must be redrawn */
   for(x = 0; x < width; ++x) {
      /* Get pointer to bottom of current column. */
      lpointer = SC_LAND_XY(w->c->land, x1 + x, y1);
      for(y = 0; y < height; ++y, ++lpointer) {
         /* Iterating along the column ... */
         switch(SC_LAND_GET_TYPE(*lpointer)) {
            case SC_LAND_GROUND:
               color = &colormap->gradient[SC_GRAD_GROUND][SC_LAND_GET_COLOR(*lpointer)];
               break;
            case SC_LAND_NIGHT_SKY:
               color = &colormap->gradient[SC_GRAD_NIGHT_SKY][SC_LAND_GET_COLOR(*lpointer)];
               break;
            case SC_LAND_FIRE_SKY:
               color = &colormap->gradient[SC_GRAD_FIRE_SKY][SC_LAND_GET_COLOR(*lpointer)];
               break;
            case SC_LAND_SMOKE:
               color = &colormap->pcolors[SC_LAND_GET_COLOR(*lpointer)];
               break;
            case SC_LAND_OBJECT:
               color = &colormap->white;
               break;
            default:
               color = &colormap->black;
         } /* What type of land? */
         /* Draw the point. */
         gdk_image_put_pixel(image, x, height - y - 1, color->pixel);
      } /* Iterating in Y ... */
   } /* Iterating in X ... */

   /* Copy local image to the offscreen pixmap */
   gdk_draw_image(w->landbuffer, sc_display_get_gc(SC_DISPLAY(w->screen)), image, 
                  0, 0, x1, fheight - y2, width, height);
   g_object_unref(image);
   
}
