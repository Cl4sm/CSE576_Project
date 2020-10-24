/* _sc_lscape_eval_mountain
   Evaluates the ``mountain'' profile function at point x */

   double alpha;
   double gamma;
   double y;

   /* Coerse the domain into the unit interval */
   x = sc_tools_clamp(0.0, 1.0, x);
   /* Compute the quadratic */
   alpha = 4.0 * (2.0 * sc_lscape_bump_height - 0.8);
   gamma = 0.1 + sc_lscape_bump_height;
   y = alpha * x * x - alpha * x + gamma;
   /* Add some randomness */
   y += sc_tools_pnoise_periodic(ri[0],  7,  7.0 * x + rd[0]) * 0.1;
   y += sc_tools_rnoise_periodic(ri[1], (int)sc_lscape_bump_freq, sc_lscape_bump_freq * x + rd[1]) * sc_lscape_bump_height;
   /* Coerse the range into the unit iterval (paranoid) */
   y  = sc_tools_clamp(0.0, 1.0, y);

   return(y);
}
