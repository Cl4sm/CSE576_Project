/* sc_land_calculate_deltas_d
   Same as above, but uses double values.  */

   /* NOTE: changes to this code should also be made to the double code. */

   double dx;        /* Normal DX */
   double dy;        /* Normal DY */
   double d2x;       /* Alternate DX */

   /* Sanity check */
   if(l == NULL) return(false);

   /* translate the coordinates */
   _sc_land_translate_x_d(l, &x1);
   _sc_land_translate_x_d(l, &x2);
   _sc_land_translate_y_d(l, &y1);
   _sc_land_translate_y_d(l, &y2);

   /* Calculate the `normal' deltas */
   dx = x2 - x1;
   dy = y2 - y1;

   /* If land is wrapping, then calculate an alternate */
   if(l->flags & SC_LAND_WRAP_X) {
      d2x = dx + l->width;
      if(fabs(d2x) < fabs(dx)) dx = d2x;
   }

   /* Set pointers and return success. */
   if(deltax != NULL) *deltax = dx;
   if(deltay != NULL) *deltay = dy;
   return(true);

}
