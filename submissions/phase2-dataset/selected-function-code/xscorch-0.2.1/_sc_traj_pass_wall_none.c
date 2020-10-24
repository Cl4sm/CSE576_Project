static inline sc_trajectory_result _sc_traj_pass_wall_none(const sc_config *c,
                                                           const sc_trajectory *tr) {
/* sc_traj_pass_wall_none
   Boundary case for no walls.  */

   int extend;                /* How far do the borders extend? */

   assert(c != NULL && tr != NULL);

   /* How far do the borders extend? */
   extend = c->physics->bordersextend;

   /* Check if we hit the ground. */
   if(rint(tr->cury) < 0) {
      return(SC_TRAJ_IMPACT_GROUND);
   }

   /* Weapon should no longer be tracked if we go too far away. */
   if(tr->curx < -extend || tr->curx > c->fieldwidth + extend) {
      return(SC_TRAJ_SIZZLE);
   }

   /* Nothing interesting occurred */
   return(SC_TRAJ_CONTINUE);

}
