/* sc_expl_damage_at_point
   Returns the amount of damage (in units comparable to life), done by the
   specified explosion to an object centered at (x, y).  If no damage is
   done, zero is returned.  */

   sc_spill *sp;     /* Spill data (for napalm damages) */
   int distance;     /* Distance between explosion center and (x,y) (squared) */
   int damage;       /* Actual amount of damage done */
   int deltax;       /* Change in X */
   int deltay;       /* Change in Y */
   int i;            /* Iterator */

   /* Determine the damaging effect of the weapon */
   damage = 0;
   switch(e->type) {
      case SC_EXPLOSION_SPIDER:
      case SC_EXPLOSION_NORMAL:
      case SC_EXPLOSION_PLASMA:
         /* Calculate distance */
         sc_land_calculate_deltas(l, &deltax, &deltay, x, y, e->centerx, e->centery);
         distance = SQR(deltax) + SQR(deltay);

         /* Calculate the overall damage */
         damage = rint(e->force * (1 - distance / (double)SQR(e->radius)));
         break;

      case SC_EXPLOSION_NAPALM:
         /* Integrate over all points... */
         sp = (sc_spill *)e->data;
         for(i = 0; i < sp->size; ++i) {
            sc_land_calculate_deltas(l, &deltax, &deltay, sp->spillx[i], sp->spilly[i], x, y);
            distance = SQR(deltax) + SQR(deltay);
            distance = rint(e->force * (1 - distance / (double)SQR(e->radius)));
            if(distance < 0) distance = 0;
            damage += distance;
         } /* Iterate through all points in damage zone */
         break;

      case SC_EXPLOSION_LIQ_DIRT:
      case SC_EXPLOSION_DIRT:
      case SC_EXPLOSION_RIOT:
         /* Dirt balls/riots do no damage to tanks */
         break;

   } /* End switch to select radius */

   if(damage < 0) damage = 0;
   return(damage);

}
