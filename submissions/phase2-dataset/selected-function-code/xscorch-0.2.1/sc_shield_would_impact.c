                            double x, double y, double nextx, double nexty) {
/* sc_shield_would_impact
   When a shield will, to the best of our knowledge, take a hit in the
   next trajectory step.  We look to see that we will be passing from
   the outside of a player's shield to the inside. */

   double dx, dy, nextdx, nextdy, rad2, nextrad2;

   /* Sanity checks */   
   if(c == NULL || p == NULL) return(false);

   /* Make sure we have a shield here */
   if(p->shield == NULL)
      return(false);

   /* Make sure the shield is absorptive, unless we must impact it anyway */
   if(!(traj_flags & SC_TRAJ_HIT_SHIELDS) && SC_ACCESSORY_SHIELD_IS_MAGNETIC(p->shield->info))
      return(false);

   /* A player's own shield is disallowed from stopping their missiles.
      I guess you've gotta let them past in order to fire them after all... */
   if(owner != NULL && p->index == owner->index) return(false);

   /* Compute the distance to the missile's present and future. */
   if(!sc_land_calculate_deltas_d(c->land, &dx, &dy, (double)p->x, (double)p->y, x, y) ||
      !sc_land_calculate_deltas_d(c->land, &nextdx, &nextdy, (double)p->x, (double)p->y, nextx, nexty))
      return(false);

   /* Square radii, whee... */
   rad2 = SQR(dx) + SQR(dy);
   nextrad2 = SQR(nextdx) + SQR(nextdy);

   /* We must start outside the tank's shield range, and end up inside it to impact it. */
   return((rad2 >= (double)SQR(p->tank->radius + 1)) && (nextrad2 < (double)SQR(p->tank->radius + 1)));

}
