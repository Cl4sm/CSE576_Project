                                                const sc_trajectory_data *t) {
/* sc_traj_hit
   Returns results if weapon tried to occupy the same pixel as something.
   It might be modified as needed to contain new trajectory information.
   NOTE: These functions do not have to be idempotent. */

   sc_trajectory_result result;/* Result from sc_traj_hit_tank. */

   assert(c != NULL && tr != NULL && t != NULL);

   /* Check if we hit a tank itself. */
   if(!(flags & SC_TRAJ_IGNORE_TANK)) {
      result = _sc_traj_hit_tank(c, tr);
      if(result != SC_TRAJ_CONTINUE) return(result);
   }

   /* Check if we hit something that is not passable. */
   if(!(flags & SC_TRAJ_IGNORE_LAND)) {
      result = _sc_traj_hit_land(c, tr, flags);
      if(result != SC_TRAJ_CONTINUE) return(result);
   }

   /* We hit nothing at this coordinate */
   return(SC_TRAJ_CONTINUE);

}
