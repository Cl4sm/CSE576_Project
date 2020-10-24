static bool _sc_traj_roller_clear(const sc_config *c, sc_trajectory *tr, int flags,
                                  double deltax, double deltay) {
/* sc_traj_roller_clear
   This takes a trajectory, and plants it at (curx + deltax),
   (cury + deltay).  It then checks to see if the trajectory
   impacts anything at the specified location.  If NOT, then this
   function returns true, indicating that the specified position
   is clear.  This function will restore the trajectory's
   original position regardless of return value; the trajectory
   is effectively constant.  */

   sc_trajectory_data t;   /* Trajectory steps */
   double origx;           /* Original X position */
   double origy;           /* Original Y position */

   /* Sanity checks */
   assert(c != NULL && tr != NULL);

   /* Save the original X position */
   origx = tr->curx;
   origy = tr->cury;

   /* Setup the temporary trajectory and step data */
   _sc_traj_roller_setup(tr, &t, origx, origy, deltax, deltay);

   /* Check for impact in the specified location. */
   if(!SC_TRAJ_IS_IMPACT(_sc_traj_pass_or_hit(c, tr, flags, &t))) {
      /* We didn't hit! */
      _sc_traj_roller_restore(tr, origx, origy);
      return(true);
   } else {
      /* We impacted something; specified location is not clear */
      _sc_traj_roller_restore(tr, origx, origy);
      return(false);
   }

}
