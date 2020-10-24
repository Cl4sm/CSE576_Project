bool sc_ai_trajectory_wind(const sc_config *c, sc_player *p, const sc_player *victim) {
/* sc_ai_trajectory_wind
   Calculates trajectory to the specified coordinates, taking into account
   gravity and wind.  This function assumes we do not have a direct line of
   sight to (dx,dy); therefore, we must assume there is a tall mountain in
   the way, and find the steepest trajectory that is within our power
   limitations.  This function returns 1 if a suitable trajectory was found,
   and zero if the target is "unreachable".  CALCULATER's use this mechanism
   primarily.  */

   sc_trajectory *t; /* Trajectory (temp variable) */
   int deltax;       /* Change in X, from player's position */
   int deltay;       /* Change in Y, from player's position */
   int angle;        /* Current angle we are trying. */
   int power;        /* Power required to reach (dx,dy) */
   int height;       /* Height reached by this trajectory */
   int powermax;     /* Power which maximizes height */
   int anglemax;     /* Angle which maximizes height */
   int heightmax;    /* Maximum attained height. */
   int flags;        /* Default trajectory flags */

   assert(c != NULL && p != NULL && victim != NULL);
   assert(SC_PLAYER_IS_ALIVE(victim));
   assert(p->index != victim->index);

   /* Determine distance & angle between player and (dx,dy) */   
   if(!sc_land_calculate_deltas(c->land, &deltax, &deltay, p->x, p->y, victim->x, victim->y)) {
      deltax = victim->x - p->x;
      deltay = victim->y - p->y;
   }

   /* Compensation for shields */
   _sc_ai_trajectory_compensation(c, p, victim, &deltax, &deltay);

   /* Start at the steepest angle allowed, and go down until we find a
      suitable trajectory.  Note, we only have to check angles from the
      vertical, down to vectorangle; the trajectory cannot be below the
      vector angle, due to gravity.  */
   angle = 90 - SC_AI_SHOOTER_DELTA_A;

   /* Scan for the angle that gets us the highest trajectory we need. */
   heightmax= -1;
   powermax = SC_PLAYER_MAX_POWER + 1;
   anglemax = 0;
   while(angle <= 90 + SC_AI_SHOOTER_DELTA_A) {
      if(_sc_ai_validate_angle(c, p, angle)) {
         if(c->weapons->tunneling && !p->contacttriggers) flags = SC_TRAJ_TUNNELING;
         else flags = SC_TRAJ_DEFAULTS;
         t = sc_traj_new_dest_angle(c, p, flags, sc_player_turret_x(p, angle), sc_player_turret_y(p, angle), 
                                    deltax, deltay, angle, SC_PLAYER_MAX_POWER * SC_PHYSICS_VELOCITY_SCL);
         if(t != NULL) {
            power = DIST(t->velx, t->vely) / SC_PHYSICS_VELOCITY_SCL;
            height = sc_traj_height(t);
            sc_traj_free(&t);
            if(height > heightmax && power <= SC_PLAYER_MAX_POWER) {
               /* Found a new ideal trajectory */
               heightmax= height;
               powermax = power;
               anglemax = angle;
            } /* Better traj? */
         } /* Found a valid trajectory? */
      } /* Is the angle valid? */
   
      /* Try the next angle down... */
      ++angle;
   }

   if(powermax <= SC_PLAYER_MAX_POWER) {
      /* Found a suitable power level! */
      sc_player_set_power(c, p, powermax);
      sc_player_set_turret(c, p, anglemax);
      p->ai->victim = victim;
      if(SC_AI_DEBUG_TRAJECTORY) {
         printf("AI_trajectory:   %s, %s no-line,wind to %d, %d with power=%d, angle=%d\n",
                sc_ai_name(p->aitype), p->name, deltax, deltay, powermax, anglemax);
      }
      return(true);
   } /* Found a trajectory? */

   /* No suitable trajectory was found. */
   return(false);

}
