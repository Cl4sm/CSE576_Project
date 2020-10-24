static sc_trajectory_result _sc_traj_step_flight(sc_config *c, sc_trajectory *tr, int flags,
                                                 sc_trajectory_action action, void *data) {
/* sc_traj_step_flight */

   sc_trajectory_result result;  /* Result of this function */
   double locx, locy;            /* Location buffers */
   double velx, vely;            /* Velocity buffers */

   assert(c != NULL && tr != NULL);

   while(tr->timestep <= tr->finalstep) {
      tr->oldx = tr->curx;
      tr->oldy = tr->cury;
      tr->curx = sc_traj_get_current_x(tr);
      tr->cury = sc_traj_get_current_y(tr);
      result = _sc_traj_traverse_path(c, tr, flags, action, data);
      /* This test turns a shield impact into a rebound for force shields */
      if(result == SC_TRAJ_IMPACT_SHIELD && !(flags & SC_TRAJ_HIT_SHIELDS)) {
         /* We want the real location of the weapon, not the interpolated one here. */
         locx = tr->curx;
         locy = tr->cury;
         /* The real velocities of the weapon would be nice but aren't known... */
         velx = sc_traj_get_velocity_x(tr);
         vely = sc_traj_get_velocity_y(tr);
         /* Ask the shield if it will be rebounding this missile. */
         if(sc_shield_get_reflection(c, c->players[tr->victim], flags,
                                     &locx, &locy, &velx, &vely)) {
            sc_traj_new_control_equation(tr, locx, locy, velx, vely);
            result = SC_TRAJ_CONTINUE;
            tr->curx = locx;
            tr->cury = locy;
            tr->victim = -1;
         }
      }
      if(result != SC_TRAJ_CONTINUE) return(result);
      if(tr->timestep < tr->finalstep) {
         tr->timestep = min(tr->timestep + SC_TRAJ_TIME_STEPS_PER_SUBSTEP, tr->finalstep);
      } else {
         tr->timestep += SC_TRAJ_TIME_STEPS_PER_SUBSTEP;
      }

      /* Perform actions normally done at the interpolation vertices here */
      if(!(flags & SC_TRAJ_IGNORE_TANK)) {
         /* Perform magnetic shield deflection. */
         velx = sc_traj_get_velocity_x(tr);
         vely = sc_traj_get_velocity_y(tr);
         if(sc_shield_get_deflection(c, tr->player, flags,
                                     sc_traj_get_current_x(tr),
                                     sc_traj_get_current_y(tr),
                                     &velx, &vely))
            sc_traj_revise_velocity(tr, velx, vely);
      }
   }
   return(SC_TRAJ_CONTINUE);

}
