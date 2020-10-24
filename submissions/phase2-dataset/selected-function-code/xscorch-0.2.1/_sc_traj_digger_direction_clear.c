                                            int flags, int dir) {
/* sc_traj_digger_direction_clear
   Check if the specified delta direction is clear or not. */

   sc_trajectory_data t;
   int result;

   assert(c != NULL && tr != NULL);

   /* Get data on our new chosen direction */
   _sc_traj_digger_get_direction(c, tr, &t, dir);
   result = _sc_traj_hit(c, tr, flags, &t);

   /* Special case for 1-pixel wide gaps */
   if(result == SC_TRAJ_CONTINUE) {
      tr->curx += t.stepx;
      tr->cury += t.stepy;
      result = _sc_traj_hit(c, tr, flags, &t);
   } /* Special case... */

   /* Check if we (eventuall) impact land next */
   if(result != SC_TRAJ_IMPACT_LAND) return(false);
   tr->dir = (tr->dir + dir) % 4;
   return(true);

}
