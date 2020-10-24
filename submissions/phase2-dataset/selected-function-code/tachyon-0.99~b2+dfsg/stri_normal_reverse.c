static void stri_normal_reverse(const stri * trn, const vector * hit, const ray * incident, vector * N) {
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
  /* Note: unlike the normal routines for other objects, the code */
  /*       for triangles interpolated surface normals tests the   */
  /*       vertex winding order rather than using the resulting   */
  /*       interpolated normal.                                   */
  /* Note: This version is the reverse of the normal version      */
  if (VDot(&norm, &(incident->d)) < 0.0)  {
    N->x=-N->x;
    N->y=-N->y;
    N->z=-N->z;
  }
}
