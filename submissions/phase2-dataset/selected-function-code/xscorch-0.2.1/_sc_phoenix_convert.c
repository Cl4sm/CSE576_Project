/* sc_phoenix_convert
 * Convert a weapon to a new trajectory handler */

   sc_phoenix_result result = SC_PHOENIX_NO_ACTION;
   int ph_type;
   int ph_count;
   int flags;

   /* We'll need the weapon flags. */
   flags = SC_WEAPON_TRAJ_FLAGS(c, *wp);

   /* Maybe convert rollers. */
   if(SC_WEAPON_IS_ROLLER(*wp)) {
      /* Skip the conversion if it's already been done. */
      if(SC_TRAJ_IS_ROLLING((*wp)->tr)) {
         result = SC_PHOENIX_NO_ACTION;
         goto out;
      }
      /* Convert the trajectory to the new type. */
      if(!sc_trajectory_convert(c, (*wp)->tr, flags, SC_TRAJ_ROLLER)) {
         /* Manually detonate the failed conversion. */
         if(sc_expl_add(e, sc_weapon_get_explosion(c, *wp, (*wp)->tr->curx, (*wp)->tr->cury, SC_EXPL_DEFAULT_DIR)))
            result = SC_PHOENIX_DETONATE;
         else
            result = SC_PHOENIX_FAILURE;
         goto out;
      }
      /* We must ask the tracking code to reset itself. */
      result = SC_PHOENIX_RESET;
   }

   /* Maybe convert diggers. */
   if(SC_WEAPON_IS_DIGGER(*wp)) {
      /* Skip the conversion if it's already been done. */
      if(SC_TRAJ_IS_DIGGING((*wp)->tr)) {
         result = SC_PHOENIX_NO_ACTION;
         goto out;
      }
      /* Convert the trajectory to the new type. */
      if(!sc_trajectory_convert(c, (*wp)->tr, flags, SC_TRAJ_DIGGER)) {
         /* Manually detonate the failed conversion. */
         if(sc_expl_add(e, sc_weapon_get_explosion(c, *wp, (*wp)->tr->curx, (*wp)->tr->cury, SC_EXPL_DEFAULT_DIR)))
            result = SC_PHOENIX_DETONATE;
         else
            result = SC_PHOENIX_FAILURE;
         goto out;
      }
      /* We must ask the tracking code to reset itself. */
      result = SC_PHOENIX_RESET;
   }

   out:
   return(result);

}
