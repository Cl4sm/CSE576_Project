  vector R;   /* reflection vector      */
  vector V;   /* direction to viewpoint */
  vector LL;  /* reverse direction to light     */
  flt inten;  /* calculated intensity   */

  LL = shadevars->L;
  VScale(&LL, -1.0);
  VAddS(-2.0 * (LL.x * shadevars->N.x + 
                LL.y * shadevars->N.y + 
                LL.z * shadevars->N.z), &shadevars->N, &LL, &R);

  V = incident->d;
  VScale(&V, -1.0);
  VNorm(&R);            /* normalize reflection vector */
  inten = VDot(&V, &R); /* dot product of halfway vector and surface normal */
  if (inten > 0.0)     
    inten = pow(inten, specpower);
  else 
    inten = 0.0;

  return inten;
} 
