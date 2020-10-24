                              double *x, double *y, double *velx, double *vely) {
/* sc_shield_get_reflection
   Find the new reflection in velocity of a missile off a force shield. */

   double dx, dy, radial;

   /* This is only for force shields. */
   if(p->shield == NULL || !SC_ACCESSORY_SHIELD_IS_FORCE(p->shield->info))
      return(false);

   /* If calculate_deltas fails somehow, so do we. */
   if(!sc_land_calculate_deltas_d(c->land, &dx, &dy, (double)p->x, (double)p->y, (*x), (*y)))
      return(false);

   /*
      To perform a reflection, we must reverse the radial velocity
      component.

      First we calculate the radial component of the two vectors
      (the velocity of the impacting missile and the radial vector
      from the center of the tank to the impact at the shield).

      Second, we subtract out twice the radial velocity, to reflect
      the missile off the shield surface at that point.

      The calculations skirt around the need for a sqrt() calculation
      by factoring a division by L = sqrt(SQR(dx) + SQR(dy)) outside
      of the radial, where it cancels in the final subtraction.
   */

   /* Find the radial velocity component through a dot product. */
   radial = (dx * (*velx) + dy * (*vely)) / (SQR(dx) + SQR(dy));

   /* Subtract 2x the perpendicular component to reverse it. */
   *velx -= 2 * radial * dx;
   *vely -= 2 * radial * dy;

   /* Move the weapon just (0.1 pixels) outside the shield (p->tank->radius + 1).
      It's difficult to get gcc to do this with the required precision.  :(  */
   *x = (double)p->x + dx * SQR((double)1.1 + (double)p->tank->radius) / (SQR(dx) + SQR(dy));
   *y = (double)p->y + dy * SQR((double)1.1 + (double)p->tank->radius) / (SQR(dx) + SQR(dy));

   /* Sap the shield. */
   if(!(traj_flags & SC_TRAJ_NO_MODIFY))
      sc_shield_absorb_hit(p, (traj_flags & SC_TRAJ_HIT_SHIELDS));

   return(true);

}
