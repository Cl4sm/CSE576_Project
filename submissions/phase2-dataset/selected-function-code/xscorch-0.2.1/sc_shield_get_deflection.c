/* sc_shield_get_deflection
   Find the total deflection in velocity of a missile by magnetic shields. */

   double dx, dy, sdist, force;
   bool changed = false;
   sc_player *p;
   int index;

   /* Look at each player. */
   for(index = 0; index < c->numplayers; ++index) {
      p = c->players[index];

      /* There are THREE continue statements below. */

      /* A player's mag shield is tuned so as not to affect his own weapons. */
      if(p == NULL || p->index == owner->index)
         continue;  /* NOTE this is a flow control point! */

      /* Determine whether this player has a magnetic shield. */
      if(p->shield == NULL || !SC_ACCESSORY_SHIELD_IS_MAGNETIC(p->shield->info))
         continue;  /* NOTE this is a flow control point! */

      /* Try and get a proper distance estimate. */
      if(!sc_land_calculate_deltas_d(c->land, &dx, &dy, (double)p->x, (double)p->y, x, y))
         continue;  /* This is a rare (perhaps nonexistant) failure case. */

      /* Determine whether the tank is close enough, but not too close. */
      sdist = SQR(dx) + SQR(dy);
      if(sdist < SQR(SC_SHIELD_MAG_MAX_DIST) && sdist > SQR(p->tank->radius)) {
         /* The weapon and tank are in range, so push the weapon. */
         changed = true;

         /*
            Find the force to be imparted to the missile, and scale it.
            We avoid using the evil sqrt() function by performing most
            of the calculations here working with squares.

            This is where the LORC reside.  :)  To be specific:

            o  The repulsion strength is p->shield->info->repulsion.
            o  The repulsion strength must be attenuated by distance.
               To do this, we use the square of the distance, which
               is stored in sdist, and the attenuation rate constant,
               from sshield.h, SC_SHIELD_MAG_ATTENUATION.
            o  Finally we scale the result to units of tank power with
               sshield.h's SC_SHIELD_MAG_TO_POWER constant, and from
               there to velocity units with SC_PHYSICS_VELOCITY_SCL,
               from sphysics.h.

            The result will be multiplied by the x or y distance,
            and then divided by the total distance, in order to get
            velocity differentials which can then be added into the
            current weapon velocities, *vx and *vy.
         */
         force  = p->shield->info->repulsion;
         force *= (double)SQR(SC_SHIELD_MAG_ATTENUATION) / sdist;
         force *= (double)(SC_SHIELD_MAG_TO_POWER * SC_PHYSICS_VELOCITY_SCL);

         /* Set the new partial velocities, accelerated by the magnetic shield. */
         *vx += force * dx / sqrt(sdist);
         *vy += force * dy / sqrt(sdist);

         /* Sap energy off the player's shield, based on the acceleration given. */
         if(!(traj_flags & SC_TRAJ_NO_MODIFY)) {
            p->shield->life -= rint(force / (double)(SC_PHYSICS_VELOCITY_SCL * SC_SHIELD_MAG_TO_COST));
            if(p->shield->life <= 0) {
               sc_shield_free(&p->shield);
            }
         }
      }
   }

   return(changed);

}
