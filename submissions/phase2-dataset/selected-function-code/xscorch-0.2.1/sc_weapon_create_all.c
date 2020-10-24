bool sc_weapon_create_all(sc_config *c, sc_explosion **e) {
/* sc_weapon_create_all
   Create the weapons for each player, to be launched.
   Returns true if we already have explosions to run... */

   bool explosions = false;
   sc_player *p;
   int i;

   for(i = c->numplayers - 1; i >= 0; --i) {
      p = c->players[i];
      if(!p->dead && p->armed) {
         if(_sc_weapon_fire_new(c, p, e)) explosions = true;
         p->armed = false;
      }
   }

   return(explosions);

}
