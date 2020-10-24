static inline sc_trajectory_result _sc_traj_pass_wall_wrap(const sc_config *c,
                                                           sc_trajectory *tr) {
/* sc_traj_pass_wall_wrap
   Boundary case for horizontal wrapping.  */

   assert(c != NULL && tr != NULL);

   /* Check if we hit the ground or went too high */
   if(rint(tr->cury) < 0) {
      return(SC_TRAJ_IMPACT_GROUND);
   }

   /* Implement weapon wrapping on horizontal boundaries */
   while(rint(tr->curx) >= c->fieldwidth) {
      tr->curx -= c->fieldwidth;
      tr->ctrx -= c->fieldwidth;
   } /* Adjusting X coordinate */
   while(rint(tr->curx) < 0) {
      tr->curx += c->fieldwidth;
      tr->ctrx += c->fieldwidth;
   } /* Adjusting Y coordinate */

   /* Not much occurred ... */
   return(SC_TRAJ_CONTINUE);

}
