void sc_player_init_round(sc_config *c, sc_player *p) {
/* sc_player_init_round
   Called once for each player at the beginning of any round. */

   /* Determine the tank's position on the field */
   p->field_index = game_lrand(c->numplayers);
   while(c->field_position[p->field_index] >= 0) {
      p->field_index = (p->field_index + 1) % c->numplayers;
   }
   c->field_position[p->field_index] = p->index;

   /* Initialise player X, Y coordinates -- we may need to modify
      the land a bit so the tank is initially on flat ground.  */
   p->x = rint((p->field_index + 1) * c->fieldwidth / (c->numplayers + 1.0));
   p->y = sc_land_avg_height_around(c->land, p->x, c->fieldheight, p->tank->radius);
   sc_land_level_around(c, c->land, p->x, p->tank->radius, p->y);

   /* Setup player turret and initial power */
   p->turret = game_lrand(181);
   p->power  = SC_PLAYER_MAX_POWER / 5;

   /* Set initial life status
      Scale from game life units to this tank's life units. */
   p->life = SC_PLAYER_MAX_LIFE * p->tank->hardness;
   p->dead = false;
   p->armed = false;
   p->killedby = -1;

   /* Setup fuel level */
   p->fuel = 0;

   /* Clear contact triggers */
   p->contacttriggers = false;

   /* Load weapons and accessories */
   p->selweapon = SC_WEAPON_DEFAULT(c->weapons);
   p->selshield = SC_ACCESSORY_DEFAULT(c->accessories);
   sc_weapon_free_chain(&p->weapons);
   sc_shield_free(&p->shield);

   /* Money is as it was at beginning of the round */
   p->oldmoney = p->money;

   /* Initialise AI */
   sc_ai_init_round(c, p);

}
