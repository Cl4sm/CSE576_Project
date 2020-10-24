/* sc_ai_random_fire
   Fires a random trajectory and power.  This is the simplest form of AI;
   currently, MORON uses it exclusively, and other AI's may use this
   mechanism as a starter or a fallback targetting system.  */

   int min_power;
   int max_power;
   int power_range;
   int power;
   
   assert(c != NULL && p != NULL);

   /* Mark Anderson corrected this in 20011029015201.A3951@www.muking.org */
   min_power   = max(SC_AI_MORON_MIN_POWER, p->power - SC_AI_POWER_DELTA_MAX);
   max_power   = max(SC_AI_MORON_MIN_POWER, p->power + SC_AI_POWER_DELTA_MAX);
   power_range = max_power - min_power;
   power       = min_power + game_lrand(power_range + 1);

   /* Set a new angle and power level. */
   sc_player_advance_turret(c, p, game_lrand(SC_AI_ANGLE_DELTA_MAX * 2 + 1) - SC_AI_ANGLE_DELTA_MAX);
   sc_player_set_power(c, p, power);

   /* No victim used */
   p->ai->victim = NULL;

   if(SC_AI_DEBUG_TRAJECTORY) {
      printf("AI_trajectory:   %s, %s random fire.\n", sc_ai_name(p->aitype), p->name);
   }

   return(NULL);

}
