void sc_lscape_setup(const struct _sc_config *c, const struct _sc_land *l) {
/* sc_lscape_setup
   Front-end to sc_lscape_explicit_setup.  Note: the internals of this
   function are bound to change quite a bit. */

   int i;
   sc_land_generator gen;
   
   if(l != NULL) {
      /* Choose the lanscape profile function */
      gen = l->generator;
      if(gen == SC_LAND_GEN_RANDOM) gen = game_lrand(SC_LAND_GEN_RANDOM);
      switch(gen) {
      case SC_LAND_GEN_NONE :
         _sc_lscape_eval = _sc_lscape_eval_none;
         break;
      case SC_LAND_GEN_CANYON :
         _sc_lscape_eval = _sc_lscape_eval_canyon;
         break;
      case SC_LAND_GEN_DOUBLE_MOUNTAIN :
         _sc_lscape_eval = _sc_lscape_eval_mountain;
         break;
      case SC_LAND_GEN_HILLSIDE :
         _sc_lscape_eval = _sc_lscape_eval_hillside;
         break;
      case SC_LAND_GEN_MOUNTAIN :
         _sc_lscape_eval = _sc_lscape_eval_mountain;
         break;
      case SC_LAND_GEN_PLAINS :
         _sc_lscape_eval = _sc_lscape_eval_plains;
         break;
      case SC_LAND_GEN_RANDOM :
      case SC_LAND_GEN_TRADITIONAL :
         _sc_lscape_eval = _sc_lscape_eval_traditional;
         break;
      case SC_LAND_GEN_VALLEY :
         _sc_lscape_eval = _sc_lscape_eval_valley;
         break;
      }

      /* Get screen dimensions */
      sc_lscape_screen_w = c->fieldwidth;
      sc_lscape_screen_h = c->maxheight;

      /* Compute sc_lscape_bump_height */
      sc_lscape_bump_height = SC_LSCAPE_BUMP_HEIGHT_IN_PIXELS / (2.0 * sc_lscape_screen_h) * sc_tools_clamp(0.0, 1.0, l->bumpiness / 100.0);
      sc_lscape_bump_freq = floor(sc_lscape_screen_w / SC_LSCAPE_BUMP_WIDTH_IN_PIXELS);

      /* Draw new random parameters */
      for(i = 0; i < SC_LSCAPE_RANDOM_PARAMETERS; i++) {
         ri[i] = game_rand();
         rd[i] = (double)game_rand() / (double)GAME_RAND_MAX;
      }
   }
}
