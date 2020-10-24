sc_trajectory *sc_traj_new_power_angle(const sc_config *c, const sc_player *p, int flags,
                                       double centerx, double centery,
                                       double power, double angle) {
/* sc_traj_new_power_angle
   Create a new trajectory with the given power (initial velocity) and
   angle.  WARNING, the power value must already be scaled to "proper" units
   before it is passed to this function!  The angle is a standard angle in
   degrees, with 0==right, 90==up.  This builds a FLIGHT trajectory.  */

   sc_trajectory *tr;         /* Storage for the new trajectory */

   assert(c != NULL);
   assert(p != NULL);

   /* Create the trajectory */
   tr = _sc_traj_new(c, p, SC_TRAJ_FLIGHT, flags, centerx, centery);
   if(tr == NULL) return(NULL);

   /* Calculate initial velocity */
   tr->velx = power * cos(angle * M_PI / 180);
   tr->vely = power * sin(angle * M_PI / 180);

   /* Copy init state over */
   _sc_traj_copy_init_params(tr);

   /* Return the trajectory */
   return(tr);

}
