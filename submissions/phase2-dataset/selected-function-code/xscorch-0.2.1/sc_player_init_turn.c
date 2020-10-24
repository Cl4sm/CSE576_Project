void sc_player_init_turn(const sc_config *c, sc_player *p) {
/* sc_player_init_turn
   Called for the player at the beginning of their turn. */

   int life;

   /* Clear any land that got stuck on our profile (sanity check) */
   /* This sanity check must not be removed, as it is the only thing
      protecting tanks from dirt bombs that might PERMANENTLY lock the
      tank into position, as a consequence. */
   sc_land_clear_profile(c, c->land, p);

   /* make sure we aren't occupying someone else's space */
   if(!sc_player_passable(c, p, p->x, p->y)) {
      fprintf(stderr, "warning: Player %d is LOCKED, cannot resolve   ** This is a bug **\n", p->index);
      fprintf(stderr, "warning: ** If there are any levitating tanks, well, this is the cause.\n");
   } /* levitation check */

   /* make sure tanks aren't levitating */
   if(sc_player_passable(c, p, p->x, p->y - 1)) {
      fprintf(stderr, "warning: Player %d is LEVITATING, cannot resolve   ** This is a bug **\n", p->index);
      fprintf(stderr, "warning: ** If there are any levitating tanks, well, this is the cause.\n");
   } /* levitation check */
   if(sc_land_support(c, c->land, p->x, p->y, p->tank->radius, p->tank->shelfsize) != 0) {
      fprintf(stderr, "warning: Player %d is UNSTABLE, cannot resolve   ** This is a bug **\n", p->index);
      fprintf(stderr, "warning: ** If there are any levitating tanks, well, this is the cause.\n");
   } /* levitation check */
   
   /* Check tank damage; update firepower if needed */
   life = INT_ROUND_DIV(p->life, p->tank->hardness);
   if(p->power > life) p->power = life;
   if(p->power < 0) p->power = 0;

   /* Update shields for the new turn. */
   sc_shield_init_turn(p);

}
