void sc_lscape_explicit_setup(int w, int h, double b, sc_lscape_generator g) {
/* sc_lscape_explicit_setup
   Performs all the stuff that needs to be done before generating a
   new landscape. */
   
   int i;

   /* Set screen dimensions */
   sc_lscape_screen_w = w;
   sc_lscape_screen_h = h;

   /* Compute bump height */
   sc_lscape_bump_height = (double)SC_LSCAPE_BUMP_HEIGHT_IN_PIXELS / (2.0 * (double)h) * b;

   /* Draw random parameters */
   for(i = 0; i < SC_LSCAPE_RANDOM_PARAMETERS; i++) {
      ri[i] = (long int)game_rand();
      rd[i] = (double)game_rand() / (double)GAME_RAND_MAX;
   }

   /* Choose the landscape profile function */
   switch(g) {
   case SC_LSCAPE_GENERATOR_NONE:
      _sc_lscape_eval = _sc_lscape_eval_none;
      break;
   case SC_LSCAPE_GENERATOR_CANYON:
      _sc_lscape_eval = _sc_lscape_eval_canyon;
      break;
   case SC_LSCAPE_GENERATOR_DOUBLE_MOUNTAIN :
      _sc_lscape_eval = _sc_lscape_eval_mountain;
      break;
   case SC_LSCAPE_GENERATOR_HILLSIDE :
      _sc_lscape_eval = _sc_lscape_eval_hillside;
      break;
   case SC_LSCAPE_GENERATOR_MOUNTAIN :
      _sc_lscape_eval = _sc_lscape_eval_mountain;
      break;
   case SC_LSCAPE_GENERATOR_PLAINS :
      _sc_lscape_eval = _sc_lscape_eval_plains;
      break;
   case SC_LSCAPE_GENERATOR_TRADITIONAL :
      _sc_lscape_eval = _sc_lscape_eval_traditional;
      break;
   case SC_LSCAPE_GENERATOR_VALLEY :
      _sc_lscape_eval = _sc_lscape_eval_valley;
      break;
   }
}
