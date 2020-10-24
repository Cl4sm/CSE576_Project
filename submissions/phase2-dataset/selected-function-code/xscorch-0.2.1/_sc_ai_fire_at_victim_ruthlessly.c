/* sc_ai_fire_at_victim_ruthlessly
   Attack victim in AI state.  If NULL or they are dead, then behave like
   calculated_fire.  This is the "big bully" behaviour; once a victim is
   chosen, well, they're pretty much dead.  This is insane, it compensates
   for wind.  */

   const sc_player *vp;

   assert(c != NULL && p != NULL);

   /* Is the victim still alive? */
   vp = p->ai->victim;
   if(vp != NULL && SC_PLAYER_IS_ALIVE(vp)) {
      /* "Victim" isn't dead; can we shoot them? */
      if(_sc_ai_line_of_sight(c, p, vp, sc_ai_trajectory_line_wind, sc_ai_trajectory_wind)) {
         /* I AM A RETURN STATEMENT!  PH33R MY LACK OF B33R! */
         return(vp);
      }
   } /* Was victim alive? */

   /* If at this point, we have no victim; find another. */
   return(_sc_ai_calculated_fire_wind(c, p));

}
