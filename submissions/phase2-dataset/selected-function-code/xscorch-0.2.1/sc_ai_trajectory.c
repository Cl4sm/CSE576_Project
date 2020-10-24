bool sc_ai_trajectory(const sc_config *c, sc_player *p, const sc_player *victim) {
/* sc_ai_trajectory
   Calculates trajectory to the specified coordinates, taking into account
   only the gravity.  This function assumes we do not have a direct line of
   sight to (dx,dy); therefore, we must assume there is a tall mountain in
   the way, and find the steepest trajectory that is within our power
   limitations.  This function returns true if a suitable trajectory was
   found, and false if the target is "unreachable".  SHOOTER's use this
   mechanism primarily.  */

   sc_trajectory *t; /* Trajectory (temp variable) */
   int deltax;       /* Change in X, from player's position */
   int deltay;       /* Change in Y, from player's position */
   int angle;        /* Current angle we are trying. */
   int power;        /* Power required to reach (dx,dy) */
   int maxdelta;     /* Maximum delta scanning angle */

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
   angle = 90 - SGN(deltax) * SC_AI_SHOOTER_START_A;
   maxdelta = _sc_ai_trajectory_angle_bounds(deltax, deltay, SC_AI_SHOOTER_DELTA_A);

   /* Scan for the steepest angle that gets us the trajectory we need. */
   while(abs(angle - 90) <= maxdelta) {
      if(_sc_ai_validate_angle(c, p, angle)) {
         t = sc_traj_new_dest_angle(c, p, SC_TRAJ_IGNORE_WIND | SC_TRAJ_IGNORE_VISC, 
                                    sc_player_turret_x(p, angle), sc_player_turret_y(p, angle), deltax, deltay,
                                    angle, SC_PLAYER_MAX_POWER * SC_PHYSICS_VELOCITY_SCL);
         if(t != NULL) {
            /* Found a suitable power level! */
            power = DIST(t->velx, t->vely) / SC_PHYSICS_VELOCITY_SCL;
            sc_traj_free(&t);
            sc_player_set_power(c, p, power);
            sc_player_set_turret(c, p, angle);
            p->ai->victim = victim;
            if(SC_AI_DEBUG_TRAJECTORY) {
               printf("AI_trajectory:   %s, %s no-line to %d, %d with power=%d, angle=%d\n", 
                      sc_ai_name(p->aitype), p->name, deltax, deltay, power, angle);
            }
            return(true);
         } /* Found a trajectory? */
      } /* Is the angle valid? */

      /* Try the next angle down... */
      angle -= SGN(deltax);
   }

   /* No suitable trajectory was found. */
   return(false);

}
