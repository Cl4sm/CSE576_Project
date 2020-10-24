  vector pnt;
  flt u, v, w, miprad, maxscale;
  standard_texture * tex = (standard_texture *) tx;
 
  pnt.x=hit->x - tex->ctr.x;
  pnt.y=hit->y - tex->ctr.y;
  pnt.z=hit->z - tex->ctr.z;

  VDOT(u, tex->uaxs, pnt);
  VDOT(v, tex->vaxs, pnt); 
  VDOT(w, tex->waxs, pnt); 

  u = u * tex->scale.x;
  u = u + tex->rot.x;
  u = u - ((int) u);
  if (u < 0.0) u += 1.0;

  v = v * tex->scale.y;
  v = v + tex->rot.y;
  v = v - ((int) v);
  if (v < 0.0) v += 1.0;

  w = w * tex->scale.z;
  w = w + tex->rot.z;
  w = w - ((int) w);
  if (w < 0.0) w += 1.0;

  maxscale = (fabs(tex->scale.x) > fabs(tex->scale.y)) ? 
             tex->scale.x : tex->scale.y;
  if (fabs(tex->scale.z) > fabs(maxscale))
    maxscale = tex->scale.z;

  miprad = 0.05 * ry->opticdist * fabs(maxscale);

  return VolMIPMap(tex->img, u, v, w, miprad);
}
