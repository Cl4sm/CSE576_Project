bool sc_ai_trajectory_scan(const sc_config *c, sc_player *p, const sc_player *victim) {
/* sc_ai_trajectory_scan
   Scan for an optimal trajectory to the specified victim.  Returns true if
   one was found, false otherwise.  */

   int power;
   int angle;

   assert(c != NULL && p != NULL && victim != NULL);
   assert(SC_PLAYER_IS_ALIVE(victim));
   assert(p->index != victim->index);

   power = p->power;
   angle = p->turret;
   if(SC_AI_DEBUG_SCAN) {
      printf("aiscan(%d): initial configuration is  pow %d  ang %d\n", p->index, power, angle);
   }

   if(!_sc_ai_trajectory_scan(c, p, victim, &power, &angle,
                              angle - 20, angle + 20, 10,
                              power - 100, power + 100, 50, 150)) return(false);
   if(!_sc_ai_trajectory_scan(c, p, victim, &power, &angle,
                              angle - 4, angle + 4, 1, 
                              power - 20, power + 20, 5, 50)) return(false);
   if(power == p->power && angle == p->turret) return(false);

   if(SC_AI_DEBUG_SCAN) {
      printf("aiscan(%d): accepted refinement to  pow %d  ang %d\n", p->index, power, angle);
   }
   sc_player_set_power(c, p, power);
   sc_player_set_turret(c, p, angle);
   p->ai->victim = victim;
   return(true);

}
