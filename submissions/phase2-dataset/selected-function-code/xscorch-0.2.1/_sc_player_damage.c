/* sc_player_damage */

   int damage;

   /* If player is already dead, then there's nothing to do */
   if(!SC_PLAYER_IS_ALIVE(p)) return;

   /* How much damage was taken? */
   damage = sc_expl_damage_at_point(c->land, e, p->x, p->y);
   if(damage <= 0) return;

   /* Check if the shields absorbed the explosion for us. */
   damage = sc_shield_absorb_explosion(p, e, damage);
   if(damage <= 0) return;

   /* Take any remaining damage ourselves */
   p->life -= damage * SC_TANK_NORMAL_HARDNESS;

   /* Revive shields if necessary (and if auto-defense enabled) */
   if(SC_PLAYER_IS_ALIVE(p)) {
      sc_player_activate_auto_shield(c, p);
   }

   /* Tank took a direct or partial hit.  Damn. */
   p->money -= c->economics->damageloss;
   if(!SC_PLAYER_IS_ALIVE(p)) {
      p->life = 0;  /* We died */
      p->killedby = e->playerid;
      /* Who killed us? */
      if(e->playerid == p->index) {
         /* It was a suicide */
         ++c->players[e->playerid]->suicides;
         p->money -= c->economics->suicideloss;
      } else {
         /* Give opponent some points for killing us */
         ++c->players[e->playerid]->kills;
         c->players[e->playerid]->money += c->economics->killbonus;
      }
   }

   if(e->playerid != p->index) {
      /* Give points to other player for damaging us */
      c->players[e->playerid]->money += c->economics->damagebonus;
   } /* Make sure not self */

}
