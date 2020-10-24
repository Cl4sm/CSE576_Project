/* sc_land_drop_column
   Drop the land in the current column, x. This is an internal function
   will no sanity checking; caution, X may be a virtual coordinate.  */

   const int *sky;   /* Sky indices for gradient */
   int height;       /* Land height */
   int skyflag;      /* Sky type flag */
   int maxdropallow; /* maximum drop allowed */
   int maxdrop;      /* Distance the land is being dropped */
   bool dither;      /* Enable land dithering? */
   int *lpdown;      /* Pointer to lower position; "falling to" */
   int *lpup;        /* Pointer to upper position; "falling from" */
   int miny;         /* Minimum height that needs to be redrawn */
   int maxy;         /* Maximum height that needs to be redrawn */
   int dy;           /* Amount of land that has actually fallen */
   int y;            /* Current Y coordinate */

   /* Make sure X coordinate is valid */
   if(!_sc_land_translate_x(l, &x)) return(false);

   /* Get and cache the sky attributes */
   sky = sc_land_sky_index(c);
   skyflag = sc_land_sky_flag(c);
   dither = c->graphics.gfxdither;

   /* Set initial Y coodinate and pointers */
   y = 0;
   height = l->height;
   maxdrop = 0;
   lpup = SC_LAND_XY(l, x, 0);

   /* Track down the screen until we find a cell that is not ground. */
   while(y < height && SC_LAND_IS_GROUND(*lpup)) {
      ++lpup;
      ++y;
   }
   lpdown = lpup; /* This is the point where land will fall _to_ */
   miny = y;      /* Minimum coordinate that will need redrawing */
   maxy = y;      /* Assume this is also the maximum coordinate. */

   /* Scan up until we find ground; note, that if everything
      from here on is sky, then there is actually no land to drop.  */
   while(y < height && !SC_LAND_IS_GROUND(*lpup)) {
      ++lpup;
      ++y;
   }

   /* If Y coordinate is out of range, then nothing to be done. */
   if(y >= height) return(false);

   /* Determine if we attempted to drop too far */
   maxdrop = lpup - lpdown;
   if(SC_CONFIG_NO_ANIM(c)) maxdropallow = c->fieldheight;
   else maxdropallow = SC_LAND_MAX_DROP_PER_CYCLE;
   if(maxdrop >= maxdropallow) {
      lpdown = lpup - maxdropallow;
      miny = y - maxdrop;
   }

   /* Start letting the land fall... */
   dy = SC_LAND_MAX_AMOUNT;
   while(y < height && dy >= 0) {
      if(SC_LAND_IS_GROUND(*lpup)) {
         *lpdown = *lpup;
         *lpup = skyflag | sc_color_gradient_index(dither, sky, y);
         maxy = y;
      }
      ++lpdown;
      ++lpup;
      --dy;
      ++y;
   }

   /* Repaint the range */
   sc_window_paint(c->window, x, miny, x, maxy, SC_REGENERATE_LAND | SC_PAINT_EVERYTHING);
   return(maxdrop >= maxdropallow || y < height);

}
