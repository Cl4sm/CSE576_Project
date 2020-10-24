  flt radius, angle;
  int grain;
  color col;
  flt x,y,z;
  standard_texture * tex = (standard_texture *) tx;

  x=(hit->x - tex->ctr.x) / tex->scale.x;
  y=(hit->y - tex->ctr.y) / tex->scale.y;
  z=(hit->z - tex->ctr.z) / tex->scale.z;

  radius=sqrt(x*x + z*z);
  if (z == 0.0) 
    angle=3.1415926/2.0;
  else 
    angle=atan(x / z);

  radius=radius + 3.0 * sin(20 * angle + y / 150.0);
  grain=((int) (radius + 0.5)) % 60;
  if (grain < 40) {
    col.r=0.8;
    col.g=1.0;
    col.b=0.2;
  }
  else {
    col.r=0.0;
    col.g=0.0;
    col.b=0.0;
  }     

  return col;
} 
