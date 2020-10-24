/* sc_ai_fire_at_victim
   Attack victim in AI state.  If NULL or they are dead, then behave like
   calculated_fire.  This is the "big bully" behaviour; once a victim is
   chosen, well, they're pretty much dead.  */

   const sc_player *vp;

   assert(c != NULL && p != NULL);

   /* Is the victim still alive? */
   vp = p->ai->victim;
   if(vp != NULL && SC_PLAYER_IS_ALIVE(vp)) {
      /* "Victim" isn't dead; can we shoot them? */
      if(_sc_ai_line_of_sight(c, p, vp, sc_ai_trajectory_line, sc_ai_trajectory)) {
         /* I AM A RETURN STATEMENT!  FEAR MY WRATH! */
         return(vp);
      }
   } /* Was victim alive? */

   /* If at this point, we have no victim; find another. */
   return(_sc_ai_calculated_fire(c, p));

}
