                           int flags, sc_trajectory_type type) {
/* sc_trajectory_convert
   Convert from one trajectory type to another. Not all possible conversions
   are supported right now.  This is generally used when a warhead in flight
   that contains a roller/digger impacts; the trajectory must be converted
   from flight mode to roller/digger as appropriate...

   The original trajectory is given in tr; the type to convert *to* is given
   in type.  The physics of the original trajectory will be converted over
   in tr itself.  If the conversion is illegal, this function returns false.  */

   int height;                /* Height of land */
   double slope;              /* Slope of nearby land */

   assert(c != NULL);
   if(tr == NULL) return(false);

   /* Figure out what conversion is occurring */
   switch(tr->type) {
   case SC_TRAJ_FLIGHT:
      /* Originally a flying weapon */
      switch(type) {
      case SC_TRAJ_FLIGHT:
         tr->ctrx = tr->curx;
         tr->ctry = tr->cury;
         break;

      case SC_TRAJ_ROLLER:
         /* Note that tr->lastx, tr->lasty indicate the last
            coordinate that was NOT an impact.  We should start
            the rolling from there, since tr->curx, tr->cury
            reflect the actual point of impact. */

         /* Also fail the conversion if we've hit a tank. */
         if(tr->victim >= 0) {
            return(false);
         }

         /* Update the static data based on current dynamic info */
         tr->type = SC_TRAJ_ROLLER;
         tr->ctrx = tr->lastx;
         tr->ctry = tr->lasty;

         /* Find the rough slope of the local land. */
         slope = sc_land_height(c->land, tr->ctrx - 1, tr->lasty + 1) -
                 sc_land_height(c->land, tr->ctrx + 1, tr->lasty + 1);
         slope /= 2.0;

         /* If the weapon velocity and the slope oppose, reverse it. */
         if(tr->velx != 0 && slope / tr->velx > 0) {
            tr->velx = -tr->velx;
         }

         /* When rolling, we don't keep track of vely. */
         tr->vely = 0;

         /* Reinitialise the weapon */
         _sc_traj_reset(tr);
         _sc_traj_roller_fallthrough(c, tr, flags);
         break;

      case SC_TRAJ_DIGGER:
         /* Update the static data based on current dynamic info */
         tr->type = SC_TRAJ_DIGGER;
         tr->ctrx = tr->curx;
         tr->ctry = tr->cury;
         height = sc_land_height(c->land, tr->ctrx, c->land->height);
         if(height > tr->ctry) tr->ctry = height;
         tr->vely = 0;

         /* Reinitialise the weapon */
         _sc_traj_reset(tr);
         _sc_traj_digger_reselect_dir(c, tr, flags);
         break;
      } /* End of flight->? case */
      break;

   case SC_TRAJ_ROLLER:
      /* Rollers cannot convert. */
      fprintf(stderr, "sc_trajectory_convert: Illegal conversion ignored.\n");
      return(false);

   case SC_TRAJ_DIGGER:
      /* Diggers cannot convert. */
      fprintf(stderr, "sc_trajectory_convert: Illegal conversion ignored.\n");
      return(false);
   }

   /* At this point, do sc_traj_run as if nothing ever happened... */
   return(true);

}
