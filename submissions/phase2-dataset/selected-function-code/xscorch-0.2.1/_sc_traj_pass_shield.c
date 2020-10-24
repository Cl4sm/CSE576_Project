                                                 int flags, const sc_trajectory_data *t) {
/* sc_traj_pass_shield
   Determine whether we are passing through a tank's shields with the
   weapon at (t).  The weapon might explode, or it might sizzle, or it
   might pass right on through... */

   const sc_player *p; /* Player structure */
   int i;              /* Index thru players */

   assert(c != NULL && tr != NULL && t != NULL);

   /* Iterate through list of players */
   for(i = c->numplayers - 1; i >= 0; --i) {
      p = c->players[i];
      if(SC_PLAYER_IS_ALIVE(p) &&
            sc_shield_would_impact(c, tr->player, p, flags, tr->lastx, tr->lasty, tr->curx, tr->cury)) {
         /* Weapon hit the shield */
         tr->victim = i;
         return(SC_TRAJ_IMPACT_SHIELD);
      } /* Player was shielded and wasn't dead.  Bummer.  :) */
   } /* Loop through players */

   /* No tanks were hit */
   return(SC_TRAJ_CONTINUE);

}
