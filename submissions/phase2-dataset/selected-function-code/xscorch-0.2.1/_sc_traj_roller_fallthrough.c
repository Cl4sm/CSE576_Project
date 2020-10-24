/* sc_traj_roller_fallthrough
   If falling, then we are allowed to reverse direction once we land.
   We don't care about impact type here, because we assume when this
   is called we've already checked for candidate detonations.  This
   function will adjust the velocity vector to indicate a direction
   reversal, if applicable.  At the end, the trajectory remains in the
   same location it was in on entry.  */

   /* Sanity checks */
   assert(c != NULL && tr != NULL);

   if(SC_PHYSICS_DEBUG_ROLLER) {
      printf("roller fallthru %8p  at %16g %16g  land is %4d\n",
             (void *)tr, tr->curx, tr->cury,
             sc_land_height(c->land, rint(tr->curx), c->land->height));
   }

   /* First, attempt a downward motion */
   if(_sc_traj_roller_clear(c, tr, flags, 0, -1)) {
      if(SC_PHYSICS_DEBUG_ROLLER) {
         printf("roller fallthru %8p  descent available\n", (void *)tr);
      }
      return;
   }

   /* That didn't work.  Follow the current velocity. */
   if(_sc_traj_roller_clear(c, tr, flags, SGN(tr->velx), 0)) {
      if(SC_PHYSICS_DEBUG_ROLLER) {
         printf("roller fallthru %8p  momentum continuing in direction %+d available\n",
                (void *)tr, SGN(tr->velx));
      }
      return;
   }

   /* Neither did that, hmmm.  Well, let's reverse. */
   if(_sc_traj_roller_clear(c, tr, flags, -SGN(tr->velx), 0)) {
      /* We should reverse direction now. */
      if(SC_PHYSICS_DEBUG_ROLLER) {
         printf("roller fallthru %8p  momentum reversing direction to %+d available\n",
                (void *)tr, -SGN(tr->velx));
      }
      /* TEMP - This needs attention.  If we still have roller problems, look here first.
                warning can velx sign change by calling pass_or_hit? -JDS
                warning looks like it to me (via pass_wall_elastic); is this a problem? -JTL */
      tr->velx = -tr->velx;
      return;
   }

   /* Nothing worked; return to original position */
   if(SC_PHYSICS_DEBUG_ROLLER) {
      printf("roller fallthru %8p  nothing suitable found\n", (void *)tr);
   }

}
