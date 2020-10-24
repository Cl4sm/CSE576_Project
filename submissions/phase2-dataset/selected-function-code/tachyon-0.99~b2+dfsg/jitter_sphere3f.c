  float dx, dy, dz, len, invlen;
  /* In order to correctly sample a sphere, using rays    */
  /* generated randomly within a cube we must throw out   */
  /* direction vectors longer than 1.0, otherwise we'll   */
  /* oversample the corners of the cube relative to       */
  /* a true sphere.                                       */
  do {
    dx = rng_frand(rngh) - 0.5f;
    dy = rng_frand(rngh) - 0.5f;
    dz = rng_frand(rngh) - 0.5f;
    len = dx*dx + dy*dy + dz*dz;
  } while (len > 0.250f);
  invlen = 1.0f / sqrt(len);

  /* finish normalizing the direction vector */
  dir[0] = dx * invlen;
  dir[1] = dy * invlen;
  dir[2] = dz * invlen;
}
