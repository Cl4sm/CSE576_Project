static sc_trajectory_result _sc_traj_hit_land(const sc_config *c, sc_trajectory *tr, int flags) {
/* sc_traj_hit_ground
   Returns SC_TRAJ_IMPACT_LAND if the weapon hit the ground.
   Otherwise, the value SC_TRAJ_CONTINUE will be returned.
   On detonation, use the current coordinates of (t) to
   determine center of the explosion.  */

   int tx;           /* current X coordinate of weapon */
   int ty;           /* current Y coordinate of weapon */
   double velx;      /* revisionist X velocity of weapon */
   double vely;      /* revisionist Y velocity of weapon */
   int *lp;          /* Pointer into land structure */

   assert(c != NULL && tr != NULL);

   /* Determine current weapon coordinates */
   tx = rint(tr->curx);
   ty = rint(tr->cury);

   /* Is weapon currently on the screen? */
   if(sc_land_translate_xy(c->land, &tx, &ty)) {
      lp = SC_LAND_XY(c->land, tx, ty);
      if(!SC_LAND_IS_SKY(*lp)) {
         /* WE HAVE IMPACTED.  Either we do not allow tunneling, in
            which case the weapon must impact.  Or we do allow some
            degree of tunneling, in which case if the weapon allows
            tunnelling and we do not have contact triggers, then...
            well... sucks for the player... */
         /* TEMP - One fix for our chronic roller problems is to
            disallow tunneling here for all but flying weapons.
            I am quite happy doing this to rollers but what will it
            do to groundhogs?  -JTL */
         if((flags & SC_TRAJ_TUNNELING) && (SC_TRAJ_IS_FLYING(tr))) {
            /* Update stepping for this interpolation pass */
            /* Actually, we CANNOT update velocities during interpolation.
               All parameters are already in the context of the next interp
               vertex so we can at best revise the course at that point,
               and hope that is sufficient. */

            /* Update current real velocity of weapon */
            velx = sc_traj_get_velocity_x(tr) * c->physics->damping;
            vely = sc_traj_get_velocity_y(tr) * c->physics->damping;
            if(SQR(velx) + SQR(vely) >= SQR(SC_TRAJ_TUNNEL_MIN_VEL)) {
               /* Still moving fast enough not to detonate */
               /* NOTE - We used to revise only if TRAJ_IS_FLYING... */
               sc_traj_revise_velocity(tr, velx, vely);
               sc_traj_revise_acceleration(tr, 0, 0);
               return(SC_TRAJ_CONTINUE);
            }
         }

         /* Not tunnelling, or no longer able to tunnel. Abort. */
         return(SC_TRAJ_IMPACT_LAND);
      }
   } /* If weapon currently on the screen? */

   /* Still not much to do ... */
   /* Be sure to restore any old accelerations since we're in sky */
   if(SC_TRAJ_IS_FLYING(tr)) sc_traj_restore_acceleration(tr);
   return(SC_TRAJ_CONTINUE);

}
