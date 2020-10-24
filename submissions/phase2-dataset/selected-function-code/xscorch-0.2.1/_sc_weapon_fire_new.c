static inline bool _sc_weapon_fire_new(sc_config *c, sc_player *p, sc_explosion **e) {
/* _sc_weapon_fire_new
   A tank wants to fire a weapon.
   Called by sc_weapon_create_all, below.
   Returns true if it creates explosions. */

   sc_weapon *wp;    /* A pointer to point to the weapon to return */
   double factorx;   /* Based on turret angle; contribution to X velocity */
   double factory;   /* Based on turret angle; contribution to Y velocity */
   sc_weapon_info *i, *ti = NULL;  /* Weapon info pointers */
   double x, y, vx, vy;            /* Temporary weapon info */
   bool ct;                        /* Temporary weapon info */
   bool explosions = false;        /* Did we trigger explosions? */

   assert(c != NULL && p != NULL && e != NULL);
   /* But *e can be NULL if it wants. ;) */

   /* Make sure we can fire and are firing something */
   i = p->selweapon;
   p->weapons = NULL;
   if(p->dead || !p->armed || SC_WEAPON_IS_NULL(i))
      return(false);

   /* Calculate contribution to each direction, based on turret angle */
   factorx = cos(p->turret * M_PI / 180);
   factory = sin(p->turret * M_PI / 180);

   /* Set initial weapon position (based on turret radius and angle) */
   /* Also set initial weapon velocity based on the tank's power. */
   x = sc_player_turret_x(p, p->turret);
   y = sc_player_turret_y(p, p->turret);
   vx = p->power * SC_PHYSICS_VELOCITY_SCL * factorx;
   vy = p->power * SC_PHYSICS_VELOCITY_SCL * factory;

   /* Do we need to apply a contact trigger? */
   ct = sc_player_use_contact_trigger(c, p);

   /* Decrement player weapon inventory */
   if(!SC_WEAPON_IS_INFINITE(i) && (--(i->inventories[p->index]) <= 0)) {
      /* We ran out, so return to the default weapon. */
      p->selweapon = SC_WEAPON_DEFAULT(c->weapons);
   }

   /* If it's a triple turret type weapon and we have a triple turret, then
      we must temporarily replace its weaponinfo with a new phoenix one.
      NOTE that this info will be free'd before this function exits... */
   if((p->ac_state & SC_ACCESSORY_STATE_TRIPLE) && (i->state & SC_WEAPON_STATE_TRIPLE)) {
      ti = (sc_weapon_info *)malloc(sizeof(sc_weapon_info));
      if(ti == NULL) {
         printf("Your weapon hasn't enough memory to fire, sir!  (fire_new() malloc failure)\n");
         return(false);
      }
      memcpy((void *)ti, (void *)i, sizeof(sc_weapon_info));
      ti->ph_fl |= SC_PHOENIX_AT_TANK | SC_PHOENIX_TYPE_SCATTER;
      ti->ph_ch = i->ident;
      /* NOTE that we hard-code behavior of the triple turret here.
         It is a TRIPLE turret after all...  ;-)
         Still, this should probably change at some point. */
      ti->children = 3;
      ti->scatter = 3;
      /* Allocate the new weapon structure */
      wp = sc_weapon_new(c, ti, x, y, vx, vy, ct, p->index);
   } else {
      /* Allocate the new weapon structure */
      wp = sc_weapon_new(c, i, x, y, vx, vy, ct, p->index);
   }

   /* If it is the right phoenix type, the weapon should be split now. */
   if(SC_WEAPON_IS_PHOENIX(wp->weaponinfo) && SC_PHOENIX_IS_AT_TANK(wp->weaponinfo))
      switch(sc_phoenix(SC_PHOENIX_AT_TANK, c, &wp, e)) {
         /* This is the case we expect if it is a triple turret weapon */
         case SC_PHOENIX_SIZZLE:
            sc_weapon_landfall(c, wp);
            sc_weapon_free(&wp);
            break;

         /* This is the case we expect for detonate at_tank weapons */
         case SC_PHOENIX_DETONATE:
            sc_weapon_landfall(c, wp);
            sc_weapon_free(&wp);
            explosions = true;
            break;

         /* How I hate it when things fail... */
         case SC_PHOENIX_FAILURE:
            sc_weapon_free_chain(&wp);
            break;

         /* This is the case we expect if it is not a tank phoenix weapon. */
         case SC_PHOENIX_NO_ACTION:
            wp->weaponinfo = i; /* safety */
         default:
            /* do nothing */;
      }

   /* Set the player as owner of the weapon chain */
   p->weapons = wp;

   /* This is quite important, despite being usually free(NULL). */
   free(ti);
   return(explosions);

}
