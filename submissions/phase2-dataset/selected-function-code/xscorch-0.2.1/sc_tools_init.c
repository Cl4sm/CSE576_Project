{
  long int i;
  double t;
  double t_squared;
  double t_cubed;

#ifdef SC_TOOLS_DEBUG
  printf("sc_tools_init: Computing random points and tangents ..\n");
#endif /* SC_TOOLS_DEBUG */

  /* Compute tables for random points and tangents */
  for ( i = 0; i < SC_TOOLS_RANDTABLE_SIZE; i++ ) {
    p_table[i] = 2.0 * (double)game_rand() / (double)GAME_RAND_MAX - 1.0;
    r_table[i] = 8.0 * (double)game_rand() / (double)GAME_RAND_MAX - 4.0;
  }

#ifdef SC_TOOLS_DEBUG
  printf("sc_tools_init: Computing look-up tables for Hermite blending function ..\n", i);
#endif /* SC_TOOLS_DEBUG */

  /* Compute look-up tables for the multipliers of p1, p2, t1, and t2 
   * in the Hermite blending function */
  for ( i = 0; i < SC_TOOLS_LOOKUP_SIZE; i++ ) {
    t = i / (SC_TOOLS_LOOKUP_SIZE - 1.0);
    t_squared = t * t;
    t_cubed = t * t_squared;
    p1_mult_lookup[i] = 2.0 * t_cubed - 3.0 * t_squared + 1;
    p2_mult_lookup[i] = -2.0 * t_cubed + 3.0 * t_squared;
    r1_mult_lookup[i] = t_cubed - 2.0 * t_squared + t;
    r2_mult_lookup[i] = t_cubed - t_squared;
  }
}
