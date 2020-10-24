                                         int radius, bool fill) {
/* sc_expl_annihilate_circular
   Clears (if fill == false) or fills (if fill == true) dirt in the
   explosion specified.  This updates the land buffer but it will NOT
   update the display screen.

   This function assumes there is no "wedge" being computed.  It
   bypasses pretty much all of the wedge-specific computations as
   a result.  */

   int cx=e->centerx;/* Center X of explosion */
   int cy=e->centery;/* Center Y of explosion */
   int dx;           /* Delta X (distance away from cx) - iterator variable */
   int dy;           /* Delta Y (distance away from cy) for _edge_ of circle */
   int rad2;         /* Radius squared */
   int rad2major2;   /* Radius^2 + the major_distance^2 */
   int min2thresh;   /* Minimum threshold to avoid redrawing columns where dx>dy */

   /* DX = major axis, DY = minor axis */
   dx = 0;           /* DX starts at zero (iterator) */
   dy = radius;      /* DY is one radius away (edge of circle at cx+dx) */
   rad2 = radius * radius; /* Calculate Radius Squared */
   rad2major2 = rad2;      /* Radius^2 + major^2, running total */
   min2thresh = rad2 - dy; /* Minimum threshold before need to redraw edges */

   /* Should know that, we are incrementing DX every time.  However,
      if we call the transpose method every time as well, then we will
      be filling parts of the circle multiple times.  Hence the
      min2thresh variable. */
   do {
      _sc_expl_annihilate_column(c, c->land, cx - dx, cy - dy, cy + dy, fill);
      _sc_expl_annihilate_column(c, c->land, cx + dx, cy - dy, cy + dy, fill);
      ++dx;
      rad2major2 -= dx + dx - 1;
      if(rad2major2 <= min2thresh) {
         /* Swap dx and dy */
         _sc_expl_annihilate_column(c, c->land, cx - dy, cy - dx, cy + dx, fill);
         _sc_expl_annihilate_column(c, c->land, cx + dy, cy - dx, cy + dx, fill);
         --dy;
         min2thresh -= dy + dy;
      }
   } while(dx <= dy);

}
