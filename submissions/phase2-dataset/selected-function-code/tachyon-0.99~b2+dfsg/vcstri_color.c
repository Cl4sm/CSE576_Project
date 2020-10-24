  vcstri_texture * tex = (vcstri_texture *) tx;
  const vcstri * trn = (vcstri *) tex->obj;
  flt U, V, W, lensqr;
  vector P, tmp, norm;
  color col;
  
  CROSS(norm, trn->edge1, trn->edge2);
  lensqr = DOT(norm, norm); 

  VSUB((*hit), trn->v0, P);

  CROSS(tmp, P, trn->edge2);
  U = DOT(tmp, norm) / lensqr;   

  CROSS(tmp, trn->edge1, P);
  V = DOT(tmp, norm) / lensqr;   

  W = 1.0 - (U + V);

  col.r = W*tex->c0.r + U*tex->c1.r + V*tex->c2.r;
  col.g = W*tex->c0.g + U*tex->c1.g + V*tex->c2.g;
  col.b = W*tex->c0.b + U*tex->c1.b + V*tex->c2.b;

  return col;
}
