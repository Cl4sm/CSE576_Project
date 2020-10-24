  flt tnear, tfar;
  vector curpos, tmax, tdelta;
  gridindex curvox, step, out; 
  int voxindex, SY, SZ;
  unsigned long serial;
#if !defined(DISABLEMBOX)
  unsigned long * mbox;
#endif
  objectlist * cur;

  if (ry->flags & RT_RAY_FINISHED)
    return;

  if (!grid_bounds_intersect(g, ry, &tnear, &tfar))
    return;
 
  if (ry->maxdist < tnear)
    return;
  
  serial=ry->serial;
#if !defined(DISABLEMBOX)
  mbox=ry->mbox;
#endif

  /* find the entry point in the grid from the near hit */ 
  curpos.x = ry->o.x + (ry->d.x * tnear);
  curpos.y = ry->o.y + (ry->d.y * tnear);
  curpos.z = ry->o.z + (ry->d.z * tnear);

  /* map the entry point to its nearest voxel */
  curvox.x = (int) ((flt) (curpos.x - g->min.x) / g->voxsize.x);
  curvox.y = (int) ((flt) (curpos.y - g->min.y) / g->voxsize.y);
  curvox.z = (int) ((flt) (curpos.z - g->min.z) / g->voxsize.z);
  if (curvox.x == g->xsize) curvox.x--;
  if (curvox.y == g->ysize) curvox.y--;
  if (curvox.z == g->zsize) curvox.z--;

  /* Setup X iterator stuff */
  if (ry->d.x < -EPSILON) {
    tmax.x = tnear + ((voxel2x(g, curvox.x) - curpos.x) / ry->d.x); 
    tdelta.x = g->voxsize.x / - ry->d.x;
    step.x = -1;
    out.x = -1;
  } else if (ry->d.x > EPSILON) {
    tmax.x = tnear + ((voxel2x(g, curvox.x + 1) - curpos.x) / ry->d.x);
    tdelta.x = g->voxsize.x / ry->d.x;
    step.x = 1;
    out.x = g->xsize;
  } else {
    tmax.x = FHUGE;
    tdelta.x = 0.0;
    step.x = 0;
    out.x = 0; /* never goes out of bounds on this axis */
  }

  /* Setup Y iterator stuff */
  if (ry->d.y < -EPSILON) {
    tmax.y = tnear + ((voxel2y(g, curvox.y) - curpos.y) / ry->d.y);
    tdelta.y = g->voxsize.y / - ry->d.y;
    step.y = -1;
    out.y = -1;
  } else if (ry->d.y > EPSILON) {
    tmax.y = tnear + ((voxel2y(g, curvox.y + 1) - curpos.y) / ry->d.y);
    tdelta.y = g->voxsize.y / ry->d.y;
    step.y = 1;
    out.y = g->ysize;
  } else {
    tmax.y = FHUGE;
    tdelta.y = 0.0; 
    step.y = 0;
    out.y = 0; /* never goes out of bounds on this axis */
  }

  /* Setup Z iterator stuff */
  if (ry->d.z < -EPSILON) {
    tmax.z = tnear + ((voxel2z(g, curvox.z) - curpos.z) / ry->d.z);
    tdelta.z = g->voxsize.z / - ry->d.z;
    step.z = -1;
    out.z = -1;
  } else if (ry->d.z > EPSILON) {
    tmax.z = tnear + ((voxel2z(g, curvox.z + 1) - curpos.z) / ry->d.z);
    tdelta.z = g->voxsize.z / ry->d.z;
    step.z = 1;
    out.z = g->zsize;
  } else {
    tmax.z = FHUGE;
    tdelta.z = 0.0; 
    step.z = 0;
    out.z = 0; /* never goes out of bounds on this axis */
  }

  /* pre-calculate row/column/plane offsets for stepping through grid */
  SY = step.y * g->xsize;
  SZ = step.z * g->xsize * g->ysize;

  /* first cell we'll be testing */
  voxindex = curvox.z*g->xsize*g->ysize + curvox.y*g->xsize + curvox.x; 

  /* Unrolled while loop by one... */
  /* Test all objects in the current cell for intersection */
  cur = g->cells[voxindex];
  while (cur != NULL) {
#if !defined(DISABLEMBOX)
    if (mbox[cur->obj->id] != serial) {
      mbox[cur->obj->id] = serial; 
      cur->obj->methods->intersect(cur->obj, ry);
    }
#else
    cur->obj->methods->intersect(cur->obj, ry);
#endif
    cur = cur->next;
  }

  /* Loop through grid cells until we're done */
  while (!(ry->flags & RT_RAY_FINISHED)) {
    /* Walk to next cell */
    if (tmax.x < tmax.y && tmax.x < tmax.z) {
      curvox.x += step.x;
      if (ry->maxdist < tmax.x || curvox.x == out.x) 
        break; 
      tmax.x += tdelta.x;
      voxindex += step.x;
    }
    else if (tmax.z < tmax.y) {
      curvox.z += step.z;
      if (ry->maxdist < tmax.z || curvox.z == out.z) 
        break;
      tmax.z += tdelta.z;
      voxindex += SZ;
    }
    else {
      curvox.y += step.y;
      if (ry->maxdist < tmax.y || curvox.y == out.y) 
        break;
      tmax.y += tdelta.y;
      voxindex += SY;
    }

    /* Test all objects in the current cell for intersection */
    cur = g->cells[voxindex];
    while (cur != NULL) {
#if !defined(DISABLEMBOX)
      if (mbox[cur->obj->id] != serial) {
        mbox[cur->obj->id] = serial; 
        cur->obj->methods->intersect(cur->obj, ry);
      }
#else
      cur->obj->methods->intersect(cur->obj, ry);
#endif
      cur = cur->next;
    }
  }
}
