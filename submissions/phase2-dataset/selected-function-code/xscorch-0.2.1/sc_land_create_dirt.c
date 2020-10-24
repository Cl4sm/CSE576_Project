/* sc_land_create_dirt
   This function creates dirt at each of the virtual coordinates in the
   lists of coordinates given.  xlist and ylist are list of (x, y) virtual
   coordinate pairs, and size indicates the size of the lists.  */

   bool dither;                     /* Dither flag */
   const int *gnd;                  /* Ground gradient */
   int boundx1 = c->fieldwidth;     /* Bound that was altered X1 */
   int boundy1 = c->fieldheight;    /* Bound that was altered Y1 */
   int boundx2 = 0;                 /* Bound that was altered X2 */
   int boundy2 = 0;                 /* Bound that was altered Y2 */
   int x;                           /* Current translated X */
   int y;                           /* Current translated Y */

   /* Sanity checks */
   if(c == NULL || l == NULL || xlist == NULL || ylist == NULL) return;

   /* Obtain dither information */
   dither = c->graphics.gfxdither;
   gnd = c->colors->gradindex[SC_GRAD_GROUND];

   /* Scan through the list of coordinates */
   while(size > 0) {
      /* translate this coordinate */
      x = *xlist;
      y = *ylist;
      if(_sc_land_translate_x(l, &x) && _sc_land_translate_y(l, &y)) {
         /* Create dirt at this coordinate */
         *SC_LAND_XY(l, x, y) = SC_LAND_GROUND | sc_color_gradient_index(dither, gnd, y);
         /* Update the real bounding box */
         if(x < boundx1) boundx1 = x;
         if(x > boundx2) boundx2 = x;
         if(y < boundy1) boundy1 = y;
         if(y > boundy2) boundy2 = y;
      } /* translation valid? */
      ++xlist;
      ++ylist;
      --size;
   } /* loop through list */

   /* Repaint the affected region */
   sc_window_paint(c->window, boundx1, boundy1, boundx2, boundy2, SC_REGENERATE_LAND | SC_PAINT_EVERYTHING);

}
