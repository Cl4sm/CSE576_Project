sc_trajectory *sc_traj_new_dest_angle(const sc_config *c, const sc_player *p, int flags,
                                      double centerx, double centery,
                                      double deltax, double deltay,
                                      double angle, double maxpower) {
/* sc_traj_new_dest_angle
   Create a new trajectory with the given power (initial velocity) and
   angle.  The destination coordinate is specified as a delta, i.e. it is
   specified _relative_ to the center coordinate.  The angle is is it was
   with new_power_angle().  */

   sc_trajectory *tr;         /* Storage for the new trajectory */
   double height;             /* Maximum height of arc */
   double power;              /* Calculated power rating */
   double wind;               /* Wind value to use */
   double visc;               /* Viscosity to use */

   /* We must have a valid config */
   if(c == NULL) return(NULL);

   /* Create the trajectory */
   tr = _sc_traj_new(c, p, SC_TRAJ_FLIGHT, flags, centerx, centery);
   if(tr == NULL) return(NULL);

   /* Get wind and viscosity ratings */
   wind = (flags & SC_TRAJ_IGNORE_WIND) ? 0 : c->physics->curwind;
   visc = (flags & SC_TRAJ_IGNORE_VISC) ? 0 : c->physics->airviscosity;

   /* Calculate an expected power rating */
   power = _sc_traj_power(c->physics->gravity, wind, visc, deltax, deltay, angle);
   if((power < 0 || power > maxpower) && !(flags & SC_TRAJ_RESTRICTIVE)) {
      /* If wrapping walls, then we can try two more trajectories */
      if(c->physics->walls == SC_WALL_WRAP) {
         power = _sc_traj_power(c->physics->gravity, wind, visc, deltax + c->fieldwidth, deltay, angle);
         if(power < 0 || power > maxpower) {
            power = _sc_traj_power(c->physics->gravity, wind, visc, deltax - c->fieldwidth, deltay, angle);
         } /* Tried a third trajectory */
      } /* Do we have wrapping boundaries? */
   } /* Can we try other trajectories? */

   /* Was the power rating valid? */
   if(power < 0 || power > maxpower) {
      /* Trajectory cannot be computed */
      free(tr);
      return(NULL);
   }

   /* Calculate initial velocity */
   tr->velx = power * cos(angle * M_PI / 180);
   tr->vely = power * sin(angle * M_PI / 180);

   /* Make sure we won't hit the upper boundary */
   if(c->physics->walls != SC_WALL_NONE && c->physics->walls != SC_WALL_WRAP) {
      height = sc_traj_height(tr) + tr->ctry + SC_TRAJ_THRESHOLD;
      if(height >= c->fieldheight) {
         /* Weapon would have hit the ceiling.  Invalid. */
         free(tr);
         return(NULL);
      } /* Did we hit ceiling? */
   } /* Do we have boundaries? */

   /* Copy init state over */
   _sc_traj_copy_init_params(tr);

   /* Return the trajectory */
   return(tr);

}
