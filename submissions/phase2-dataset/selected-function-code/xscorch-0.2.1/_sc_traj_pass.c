static inline sc_trajectory_result _sc_traj_pass(const sc_config *c, sc_trajectory *tr,
                                                 int flags, sc_trajectory_data *t) {
/* sc_traj_pass
   Returns results if weapon passed through a non-pixel boundary.
   It might be modified as needed to contain new trajectory information.
   NOTE: These functions must be idempotent! */

   sc_trajectory_result result;/* Result from sc_traj_hit_tank. */

   assert(c != NULL && tr != NULL && t != NULL);

   /* Check for horizontal/vertical wall boundaries */
   result = _sc_traj_pass_wall(c, tr, t);
   if(result != SC_TRAJ_CONTINUE) return(result);

   /* Check if we hit a tank's shields */
   if(!(flags & SC_TRAJ_IGNORE_TANK)) {
      result = _sc_traj_pass_shield(c, tr, flags, t);
      if(result != SC_TRAJ_CONTINUE) return(result);
   }

   /* We hit nothing at this coordinate */
   return(SC_TRAJ_CONTINUE);

}
