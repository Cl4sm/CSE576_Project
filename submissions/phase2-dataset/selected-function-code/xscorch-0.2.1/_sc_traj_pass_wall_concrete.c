                                                               const sc_trajectory *tr) {
/* sc_traj_pass_wall_concrete
   Boundary case for a good ol' solid concrete box.  */

   assert(c != NULL && tr != NULL);

   /* Check if we hit anything. */
   if(rint(tr->curx) < 0 || rint(tr->curx) >= c->fieldwidth) {
      return(SC_TRAJ_IMPACT_WALL);
   }
   if(rint(tr->cury) >= c->fieldheight) {
      return(SC_TRAJ_IMPACT_WALL);
   }
   if(rint(tr->cury) < 0) {
      return(SC_TRAJ_IMPACT_GROUND);
   }

   /* Not much occurred ... */
   return(SC_TRAJ_CONTINUE);

}
