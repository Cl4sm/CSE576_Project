flt shade_blinn(const ray * incident, const shadedata * shadevars, flt specpower) {
  vector H;   /* Blinn's halfway vector */
  flt inten;  /* calculated intensity   */

  /* since incident ray is negated direction to viewer, we subtract... */
  /* sub. incoming ray dir. from light direction */
  H.x = shadevars->L.x - incident->d.x; 
  H.y = shadevars->L.y - incident->d.y;
  H.z = shadevars->L.z - incident->d.z;

  inten = shadevars->N.x * H.x + shadevars->N.y * H.y + shadevars->N.z * H.z;
  if (inten > MINCONTRIB) {
    /* normalize the previous dot product */
    inten /= sqrt(H.x * H.x + H.y * H.y + H.z * H.z);

    /* calculate specular exponent */
    inten = pow(inten, specpower);
  } else {
    inten = 0.0;
  }

  return inten;
}
