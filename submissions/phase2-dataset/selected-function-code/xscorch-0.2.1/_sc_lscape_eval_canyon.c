/* _sc_lscape_eval_canyon
   Evaluates the ``canyon'' profile function at point x. */

   double y;
   double delta;

   /* Coerse the domain into the unit interval */
   x = sc_tools_clamp(0.0, 1.0, x);

   delta = 1.0 - (0.2 + 2.0 * sc_lscape_bump_height);
   y = sc_tools_smoothstep(-0.25, 0.25, sc_tools_pnoise_periodic(ri[0], 7, 7.0 * x + rd[0])) * delta;
   y += sc_tools_pnoise_periodic(ri[1], 7, 7.0 * x + rd[1]) * 0.1;
   y += sc_tools_rnoise_periodic(ri[2], (int)sc_lscape_bump_freq, sc_lscape_bump_freq * x + rd[2]) * sc_lscape_bump_height;
   y += sc_lscape_bump_height + 0.1;

   y = sc_tools_clamp(0.0, 1.0, y);

   return(y);
}
