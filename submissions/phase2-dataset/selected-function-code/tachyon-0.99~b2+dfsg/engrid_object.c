  vector omin, omax; 
  gridindex low, high;
  int x, y, z, zindex, yindex, voxindex;
  objectlist * tmp;
 
  if (obj->methods->bbox(obj, &omin, &omax)) { 
    if (!pos2grid(g, &omin, &low) || !pos2grid(g, &omax, &high)) {
      return 0; /* object is not wholly contained in the grid, don't engrid */
    }
  } else {
    return 0;   /* object is unbounded, don't engrid this object */
  }

#if 0
  /* test grid cell occupancy size to see if this object would   */
  /* consume a huge number of grid cells (thus causing problems) */
  /* in the special case of a top level grid, we could count the */
  /* number of problematic objects, and if they exceed a maximum */
  /* percentage or absolute number of these, we should cancel    */
  /* filling the top level grid and rebuild a coarser top level  */
  /* grid instead, to prevent an explosion of memory use.        */
  {
  int voxeloccupancy = (high.x - low.x) * (high.y - low.y) * (high.z - low.z);
  if (voxeloccupancy > 22000) {
    return 0; /* don't engrid this object */
  }
  }
#endif

  /* add the object to the complete list of objects in the grid */
  if (addtolist) { 
    obj->nextobj = g->objects;
    g->objects = obj;
  }

  /* add this object to all voxels it inhabits */
  for (z=low.z; z<=high.z; z++) {
    zindex = z * g->xsize * g->ysize;
    for (y=low.y; y<=high.y; y++) {
      yindex = y * g->xsize;
      for (x=low.x; x<=high.x; x++) {
        voxindex = x + yindex + zindex; 
        tmp = (objectlist *) malloc(sizeof(objectlist));
        tmp->next = g->cells[voxindex];
        tmp->obj = obj;
        g->cells[voxindex] = tmp;
      }
    }
  }
 
  return 1;
}
