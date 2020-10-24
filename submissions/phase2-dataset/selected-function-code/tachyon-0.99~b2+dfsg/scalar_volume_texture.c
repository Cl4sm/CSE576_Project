  color col, col2;
  box * bx;
  flt a, tx1, tx2, ty1, ty2, tz1, tz2;
  flt tnear, tfar;
  flt t, tdist, dt, sum, tt; 
  vector pnt, bln;
  scalarvol * vol;
  flt scalar, transval; 
  int x, y, z;
  unsigned char * ptr;
  standard_texture * tex = (standard_texture *) tx;

  bx=(box *) tex->obj;
  vol=(scalarvol *) ((standard_texture *) bx->tex)->img;
   
  col.r=0.0;
  col.g=0.0;
  col.b=0.0;
 
  tnear= -FHUGE;
  tfar= FHUGE;
 
  if (ry->d.x == 0.0) {
    if ((ry->o.x < bx->min.x) || (ry->o.x > bx->max.x)) return col;
  }
  else {
    tx1 = (bx->min.x - ry->o.x) / ry->d.x;
    tx2 = (bx->max.x - ry->o.x) / ry->d.x;
    if (tx1 > tx2) { a=tx1; tx1=tx2; tx2=a; }
    if (tx1 > tnear) tnear=tx1;
    if (tx2 < tfar)   tfar=tx2;
  }
  if (tnear > tfar) return col;
  if (tfar < 0.0) return col;
 
 if (ry->d.y == 0.0) {
    if ((ry->o.y < bx->min.y) || (ry->o.y > bx->max.y)) return col;
  }
  else {
    ty1 = (bx->min.y - ry->o.y) / ry->d.y;
    ty2 = (bx->max.y - ry->o.y) / ry->d.y;
    if (ty1 > ty2) { a=ty1; ty1=ty2; ty2=a; }
    if (ty1 > tnear) tnear=ty1;
    if (ty2 < tfar)   tfar=ty2;
  }
  if (tnear > tfar) return col;
  if (tfar < 0.0) return col;
 
  if (ry->d.z == 0.0) {
    if ((ry->o.z < bx->min.z) || (ry->o.z > bx->max.z)) return col;
  }
  else {
    tz1 = (bx->min.z - ry->o.z) / ry->d.z;
    tz2 = (bx->max.z - ry->o.z) / ry->d.z;
    if (tz1 > tz2) { a=tz1; tz1=tz2; tz2=a; }
    if (tz1 > tnear) tnear=tz1;
    if (tz2 < tfar)   tfar=tz2;
  }

#if 0
  /* XXX this is where we cause early exit if the volumetric */
  /*     object intersects other geometric objects           */

  /* stop at closest intersection from other objects */
  if (ry->maxdist < tfar) 
    tfar = ry->maxdist;
#endif

  if (tnear > tfar) return col;
  if (tfar < 0.0) return col;
 
  if (tnear < 0.0) tnear=0.0;
 
  tdist=sqrt(vol->xres*vol->xres + vol->yres*vol->yres + vol->zres*vol->zres);
  tt = (vol->opacity / tdist); 

  bln.x=fabs(bx->min.x - bx->max.x);
  bln.y=fabs(bx->min.y - bx->max.y);
  bln.z=fabs(bx->min.z - bx->max.z);
  
  dt=sqrt(bln.x*bln.x + bln.y*bln.y + bln.z*bln.z) / tdist; 
  sum=0.0;

  for (t=tnear; t<=tfar; t+=dt) {
    pnt.x=((ry->o.x + (ry->d.x * t)) - bx->min.x) / bln.x;
    pnt.y=((ry->o.y + (ry->d.y * t)) - bx->min.y) / bln.y;
    pnt.z=((ry->o.z + (ry->d.z * t)) - bx->min.z) / bln.z;
 
    x=(int) ((vol->xres - 1.5) * pnt.x + 0.5);
    y=(int) ((vol->yres - 1.5) * pnt.y + 0.5);
    z=(int) ((vol->zres - 1.5) * pnt.z + 0.5);
   
    ptr = vol->data + ((vol->xres * vol->yres * z) + (vol->xres * y) + x);
   
    scalar = (flt) ((flt) 1.0 * ((int) ptr[0])) / 255.0;

    sum += tt * scalar; 

    transval = tt * scalar; 

    col2 = VoxelColor(scalar);

    if (sum < 1.0) {
      col.r += transval * col2.r;
      col.g += transval * col2.g;
      col.b += transval * col2.b;
      if (sum < 0.0) sum=0.0;
    }  
    else { 
      sum=1.0;
    }
  }


  /* XXX this has to be changed in order to allow volumetric objects */
  /*     to intersect with geometric objects                         */

  if (sum < 1.0) {      /* spawn transmission rays / refraction */    
    color transcol;
    shadedata shadevars;
    shadevars.hit=*hit;
    transcol = shade_transmission(ry, &shadevars, 1.0 - sum);

    col.r += transcol.r; /* add the transmitted ray  */    
    col.g += transcol.g; /* to the diffuse and       */
    col.b += transcol.b; /* transmission total..     */  
  }

  return col;
}
