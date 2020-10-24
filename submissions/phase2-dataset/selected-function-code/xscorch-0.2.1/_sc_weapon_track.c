/* sc_weapon_track
   Track a single weapon component.  This function undraws the weapon at
   its current position ((*wp)->tr->cur{x,y}).  Then, it tracks the weapon
   to its new location, or to the point of impact if the weapon hit any-
   thing.  Impacts are dealt with, deleting the current weapon if needed,
   and adding an explosion to e.

   wp is a list; we are only tracking the weapon at the HEAD of the list
   but we may append extra warheads to the tail if needed (phoenix code
   will do this).  Similarly, e is a list; we may append new explosions
   to it at the tail of the list.

   A fair amount of phoenix processing is done here to determine if the
   weapon hit its apex and should break apart (for MIRVs, etc).  */

   sc_trajectory_result stepres;    /* Indicates result of sc_traj_step */
   sc_trajectory *traj;             /* Shortcut to (*wp)->traj. */
   bool goingup;                    /* True if weapon initially ascending */
   int flags;                       /* Flags for if we are tunnelling, etc */

   assert(c != NULL && wp != NULL && e != NULL);
   assert(*wp != NULL);

   /* We don't want to process this weapon now if it's deferring.
      Instead, we'll mark it to be processed next time and let the
      tracking state know it needs to keep processing the weapon. */
   if(SC_WEAPON_IS_DEFERRING(*wp)) {
      (*wp)->state &= ~SC_WEAPON_STATE_DEFER;
      return(SC_WEAPON_TRACK_NEED_RECURSE);
   }

   /* At this point, traj->cur{x,y} indicate the original control
      point; the point that the weapon was at upon entering this
      function. */
   traj = (*wp)->tr;

   sc_window_undraw_weapon(c->window, *wp);
   goingup = sc_traj_get_velocity_y(traj) >= 0;
   if(traj->timestep == 0) goingup = true;

   /* Are we tunnelling? This is important,
      as we must set the trajectory flags now! */
   flags = SC_WEAPON_TRAJ_FLAGS(c, *wp);

   /* WEAPON collision check;  since the weapon is likely to be skipping
      more than one pixel away per iteration, we need to make sure it
      wouldn't have collided with something along the way.  We need to
      check all points between the previous and current position.  We do
      this with a line approximation, which is well, good enough...  */
   stepres = sc_traj_step(c, traj, flags, _sc_weapon_track_point, *wp);

   /* At this point, the values traj->cur{x,y} either indicate the new
      control point, or the point of impact if the weapon hit something.  */

   /* Deal damage to shields if appropriate */
   if(stepres == SC_TRAJ_IMPACT_SHIELD && !SC_WEAPON_IS_LIQUID(*wp)) {
      sc_shield_absorb_hit(c->players[traj->victim], SC_WEAPON_IS_SAPPER(*wp) ? 1 : 0);
      stepres = SC_TRAJ_SIZZLE;
   }

   /* This code checks if we just detonated.  If we did, it checks whether
      the weapon is a LAND phoenix.  If so, it calls sc_phoenix() which
      checks which phoenix subroutine to run on the weapon. */
   if(SC_TRAJ_IS_IMPACT(stepres) && SC_PHOENIX_IS_AT_LAND((*wp)->weaponinfo)) {
      switch(sc_phoenix(SC_PHOENIX_AT_LAND, c, wp, e)) {
         /* Weapon hit something, exploded, and also created children */
         case SC_PHOENIX_DETONATE:
            sc_weapon_landfall(c, *wp);
            sc_weapon_free(wp);
            return(SC_WEAPON_TRACK_DETONATE);

         /* Weapon hit something and created children but we call it a sizzle
            because we don't detonate the original weapon.  Note that this is
            never done in the original Scorched Earth. */
         case SC_PHOENIX_SIZZLE:
            sc_weapon_landfall(c, *wp);
            sc_weapon_free(wp);
            return(SC_WEAPON_TRACK_SIZZLE);

         /* We need to skip tracking the weapon this turn */
         case SC_PHOENIX_RESET:
            /* Draw the weapon at its new position */
            sc_window_draw_weapon(c->window, *wp);
            /* Defer to next tracking round */
            return(SC_WEAPON_TRACK_NEED_RECURSE);

         /* There was an error (oom, for example) in sc_phoenix() */
         case SC_PHOENIX_FAILURE:

         /* The default case should not arise (we hope) */
         default:
            /* do nothing */;
      }
   }

   /* Decide what to do based on the return value of the path traversal */
   if(SC_TRAJ_IS_IMPACT(stepres)) {
      /* Weapon hit something, and detonated */
      if(*e == NULL) {
         sc_expl_add(e, sc_weapon_get_explosion(c, *wp, traj->curx, traj->cury, SC_EXPL_DEFAULT_DIR));
         sc_weapon_landfall(c, *wp);
         sc_weapon_free(wp);
         return(SC_WEAPON_TRACK_DETONATE);
      } else {
         /* Draw the weapon at its new position.  Probably not needed... */
         sc_window_draw_weapon(c->window, *wp);
         /* We deferred */
         return(SC_WEAPON_TRACK_NEED_RECURSE);
      }
   } else if(stepres == SC_TRAJ_SIZZLE) {
      /* Weapon sizzled */
      sc_weapon_landfall(c, *wp);
      sc_weapon_free(wp);
      return(SC_WEAPON_TRACK_SIZZLE);
   } /* Any special orders? */

   /* This code checks if we're at the apex of the weapon's flight curve.
      If we are and this is an APEX phoenix weapon, it calls sc_phoenix()
      which checks which phoenix function to run on the weapon.  For
      reasons of timing it is important to interrupt sc_weapon_track. */
   if(goingup && sc_traj_get_velocity_y(traj) <= 0 && SC_PHOENIX_IS_AT_APEX((*wp)->weaponinfo)) {
      switch(sc_phoenix(SC_PHOENIX_AT_APEX, c, wp, e)) {
         /* In this case the weapon split successfully */
         case SC_PHOENIX_SIZZLE:
            sc_weapon_landfall(c, *wp);
            sc_weapon_free(wp);
            return(SC_WEAPON_TRACK_SIZZLE);

         /* In this case, we detonated in mid-air.  Strange... */
         case SC_PHOENIX_DETONATE:
            sc_weapon_landfall(c, *wp);
            sc_weapon_free(wp);
            return(SC_WEAPON_TRACK_DETONATE);

         /* In this case something is probably very wrong, since phoenix failed */
         case SC_PHOENIX_FAILURE:

         /* In this case, nothing happened */
         case SC_PHOENIX_NO_ACTION:
         default:
            /* do nothing */;
      }
   }

   /* Check for RAND phoenix weapons and if so check whether now is the time to run them */
   /* TEMP: the probability should be selectable or perhaps scaled by the expected arc length? */
   if(SC_PHOENIX_IS_AT_RAND((*wp)->weaponinfo) && game_drand() < SC_PHOENIX_PROB_AT_RAND) {
      switch(sc_phoenix(SC_PHOENIX_AT_RAND, c, wp, e)) {
         /* In this case the weapon was successfully modified by sphoenix */
         case SC_PHOENIX_SIZZLE:
            sc_weapon_landfall(c, *wp);
            sc_weapon_free(wp);
            return(SC_WEAPON_TRACK_SIZZLE);

         /* In this case, we detonated */
         case SC_PHOENIX_DETONATE:
            sc_weapon_landfall(c, *wp);
            sc_weapon_free(wp);
            return(SC_WEAPON_TRACK_DETONATE);

         /* Failure in the sphoenix code */
         case SC_PHOENIX_FAILURE:

         /* No action to perform; we hope this case doesn't arise */
         case SC_PHOENIX_NO_ACTION:
         default:
            /* do nothing */;
      }
   }

   /* If we're still running, then the weapon is still live.  Go ahead
      and re-draw it at its new position. */
   sc_window_draw_weapon(c->window, *wp);

   /* We were tracking a weapon; we'll need to recurse. */
   return(SC_WEAPON_TRACK_NEED_RECURSE);

}
