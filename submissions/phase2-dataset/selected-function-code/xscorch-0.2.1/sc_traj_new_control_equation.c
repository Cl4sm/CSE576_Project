                                  double ctrx, double ctry,
                                  double velx, double vely) {
/* sc_traj_new_control_equation
   Writes a new control equation, while PRESERVING timestep
   it will recenter so at the current timestep, the weapon
   would be at the position and with the velocity indicated.
   ONLY use this function at an interpolation vertex !!  The
   current REAL position and desired new velocity are passed
   in.  Current acceleration and timestep will be preserved.
   This algorithm models the revise_acceleration algorithm.  */

   /* Sanity checks */
   assert(tr != NULL);
   if(tr->type != SC_TRAJ_FLIGHT) {
      fprintf(stderr, "WARNING:  does not make sense to set acceleration of non-airborn trajectories!\n");
      return;
   }

   /* Rewrite initial equation parametres */
   tr->velx = velx - tr->timestep * tr->accx;
   tr->vely = vely + tr->timestep * tr->accy;
   tr->ctrx = ctrx - tr->timestep * (tr->velx + 0.5 * tr->timestep * tr->accx);
   tr->ctry = ctry - tr->timestep * (tr->vely - 0.5 * tr->timestep * tr->accy);

}
