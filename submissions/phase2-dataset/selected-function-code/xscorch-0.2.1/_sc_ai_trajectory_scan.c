                                   int *newpower, int *newangle, int minangle, int maxangle, int deltaangle, 
                                   int minpower, int maxpower, int deltapower, double maxdist) {
/* _sc_ai_trajectory_scan
   Scans all possible turret angles and power levels, trying to find the
   best way to hit the intended victim.  This uses the real trajectory
   mechanism so this should take every possible contingency into account. */

   sc_trajectory_result result;
   sc_trajectory *t; /* Trajectory (temp variable) */
   int deltax;       /* Change in X, from player's position */
   int deltay;       /* Change in Y, from player's position */
   double dist;      /* Distance off from desired destination */
   int angle;        /* Current angle we are trying. */
   int power;        /* Power required to reach (dx,dy) */
   double distmin;   /* Best distance found... */
   int powermin;     /* Best power value found. */
   int anglemin;     /* Best angle found so far */
   int tx;           /* Targetted X coordinate */
   int ty;           /* Targetted Y coordinate */

   assert(c != NULL && p != NULL && victim != NULL);
   assert(newpower != NULL && newangle != NULL);
   assert(minangle <= maxangle);
   assert(minpower <= maxpower);
   assert(deltaangle > 0);
   assert(deltapower > 0);
   assert(maxdist > 0);
   assert(SC_PLAYER_IS_ALIVE(victim));
   assert(p->index != victim->index);

   /* Determine distance & angle between player and (dx,dy) */   
   if(!sc_land_calculate_deltas(c->land, &deltax, &deltay, p->x, p->y, victim->x, victim->y)) {
      deltax = victim->x - p->x;
      deltay = victim->y - p->y;
   }

   /* Compensation for shields */
   _sc_ai_trajectory_compensation(c, p, victim, &deltax, &deltay);

   /* Construct the desired target coordinate */
   tx = p->x + deltax;
   ty = p->y + deltay;

   /* Try to find a trajectory consuming the least amount of power. */   
   if(SC_AI_DEBUG_SCAN) {
      printf("aiscan(%d): scanning\n", p->index);
   }
   anglemin = 90;
   powermin = SC_PLAYER_MAX_POWER + 1;
   distmin  = 0;
   for(angle = minangle; angle <= maxangle; angle += deltaangle) {
      if(SC_AI_DEBUG_SCAN) {
         printf("aiscan(%d):  scanning angle %d\n", p->index, angle);
      }
      for(power = minpower; power < maxpower && power < SC_PLAYER_MAX_POWER; power += deltapower) {
         if(power >= SC_AI_SCAN_MIN_POWER) {
            t = sc_traj_new_power_angle(c, p, SC_TRAJ_NO_MODIFY, 
                                        sc_player_turret_x(p, angle), sc_player_turret_y(p, angle), 
                                        power * SC_PHYSICS_VELOCITY_SCL, angle);
            if(t != NULL) {
               result = sc_traj_run((sc_config *)c, t, SC_TRAJ_NO_MODIFY, NULL, NULL);
               if(SC_TRAJ_IS_IMPACT(result) && result != SC_TRAJ_IMPACT_SHIELD) {
                  /* Check to see how close the impact was */
                  if(sc_land_calculate_deltas(c->land, &deltax, &deltay, tx, ty, rint(t->curx), rint(t->cury))) {
                     dist = SQR((double)deltax) + SQR((double)deltay);
                     if(dist < distmin || powermin > SC_PLAYER_MAX_POWER) {
                        anglemin = angle;
                        powermin = power;
                        distmin  = dist;
                     } /* new optimal */
                  } else if(SC_AI_DEBUG_SCAN) {
                     printf("    %d, %d: no deltas!\n", power, angle);
                  }
               } else if(SC_AI_DEBUG_SCAN) {
                  printf("    %d, %d: no impact!\n", power, angle);
               }
               sc_traj_free(&t);
            } else if(SC_AI_DEBUG_SCAN) {
               printf("    %d, %d: no traj!\n", power, angle);
            }
         } /* min power satisfied? */
      } /* power loop */
   } /* angle loop */

   /* Did we find an acceptable solution? */   
   if(powermin <= SC_PLAYER_MAX_POWER && distmin >= 0 && distmin <= SQR(maxdist)) {
      /* Yes; set new trajectory and power. */
      *newpower = powermin;
      *newangle = anglemin;
      if(SC_AI_DEBUG_SCAN) {
         printf("aiscan(%d): done with %g at pow %d, ang %d\n", p->index, sqrt(distmin), powermin, anglemin);
      }
      return(true);
   }

   /* No suitable trajectory found */
   if(SC_AI_DEBUG_SCAN) {
      printf("aiscan(%d): done with %g, bad\n", p->index, sqrt(distmin));
   }
   return(false);

}
