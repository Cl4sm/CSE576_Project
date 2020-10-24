/* sc_traj_hit_tank
   Determine whether we hit a tank with the weapon at (t).
   The weapon might explode, or it might pass right on through... */

   const sc_player *p; /* Player structure */
   int tx, ty;         /* Weapon current X, Y */
   int i;              /* Index thru players */

   assert(c != NULL && tr != NULL);

   /* Determine current weapon position */
   tx = rint(tr->curx);
   ty = rint(tr->cury);

   /* Iterate through list of players */
   for(i = c->numplayers - 1; i >= 0; --i) {
      p = c->players[i];
      /* We use !p->dead here because missiles should still hit the tank
         until it is actually removed from the playing field... */
      if(!p->dead && sc_player_would_impact(c, p, tx, ty)) {
         /* Weapon hit the tank itself */
         tr->victim = i;
         return(SC_TRAJ_IMPACT_TANK);
      } /* Hit a tank that was still around */
   } /* Loop through players */

   /* No tanks were hit */
   return(SC_TRAJ_CONTINUE);

}
