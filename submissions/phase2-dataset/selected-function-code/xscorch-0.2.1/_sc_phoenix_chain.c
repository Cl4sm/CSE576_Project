/* _sc_phoenix_chain
   Split a missile with detonation on land and bounce a child if it has any */

   double dx, dy;
   int ph_type, ph_count;
   sc_explosion *expl;
   sc_weapon_info *ph_i;
   sc_weapon *ph_wp;

   /* What shall we reincarnate as? */
   ph_type = SC_PHOENIX_CHILD_TYPE((*wp)->weaponinfo);
   ph_count = SC_WEAPON_CHILD_COUNT(*wp);

   /* Allow the final child to detonate on its own. */
   if(ph_count >= SC_PHOENIX_CHILD_COUNT((*wp)->weaponinfo))
      return(SC_PHOENIX_NO_ACTION);

   /* Return subsurface weapons to the playing field. */
   sc_land_translate_y_d(c->land, &(*wp)->tr->cury);
   if((*wp)->tr->cury <= 0) (*wp)->tr->cury = 1;

   /* Insert the next phoenix-like frog (replacing the current weapon). */
   ph_i = sc_weapon_lookup(c->weapons, ph_type, SC_WEAPON_LIMIT_NONE);
   ph_wp = sc_weapon_new(c, (*wp)->weaponinfo, (*wp)->tr->curx, (*wp)->tr->cury, sc_traj_get_velocity_x((*wp)->tr), sc_traj_get_velocity_y((*wp)->tr), (*wp)->triggered, (*wp)->playerid);
   ph_wp->state |= SC_WEAPON_STATE_DEFER;
   ph_wp->chain = (*wp)->chain;
   (*wp)->chain = ph_wp;

   /* Scale and modify values of the new frog. */
   ph_wp->exp_res = (*wp)->exp_res * SC_PHOENIX_INCREASE_FACTOR;
   dx = fabs(sc_traj_get_velocity_x((*wp)->tr)) * ph_wp->exp_res;
   dy = fabs(sc_traj_get_velocity_y((*wp)->tr)) * ph_wp->exp_res;

   /* Bounding by acceptable values. */
   if(dy < SC_PHYSICS_VELOCITY_SCL * SC_PHOENIX_MIN_FROG_POWER)
      dy = SC_PHYSICS_VELOCITY_SCL * SC_PHOENIX_MIN_FROG_POWER;
   if(dx < SC_PHYSICS_VELOCITY_SCL * SC_PHOENIX_MIN_FROG_POWER)
      dx = SC_PHYSICS_VELOCITY_SCL * SC_PHOENIX_MIN_FROG_POWER;
   if(dy > SC_PHYSICS_VELOCITY_SCL * SC_PHOENIX_MAX_FROG_POWER)
      dy = SC_PHYSICS_VELOCITY_SCL * SC_PHOENIX_MAX_FROG_POWER;
   if(dx > SC_PHYSICS_VELOCITY_SCL * SC_PHOENIX_MAX_FROG_POWER)
      dx = SC_PHYSICS_VELOCITY_SCL * SC_PHOENIX_MAX_FROG_POWER;

   /* Restore sign of X axis (Y axis must be positive). */
   if(sc_traj_get_velocity_x((*wp)->tr) < 0)
      dx *= -1;

   /* Finalize the new trajectory. */
   sc_traj_set_velocity(ph_wp->tr, dx, dy);

   /* This is the a run-time child count. */
   ph_wp->children = ++ph_count;

   /* We want the old warhead detonated now. */
   expl = sc_weapon_get_explosion(c, *wp, (*wp)->tr->curx, (*wp)->tr->cury, SC_EXPL_DEFAULT_DIR);
   if(expl == NULL) return(SC_PHOENIX_FAILURE);
   expl->radius = c->weapons->scaling * (*wp)->exp_res * ph_i->radius;
   expl->force  = ph_i->force;
   sc_expl_add(e, expl);

   return(SC_PHOENIX_DETONATE);

}
