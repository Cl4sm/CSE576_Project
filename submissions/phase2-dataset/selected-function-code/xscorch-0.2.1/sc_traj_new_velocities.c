                                      double centerx, double centery,
                                      double velx, double vely) {
/* sc_traj_new_velocities
   Create a new trajectory with the given initial velocity, broken up
   into X and Y components.  Accelerations are inherited from the config
   structure.  This builds a FLIGHT trajectory.  */

   sc_trajectory *tr;         /* Storage for the new trajectory */

   assert(c != NULL);
   assert(p != NULL);

   /* Create the trajectory */
   tr = _sc_traj_new(c, p, SC_TRAJ_FLIGHT, flags, centerx, centery);
   if(tr == NULL) return(NULL);

   /* Calculate initial velocity */
   tr->velx = velx;
   tr->vely = vely;

   /* Copy init state over */
   _sc_traj_copy_init_params(tr);

   /* Return the trajectory */
   return(tr);

}
