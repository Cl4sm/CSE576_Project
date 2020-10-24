                             __libj_unused double visc,
                             double deltax, double deltay,
                             double height) {
/* sc_traj_angle
   Calculates angle required for a trajectory to the specified coordinates,
   while attaining a given maximum height, taking into account gravity.
   WARNING, this function is unable to consider the contribution from wind.
   We assume the current player is at coordinates (0, 0).  Gravity value is
   always positive, and oriented downward.  Wind value is positive if the
   wind is directed to the right (+x direction).  This function returns a
   value less than zero if the trajectory could not be computed that would
   reach the given height. */

   double yslope;    /* Calculation for y-slope */
   double angle;     /* Expected firing angle */

   /* Calculate value in the inner sqrt */
   yslope = height * (height - deltay);
   if(yslope < 0) {
      /* Height is less than target Y */
      return(-1);
   }

   /* Calculate the remainder of the Y slope */
   yslope = 2 * (height + sqrt(yslope));

   /* Calculate target angle */
   angle = atan2(yslope, deltax) * 180 / M_PI;
   if(angle < 0) angle += 360;

   /* Return the angle (if it is valid) */
   if(angle > 180) return(-1);
   return(angle);

}
