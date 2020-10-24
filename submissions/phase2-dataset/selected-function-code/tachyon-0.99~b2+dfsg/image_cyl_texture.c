color image_cyl_texture(const vector * hit, const texture * tx, const ray * ry) {
  vector rh;
  flt u, v, miprad, maxscale, cyrad;
  standard_texture * tex = (standard_texture *) tx;
 
  rh.x=hit->x - tex->ctr.x;
  rh.z=hit->y - tex->ctr.y;
  rh.y=hit->z - tex->ctr.z;
 
  xyztocyl(rh, 1.0, &u, &v);

  u = u * tex->scale.x;  
  u = u + tex->rot.x;
  u = u - ((int) u);
  if (u < 0.0) u+=1.0; 

  v = v * tex->scale.y; 
  v = v + tex->rot.y;
  v = v - ((int) v);
  if (v < 0.0) v+=1.0; 

  cyrad = EPSILON + 8.0 * sqrt(rh.x*rh.x + rh.y*rh.y + rh.z*rh.z);

  maxscale = (fabs(tex->scale.x) > fabs(tex->scale.y)) ? 
             tex->scale.x : tex->scale.y;

  miprad = (0.05 * ry->opticdist * fabs(maxscale)) / cyrad;

  return MIPMap(tex->img, u, v, miprad);
}  
