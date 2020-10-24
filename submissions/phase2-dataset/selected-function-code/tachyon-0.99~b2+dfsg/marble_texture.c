color marble_texture(const vector * hit, const texture * tx, const ray * ry) {
  flt i,d;
  flt x,y,z;
  color col;
/*
  standard_texture * tex = (standard_texture *) tx;
*/
 
  x=hit->x;
  y=hit->y; 
  z=hit->z;

  x=x * 1.0;

  d=x + 0.0006 * Noise(x, (y * 1.0), (z * 1.0));
  d=d*(((int) d) % 25);
  i=0.0 + 0.10 * fabs(d - 10.0 - 20.0 * ((int) d * 0.05));
  if (i > 1.0) i=1.0;
  if (i < 0.0) i=0.0;  

/*
  col.r=i * tex->col.r;
  col.g=i * tex->col.g;
  col.b=i * tex->col.b;
*/

  col.r = (1.0 + sin(i * 6.28)) / 2.0;
  col.g = (1.0 + sin(i * 16.28)) / 2.0;
  col.b = (1.0 + cos(i * 30.28)) / 2.0;

  return col;      
}
