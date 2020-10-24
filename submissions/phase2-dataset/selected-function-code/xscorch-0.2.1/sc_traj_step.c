sc_trajectory_result sc_traj_step(sc_config *c, sc_trajectory *tr, int flags,
                                  sc_trajectory_action action, void *data) {
/* sc_traj_step
   Run one step of a trajectory.  Used for code that intends to
   actually animate the trajectory.  Only one timestep is run.  */

   sc_trajectory_result result;  /* Result of this function */

   assert(c != NULL);

   if(tr == NULL) {
      return(SC_TRAJ_SIZZLE);
   }

   if(tr->timestep >= tr->stopstep) {
      /* Weapon timed out */
      return(SC_TRAJ_SIZZLE);
   }

   tr->finalstep = min(tr->timestep + SC_TRAJ_TIME_STEP, tr->stopstep);

   result = SC_TRAJ_SIZZLE;
   switch(tr->type) {
      case SC_TRAJ_FLIGHT:
         result = _sc_traj_step_flight(c, tr, flags, action, data);
         break;
      case SC_TRAJ_ROLLER:
         result = _sc_traj_step_roller(c, tr, flags, action, data);
         break;
      case SC_TRAJ_DIGGER:
         result = _sc_traj_step_digger(c, tr, flags, action, data);
         break;
   }

   if(result != SC_TRAJ_CONTINUE && !(flags & SC_TRAJ_NO_MODIFY)) {
      sc_ai_trajectory_terminus(c, tr);
   }

   return(result);

}
