                             __libj_unused double visc,
                             double deltax, double deltay,
                             double angle) {
/* sc_traj_power
   Calculates power required for a trajectory to the specified coordinates,
   using a given angle, taking into account gravity and wind.  We assume the
   current player is at coordinates (0, 0).  Gravity value is always
   positive, and oriented downward.  Wind value is positive if the wind is
   directed to the right (+x direction).  This function returns a value less
   than zero if the trajectory could not be computed along the given angle. */

   double flighttime;/* Total flying time to reach destination */
   double flttime2;  /* Square of time required to reach target */
   double WSin_GCos; /* (W sin a + G cos a) */
   double velocity;  /* Real velocity rqd to reach (dx,dy) */
   double sina;      /* Sine of angle */
   double cosa;      /* Cosine of angle */
   double xprime;    /* Transformed X coordinate */
   double yprime;    /* Transformed Y coordinate */

   /* Calculate the relevant sines and cosines. */
   sina = sin(angle * M_PI / 180);
   cosa = cos(angle * M_PI / 180);

   /* Check if weapon will hit source eventually. */
   WSin_GCos = wind * sina + gravity * cosa;
   if(WSin_GCos == 0) {
      /* Transform so coords are in a vertical forcing field. */
      xprime = deltax * sina - deltay * cosa;
      yprime = deltax * cosa + deltay * sina;
      if(xprime != 0 || yprime <= 0) return(-1);
      return(sqrt(2 * WSin_GCos * yprime));
   } /* Are we firing on 1 dimension? */

   if(cosa == 0) {
      /* We don't know how to deal with this case. */
      return(-1);
   }

   /* Calculate the flying time. */
   flttime2 = 2 * (deltax * sina - deltay * cosa);
   flttime2 = flttime2 / WSin_GCos;
   if(flttime2 <= 0) {
      /* Flight time is impossible. */
      return(-1);
   } /* Is flight time valid? */
   flighttime = sqrt(flttime2);

   /* Calculate the real velocity. */
   velocity = (deltax - 0.5 * wind * flttime2);
   velocity = velocity / (flighttime * cosa);

   /* Sanity check; velocity cannot be negative. */
   if(velocity < 0) {
      return(-1);
   }

   /* Return the power value */
   return(velocity);

}
