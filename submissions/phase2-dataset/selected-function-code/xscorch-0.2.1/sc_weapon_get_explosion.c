                                      int x, int y, double direction) {
/* sc_weapon_get_explosion
   Take a weapon and a location, and return a ready-made explosion.
   The explosion will be created with a point-of-impact at (x,y).  */

   double angular_width;
   int realradius;
   int force;
   int playerid;
   sc_explosion *expl;
   sc_explosion_type type;
   bool create_spill = false;

   assert(c != NULL && c->weapons != NULL);
   assert(wp != NULL && wp->weaponinfo != NULL);

   /* Figure out the real radius and force of the explosion */
   angular_width  = wp->weaponinfo->angular_width;
   realradius     = c->weapons->scaling * wp->exp_res * wp->weaponinfo->radius;
   force          = wp->weaponinfo->force;
   playerid       = wp->playerid;

   /* Figure out what explosion type to create. */
   if(SC_WEAPON_IS_PLASMA(wp)) {
      type = SC_EXPLOSION_PLASMA;
   } else if(SC_WEAPON_IS_NAPALM(wp)) {
      type = SC_EXPLOSION_NAPALM;
      create_spill = true;
   } else if(SC_WEAPON_IS_LIQ_DIRT(wp)) {
      /* Caution: must appear before IS_DIRT! */
      type = SC_EXPLOSION_LIQ_DIRT;
      create_spill = true;
   } else if(SC_WEAPON_IS_DIRT(wp)) {
      type = SC_EXPLOSION_DIRT;
   } else if(SC_WEAPON_IS_RIOT(wp)) {
      type = SC_EXPLOSION_RIOT;
   } else {
      type = SC_EXPLOSION_NORMAL;
   } /* Which type of explosion? */

   /* Create the initial explosion */
   if(angular_width != 0.0) {
      expl = sc_expl_new_with_angle(x, y, realradius, force,
                                 direction, angular_width,
                                 playerid, type);
   } else {
      expl = sc_expl_new(x, y, realradius, force, playerid, type);
   }
   if(expl == NULL) return(NULL);

   /* If a spill was requested, build the spill now. */
   if(create_spill) {
      expl->data = sc_spill_new(c, c->land, wp->weaponinfo->liquid,
                                expl->centerx, expl->centery);
   }

   /* Return the new explosion. */
   return(expl);

}
