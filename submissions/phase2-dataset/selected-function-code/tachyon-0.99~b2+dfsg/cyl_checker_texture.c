color cyl_checker_texture(const vector * hit, const texture * tx, const ray * ry) {
  long x,y;
  vector rh;
  flt u,v;
  color col;
  standard_texture * tex = (standard_texture *) tx;
 
  rh.x=hit->x - tex->ctr.x;
  rh.y=hit->y - tex->ctr.y;
  rh.z=hit->z - tex->ctr.z;

  xyztocyl(rh, 1.0, &u, &v); 

  x=(long) (fabs(u) * 18.0);
  x=x % 2;
  y=(long) (fabs(v) * 10.0);
  y=y % 2;
 
  if (((x + y) % 2)==1) {
    col.r=1.0;
    col.g=0.2;
    col.b=0.0;
  }
  else {
    col.r=0.0;
    col.g=0.2;
    col.b=1.0;
  }
 
  return col;
}
