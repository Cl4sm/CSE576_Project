static int _sc_ai_trajectory_angle_bounds(int deltax, int deltay, int maxdelta) {
/* sc_ai_trajectory_angle_bounds
   Calculate the maximum permitted delta angle, for scanning for an optimal
   angle to fire at.  maxdelta is the maximum delta angle permitted from the
   vertical, to either side.  This function is also aware that angle cannot
   go below the vector angle from us to the victim.  The value returned is
   the maximum delta from the vertical that angle may take on, and the
   return value <= maxdelta.  */

   double vectorangle;/*Vector angle from player to (dx,dy) */
   int delta;        /* Maximum angle we should take on, here. */

   /* Calculate the vector angle. */
   vectorangle = atan2(deltay, deltax) * 180 / M_PI;

   /* Start at the steepest angle allowed, and go down until we find a
      suitable trajectory.  Note, we only have to check angles from the
      vertical, down to vectorangle; the trajectory cannot be below the
      vector angle, due to gravity.  */
   if(deltax >= 0) {
      /* We are firing to the right. */
      if(vectorangle < 0) vectorangle = 0;
      delta = 90 - ceil(vectorangle);
   } else {
      /* We are firing to the left. */
      if(vectorangle < 0) vectorangle = 180;
      delta = floor(vectorangle) - 90;
   } /* Which way are we firing? */

   /* The AI also imposes certain max angles, as an extra sanity check. */
   if(delta > maxdelta) delta = maxdelta;
   return(delta);

}
