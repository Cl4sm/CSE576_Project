sc_weapon *sc_weapon_new(const sc_config *c, sc_weapon_info *info,
                         double x, double y, double vx, double vy,
                         bool has_contact_trigger, int playerid) {
/* sc_weapon_new
   Create a new weapon with the parametres described.  The weapon info to
   use is in info.  The starting coordinates are in (x,y), and the starting
   velocities are in (vx,vy).  Also indicate if the weapon is using a
   contact trigger, and which player owns the weapon.  */

   sc_weapon *wp;    /* Weapon pointer */
   int flags;        /* Trajectory flags */

   assert(c != NULL);
   assert(info != NULL);
   assert(playerid >= 0 && playerid < c->numplayers);

   /* Allocate the new weapon structure */
   wp = (sc_weapon *)malloc(sizeof(sc_weapon));
   if(wp == NULL) return(NULL);

   /* Set all weapon creation defaults */
   wp->playerid      = playerid;
   wp->state         = info->state;
   wp->weaponinfo    = info;
   wp->children      = 0;
   wp->chain         = NULL;
   wp->exp_res       = 1;

   /* Are we tunnelling? This is important,
      as we must set the trajectory flags now! */
   wp->triggered     = has_contact_trigger;
   flags = SC_WEAPON_TRAJ_FLAGS(c, wp);

   /* Setup initial trajectory */
   wp->tr = sc_traj_new_velocities(c, c->players[playerid], flags, x, y, vx, vy);

   /* Return the newly created weapon. */
   return(wp);

}
