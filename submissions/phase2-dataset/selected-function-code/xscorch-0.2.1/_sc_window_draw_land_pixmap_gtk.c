static void _sc_window_draw_land_pixmap_gtk(sc_window_gtk *w, int x1, int y1, int x2, int y2) {
/* sc_window_draw_land_pixmap_gtk
   This was the original drawing code, sending pixels at a time to the X
   server.  Note this is not at all efficient in that it wastes bandwidth
   and seriously slows us down for large area updates. See the next function
   for a faster approach.  However, if very small areas are being updated
   (say, 10x10 region) this method is still faster.  The bounding box MUST
   be in real coordinates.  */

   sc_color_gtk *colormap;    /* Colormap data */
   GdkPixmap *buffer;         /* Land pixmap buffer */
   GdkColor *lcolor;          /* Last-used color */
   GdkColor *color;           /* Current color */
   GdkGC *gc;                 /* Graphics context */
   const int *lpointer;       /* Pointer into land */
   int height;                /* Plotter height */
   int x;                     /* Current X (land coords) */
   int y;                     /* Current Y (land coords) */
   int y0;                    /* Y0 of the current line */

   /* Get the colormap data, pixmap, and graphics context */
   height = w->c->fieldheight - 1;
   colormap = w->colormap;
   buffer = w->landbuffer;
   gc = sc_display_get_gc(SC_DISPLAY(w->screen));   

   /* All points must be redrawn */
   lcolor = NULL;
   for(x = x1; x <= x2; ++x) {
      /* Get pointer to bottom of current column. */
      lpointer = SC_LAND_XY(w->c->land, x, y1);
      lcolor = NULL;
      y0 = -1;
      for(y = y1; y <= y2; ++y, ++lpointer) {   
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
         }
         /* Only set new pen color if we actually changed pens. */
         if(color != lcolor) {
            if(y0 >= 0) {
               gdk_gc_set_foreground(gc, lcolor);
               gdk_draw_line(buffer, gc, x, height - y0, x, height - y + 1);
            }
            lcolor = color;
            y0 = y;
         }
         /* Draw the point. */
         /*gdk_draw_point(buffer, gc, x, height - y);*/
      } /* Iterating in Y ... */
      if(y0 >= 0) {
         gdk_gc_set_foreground(gc, lcolor);
         gdk_draw_line(buffer, gc, x, height - y0, x, height - y2);
      }
   } /* Iterating in X ... */
   
}
