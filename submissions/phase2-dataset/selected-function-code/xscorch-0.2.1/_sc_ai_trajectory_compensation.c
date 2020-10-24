static inline void _sc_ai_trajectory_compensation(const sc_config *c, const sc_player *p, 
                                                  const sc_player *victim, int *deltax, int *deltay) {
/* sc_ai_trajectory_compensation 
   Compensation for shields, if offset targetting is allowed.
   The deltax, deltay values indicate the current position being
   targeted; they may be revised to indicate a new coordinate to
   target if offset targetting is used.  */

   assert(c != NULL && p != NULL && victim != NULL);
   assert(deltax != NULL && deltay != NULL);
   assert(SC_PLAYER_IS_ALIVE(victim));
   assert(p->index != victim->index);

   if(SC_AI_WILL_OFFSET(c, victim)) {
      /* deltax is always along the shortest path, so we will choose
         to compensate even _closer_, i.e. bring the target nearer
         to us in all possible cases.  If they are on a peak, this
         way instead of overshooting the mountain we take out the
         mountain >:) */
      *deltax -= (SC_AI_COMPENSATE_DELTA + victim->tank->radius) * SGN(*deltax); 
   }

}
