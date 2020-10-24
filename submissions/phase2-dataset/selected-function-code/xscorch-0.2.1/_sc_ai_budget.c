static inline int _sc_ai_budget(const sc_config *c, const sc_player *p) {
/* sc_ai_budget
   Calculate the amount of money the AI is permitted to spend this turn */

   const sc_economy_config *ec; /* Economy data */
   double percent;              /* Percentage of rounds played */

   /* Any budget constraints? */
   if(c->aicontrol->nobudget) {
      return(p->money * 0.75);
   }

   ec = c->economics;
   if(ec->computersbuy) {
      /* Only buy based on current interest rate, or the default
         conservative budget, whichever is larger */
      if(ec->computersaggressive) {
         percent = SC_AI_AGGRESSIVE_BUDGET;
      } else {
         percent = SC_AI_CONSERVATIVE_BUDGET;
      } /* What rate should we set as the "minimum"? */
      if(ec->interestrate > percent) {
         percent = ec->currentinterest;
      } /* Take maximum of "percent" and current interest rate */

      /* Also factor in the number of rounds remaining :) */
      percent += ((double)(c->curround + 1) / c->numrounds) * (1 - percent);
      if(percent > 1) percent = 1;

      if(SC_AI_DEBUG_BUY) {
         printf("AI_budget:   %s, %s has budget %d\n", sc_ai_name(p->aitype), p->name, (int)(p->money * percent));
      }

      /* That ought to be sufficiently devastating. */
      return(p->money * percent);
   } /* Can computers buy? */

   /* Computers cannot buy. */
   return(0);

}
