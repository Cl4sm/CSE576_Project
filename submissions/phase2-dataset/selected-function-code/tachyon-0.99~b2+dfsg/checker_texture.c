  long x,y,z;
  flt xh,yh,zh;
  color col;
  standard_texture * tex = (standard_texture *) tx;

  xh=hit->x - tex->ctr.x; 
  x=(long) ((fabs(xh) * 3) + 0.5);
  x=x % 2;
  yh=hit->y - tex->ctr.y;
  y=(long) ((fabs(yh) * 3) + 0.5);
  y=y % 2;
  zh=hit->z - tex->ctr.z;
  z=(long) ((fabs(zh) * 3) + 0.5);
  z=z % 2;

  if (((x + y + z) % 2)==1) {
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
