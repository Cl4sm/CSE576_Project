bool sc_shield_absorb_hit(sc_player *p, bool sapper) {
/* sc_shield_absorb_hit
   When a shield takes a direct hit. */

   /* Don't accept no substitutes! */
   if(p->shield == NULL) {
      return(false);
   }

   /* Shield sappers take more life out of a shield. */
   if(sapper) {
      p->shield->life -= SC_SHIELD_ABSORB_HIT * SC_SHIELD_SAPPER_RATE;
   } else {
      p->shield->life -= SC_SHIELD_ABSORB_HIT;
   }

   /* If the shield was completely obliterated, get rid of it. */
   if(p->shield->life <= 0) {
      sc_shield_free(&p->shield);
   }

   return(true);

}
