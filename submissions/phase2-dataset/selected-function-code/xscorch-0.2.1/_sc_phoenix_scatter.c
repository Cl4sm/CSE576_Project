static sc_phoenix_result _sc_phoenix_scatter(__libj_unused int locate, const sc_config *c, sc_weapon **wp, __libj_unused sc_explosion **e) {
/* _sc_phoenix_scatter
   Scatter a missile without detonation */

   float xscatter, yscatter;
   sc_weapon *ph_wp, *chain;
   sc_weapon_info *ph_i;
   double velx;
   double vely;
   int ph_type;
   int ph_count;

   /* What shall we reincarnate as? */
   ph_type = SC_PHOENIX_CHILD_TYPE((*wp)->weaponinfo);
   ph_count = SC_PHOENIX_CHILD_COUNT((*wp)->weaponinfo);

   /* Set some more variables. */
   ph_i = sc_weapon_lookup(c->weapons, ph_type, SC_WEAPON_LIMIT_NONE);
   chain = (*wp)->chain;
   ph_wp = *wp;

   /* Set us up to scatter the missiles.  Conceptually, this should be thought
      of as if the old weapon was actually a chain of new weapons linked together.
      This function just sets off explosions at all of the linkeages, starting
      first with the middle one(s), so that the outer weapons are given the
      largest velocity boosts (in opposing directions) and the center weapon, if
      there are an odd number, retains the old velocity.  This behavior is
      fairly consistant with the behavior of the original Scorched Earth. */
   velx = sc_traj_get_velocity_x((*wp)->tr);
   vely = sc_traj_get_velocity_y((*wp)->tr);
   if(ph_count > 1) {
      if(velx == 0 && vely == 0) {
         /* Avoid divide by zero. */
         xscatter = SC_PHOENIX_SCATTER_SCALE * SC_PHOENIX_SCATTER((*wp)->weaponinfo);
         yscatter = 0;
      } else {
         /* Find the x and y portions of the scatter. */
         xscatter = SC_PHOENIX_SCATTER_SCALE * SC_PHOENIX_SCATTER((*wp)->weaponinfo) * velx / DIST(velx, vely);
         yscatter = SC_PHOENIX_SCATTER_SCALE * SC_PHOENIX_SCATTER((*wp)->weaponinfo) * vely / DIST(velx, vely);
      }
      velx -= 0.5 * xscatter;
      vely -= 0.5 * yscatter;
      xscatter /= ph_count;
      yscatter /= ph_count;
   } else {
      xscatter = 0;
      yscatter = 0;
   }

   /* The weapons have monotonically differential velocities. */
   for(; ph_count > 0; --ph_count) {
      velx += xscatter;
      vely += yscatter;
      ph_wp->chain = sc_weapon_new(c, ph_i, (*wp)->tr->curx, (*wp)->tr->cury, velx, vely, (*wp)->triggered, (*wp)->playerid);
      ph_wp = ph_wp->chain;
   }
   ph_wp->chain = chain;

   /* sc_phoenix_scatter never detonates weapons; they just sizzle.
      This is not a mandate; it's really more of a cosmetic thing.
      It would be odd to have them exploding in mid air... */
   return(SC_PHOENIX_SIZZLE);

}
