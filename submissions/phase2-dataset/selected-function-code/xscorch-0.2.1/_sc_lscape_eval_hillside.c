static double _sc_lscape_eval_hillside(double x) {
/* _sc_lscape_eval_hillside
   Evaluates the ``hillside'' profile function at point x. */

   double alpha;
   double beta;
   double y;

   /* Coerse the domain into the unit interval */
   x = sc_tools_clamp(0.0, 1.0, x);
   /* Figure out coefficients */
   alpha = rd[0] > 0.5 ? 0.1 + sc_lscape_bump_height : 0.9 - sc_lscape_bump_height;
   beta  = 1 - 2.0 * alpha;
   /* Compute height along the line */
   y = alpha + beta * x;
   /* Add some randomness */
   y += sc_tools_pnoise(ri[0], 7.0 * x + rd[0]) * 0.1;
   y += sc_tools_rnoise(ri[1], sc_lscape_bump_freq * x + rd[1]) * sc_lscape_bump_height;
   /* Coerse the range into the unit iterval (paranoid) */
   y = sc_tools_clamp(0.0, 1.0, y);

   return(y);
}
