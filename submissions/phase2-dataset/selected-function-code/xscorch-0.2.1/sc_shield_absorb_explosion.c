int sc_shield_absorb_explosion(sc_player *p, const sc_explosion *e, int damage) {
/* sc_shield_absorb_explosion
   Try to absorb damage into a player's shield.  The damage undealt
   is returned, or zero if all damage absorbed.  */

   /* Must have a shield for it to take damage */
   if(p->shield == NULL) return(damage);

   /* Find out how much of the damage it took */
   p->shield->life -= damage;
   if(p->shield->life <= 0) {
      damage = -p->shield->life;
      sc_shield_free(&p->shield);
      if(e->type == SC_EXPLOSION_NAPALM) {
         damage = 0;
      }
   } else {
      damage = 0;
   }

   /* Return damage to the actual tank */
   return(damage);

}
