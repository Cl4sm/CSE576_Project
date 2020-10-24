static sc_trajectory_result _sc_traj_step_roller(sc_config *c, sc_trajectory *tr, int flags,
                                                 sc_trajectory_action action, void *data) {
/* sc_traj_step_roller */

   sc_trajectory_result result;  /* Result of this function */
   double distance;              /* Distance travelling. */

   assert(c != NULL && tr != NULL);

   while(tr->timestep <= tr->finalstep) {
      tr->oldx = tr->curx;
      tr->oldy = tr->cury;
      distance = fabs(tr->velx);
      result = _sc_traj_traverse_roller(c, tr, flags, distance, action, data);
      if(result != SC_TRAJ_CONTINUE) return(result);
      if(tr->timestep < tr->finalstep) {
         tr->timestep = min(tr->timestep + SC_TRAJ_TIME_STEPS_PER_SUBSTEP, tr->finalstep);
      } else {
         tr->timestep += SC_TRAJ_TIME_STEPS_PER_SUBSTEP;
      }
   }
   return(SC_TRAJ_CONTINUE);

}
