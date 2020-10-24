sc_trajectory *sc_traj_new_dest_height(const sc_config *c, const sc_player *p, int flags,
                                       double centerx, double centery,
                                       double deltax, double deltay,
                                       double height) {
/* sc_traj_new_dest_height
   Create a new trajectory with the given power (initial velocity) and
   angle.  The destination coordinate is specified as a delta, i.e. it is
   specified _relative_ to the center coordinate.  The height is specified
   relative to center coordinate, and we must have height > deltay.  */

   sc_trajectory *tr;         /* Storage for the new trajectory */
   double angle;              /* Calculated angle to fire at */
   double power;              /* Calculated power rating */
   double wind;               /* Wind value to use */
   double visc;               /* Viscosity to use */

   /* We must have a valid config */
   if(c == NULL) return(NULL);

   /* Try to get wind and viscosity models */
   wind = (flags & SC_TRAJ_IGNORE_WIND) ? 0 : c->physics->curwind;
   visc = (flags & SC_TRAJ_IGNORE_VISC) ? 0 : c->physics->airviscosity;

   /* Create the trajectory */
   tr = _sc_traj_new(c, p, SC_TRAJ_FLIGHT, flags, centerx, centery);
   if(tr == NULL) return(NULL);

   /* Calculate an expected firing angle */
   angle = _sc_traj_angle(c->physics->gravity, wind, visc, deltax, deltay, height);
   if(angle < 0) {
      /* Trajectory cannot be computed */
      free(tr);
      return(NULL);
   }

   /* Calculate an expected power rating */
   power = _sc_traj_power(c->physics->gravity, wind, visc, deltax, deltay, angle);
   if(power < 0) {
      /* Trajectory cannot be computed */
      free(tr);
      return(NULL);
   }

   /* Calculate initial velocity */
   tr->velx = power * cos(angle * M_PI / 180);
   tr->vely = power * sin(angle * M_PI / 180);

   /* Must we terminate at destination? */
   if(flags & SC_TRAJ_TERMINUS) {
      tr->stopstep = _sc_traj_timesteps(tr, deltay);
   }

   /* Copy init state over */
   _sc_traj_copy_init_params(tr);

   /* Return the trajectory */
   return(tr);

}
