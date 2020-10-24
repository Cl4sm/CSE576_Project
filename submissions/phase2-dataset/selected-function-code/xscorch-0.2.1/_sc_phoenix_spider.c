/* _sc_phoenix_spider
   Split a missile with detonation on land and give random destinations */

   int i, j, ph_type, ph_count, expl_count, leg_radius, leg_force, scatter;
   sc_explosion *cure; /* for CURrent Explosion ;) */
   sc_weapon_info *ph_i;
   double srcx;
   double srcy;

   /* What shall we reincarnate as? */
   ph_type = SC_PHOENIX_CHILD_TYPE((*wp)->weaponinfo);
   ph_count = SC_PHOENIX_CHILD_COUNT((*wp)->weaponinfo);

   ph_i = sc_weapon_lookup(c->weapons, ph_type, SC_WEAPON_LIMIT_NONE);
   /* Random number of explosions based on no less than half of child count. */
   expl_count = ph_count / 2 + game_lrand(ph_count / 2);
   leg_radius = ph_i->radius;
   leg_force = ph_i->force;
   /* Radius of blast. */
   scatter = SC_PHOENIX_SCATTER((*wp)->weaponinfo) * (*wp)->exp_res *
             (*wp)->weaponinfo->radius * SC_PHOENIX_SPIDER_POWER;

   /* First the old weapon explodes (we hope)... */
   if(!sc_expl_add(e, sc_weapon_get_explosion(c, *wp, (*wp)->tr->curx, (*wp)->tr->cury, SC_EXPL_DEFAULT_DIR)))
      goto failure;

   /* Start loop on number of spider legs (child explosions). */
   for(i = 0; i < expl_count; i++) {
      /* Create a new explosion. */
      cure = sc_weapon_get_explosion(c, *wp, 0, 0, SC_EXPL_DEFAULT_DIR);
      if(cure == NULL) goto failure;
      cure->radius = c->weapons->scaling * (*wp)->exp_res * leg_radius;
      cure->force  = leg_force;
      cure->type   = SC_EXPLOSION_SPIDER;

      /* Pick a random x within blast radius.  The wall rules and location validation
         are performed in the low-level land manipulation and trajectory functions now.
         Also, we make sure to bail out if we try many legs and they all fail validity.
         In this case, we will simply make (at least) one fewer child explosion(s). */
      for(j = 0; j < SC_PHOENIX_SPIDER_MAX_LEG_ATTEMPTS; ++j) {
         /* Select what we hope is a reasonable center. */
         cure->centerx = (*wp)->tr->curx - scatter + game_lrand(2 * scatter);
         /* Locate the y location corresponding to our x. */
         cure->centery = sc_land_height(c->land, cure->centerx, c->fieldheight);
         /* Valid spider leg found? */
         if(cure->centery <= (*wp)->tr->cury + SC_PHOENIX_SPIDER_ARC) {
            /* Create arc. */
            srcx = (*wp)->tr->curx;
            srcy = sc_land_height(c->land, srcx, c->fieldheight);
            if((*wp)->tr->cury > srcy) srcy = (*wp)->tr->cury;
            cure->data = sc_traj_new_dest_height(c, c->players[(*wp)->playerid], SC_TRAJ_IGNORE_WIND | SC_TRAJ_IGNORE_VISC | SC_TRAJ_TERMINUS, srcx, srcy, cure->centerx - srcx, cure->centery - srcy, SC_PHOENIX_SPIDER_ARC);
            /* Add explosion to explosion chain. */
            sc_expl_add(e, cure);
            /* Avoid free(cure) when we've added it to e. */
            cure = NULL;
            break;
         }
      } /* maximum leg attempts */

      /* If we utterly failed to create this leg, free its explosion. */
      if(cure != NULL) sc_expl_free(&cure);
   } /* expl_count */

   /* We have detonated the initial weapon and (hopefully) some children. */
   return(SC_PHOENIX_DETONATE);

   failure:
   if(cure != NULL) sc_expl_free(&cure);
   return(SC_PHOENIX_FAILURE);

}
