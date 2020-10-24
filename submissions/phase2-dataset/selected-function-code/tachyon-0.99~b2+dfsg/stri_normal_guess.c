static void stri_normal_guess(const stri * trn, const vector * hit, const ray * incident, vector * N) {
  flt U, V, W, lensqr, invlen;
  vector P, tmp, norm;
  
  CROSS(norm, trn->edge1, trn->edge2);
  lensqr = DOT(norm, norm); 

  VSUB((*hit), trn->v0, P);

  CROSS(tmp, P, trn->edge2);
  U = DOT(tmp, norm) / lensqr;   

  CROSS(tmp, trn->edge1, P);
  V = DOT(tmp, norm) / lensqr;   

  W = 1.0 - (U + V);

  N->x = W*trn->n0.x + U*trn->n1.x + V*trn->n2.x;
  N->y = W*trn->n0.y + U*trn->n1.y + V*trn->n2.y;
  N->z = W*trn->n0.z + U*trn->n1.z + V*trn->n2.z;

  invlen = 1.0 / sqrt(N->x*N->x + N->y*N->y + N->z*N->z);
  N->x *= invlen;
  N->y *= invlen;
  N->z *= invlen;

  /* Flip surface normal to point toward the viewer if necessary  */
  /* XXX NOTE: this is actually incorrect, but will sorta work    */
  /* for surfaces with inconsistent winding order and mean vertex */
  /* normal directions.  This implementation is provided only for */
  /* cases where the incoming geometry can't be fixed and is      */
  /* randomly mixing winding order and normal direction.          */
  if (VDot(N, &(incident->d)) > 0.0)  {
    N->x=-N->x;
    N->y=-N->y;
    N->z=-N->z;
  }
}
