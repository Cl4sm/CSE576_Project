static sc_trajectory_result _sc_weapon_track_point(sc_config *c, sc_trajectory *tr, void *data) {
/* sc_weapon_track_point
   This function takes a current trajectory tr, and a weapon (passed in
   using the extra data argument).  This function is called when a
   weapon passes through a particular coordinate; this function clears
   any land that may have occupied the coordinate, applies "smoke" to
   the land mask if necessary, and updates hints indicating landfall
   should occur and that the screen needs to be redrawn.

   This function takes the trajectory's current x, y coordinates and
   rounds them to the nearest integer values to determine what coordinate
   the weapon currently occupies.  It will update the land mask and hints
   in tr, but it will not adjust the weapon.

   This function has no effect if the weapon is out-of-bounds.  This
   will always return SC_TRAJ_CONTINUE, indicating tracking should
   proceed.  */

   const int *gradient;    /* Sky gradient */
   int gradientflag;       /* Sky gradient flag */
   bool dither;            /* Sky: dithering allowed? */
   bool repaint = false;   /* Is a repaint needed? */
   sc_weapon *wp = (sc_weapon *)data;  /* Weapon data */
   int *trace;             /* Trace location on land */
   int x;                  /* Weapon's current X */
   int y;                  /* Weapon's current Y */

   /* Get the current land pointer at (x, y) */
   x = rint(tr->curx);
   y = rint(tr->cury);
   if(!sc_land_translate_xy(c->land, &x, &y)) return(SC_TRAJ_CONTINUE);
   trace = SC_LAND_XY(c->land, x, y);

   /* Check if the weapon just clobbered some land... */
   if(SC_LAND_IS_GROUND(*trace)) {
      /* Get the sky gradient */
      gradient = sc_land_sky_index(c);
      gradientflag = sc_land_sky_flag(c);
      dither = c->graphics.gfxdither;
      *trace = gradientflag | sc_color_gradient_index(dither, gradient, y);
      _sc_traj_landfall(tr, x);
      repaint = true;
   } /* Obliterated some land? */

   /* Check if the weapon is leaving a smoke trail */
   if((c->weapons->tracepaths || SC_WEAPON_IS_SMOKING(wp)) && !SC_CONFIG_GFX_FAST(c)) {
      *trace = SC_LAND_SMOKE | wp->playerid;
      repaint = true;
   }

   if(repaint) {
      sc_window_paint(c->window, x, y, x, y, SC_REGENERATE_LAND | SC_PAINT_EVERYTHING);
   }

   /* Nothing interesting ever happens... */
   return(SC_TRAJ_CONTINUE);

}
