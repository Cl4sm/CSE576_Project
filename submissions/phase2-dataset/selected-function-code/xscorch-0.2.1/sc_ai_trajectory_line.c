bool sc_ai_trajectory_line(const sc_config *c, sc_player *p, const sc_player *victim) {
/* sc_ai_trajectory_line
   Calculates trajectory to the specified coordinates, taking into account
   only the gravity.  This function assumes we do have a direct line of
   sight to (dx,dy); therefore, we can choose any trajectory we like.  Let's
   opt for the trajectory requiring the least amount of power.  */

   sc_trajectory *t; /* Trajectory (temp variable) */
   int deltax;       /* Change in X, from player's position */
   int deltay;       /* Change in Y, from player's position */
   int angle;        /* Current angle we are trying. */
   int power;        /* Power required to reach (dx,dy) */
   int maxdelta;     /* Maximum delta scanning angle */
   int powermin;     /* Best power value found. */
   int anglemin;     /* Best angle found so far */

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
   maxdelta = _sc_ai_trajectory_angle_bounds(deltax, deltay, SC_AI_SHOOTER_SIGHT_A);

   /* Try to find a trajectory consuming the least amount of power. */   
   anglemin = 90;
   powermin = SC_PLAYER_MAX_POWER + 1;
   while(abs(angle - 90) <= maxdelta) {
      if(_sc_ai_validate_angle(c, p, angle)) {
         t = sc_traj_new_dest_angle(c, p, SC_TRAJ_IGNORE_WIND | SC_TRAJ_IGNORE_VISC | SC_TRAJ_RESTRICTIVE, 
                                    sc_player_turret_x(p, angle), sc_player_turret_y(p, angle), deltax, deltay, 
                                    angle, SC_PLAYER_MAX_POWER * SC_PHYSICS_VELOCITY_SCL);
         if(t != NULL) {
            power = DIST(t->velx, t->vely) / SC_PHYSICS_VELOCITY_SCL;
            sc_traj_free(&t);
            if(power < powermin) {
               /* Found a better solution. */
               anglemin = angle;
               powermin = power;
            } /* Is this sol'n better? */
         } /* Found a valid trajectory */
      } /* Is the angle valid? */
      angle -= SGN(deltax);
   }

   /* Did we find an acceptable solution? */   
   if(powermin <= SC_PLAYER_MAX_POWER) {
      /* Yes; set new trajectory and power. */
      sc_player_set_power(c, p, powermin);
      sc_player_set_turret(c, p, anglemin);
      p->ai->victim = victim;
      if(SC_AI_DEBUG_TRAJECTORY) {
         printf("AI_trajectory:   %s, %s line to %d, %d with power=%d, angle=%d\n", 
                sc_ai_name(p->aitype), p->name, deltax, deltay, powermin, anglemin);
      }
      return(true);
   }

   /* No suitable trajectory found */
   return(false);

}
