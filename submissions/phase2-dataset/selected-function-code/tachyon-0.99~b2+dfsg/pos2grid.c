static int pos2grid(grid * g, vector * pos, gridindex * index) {
  index->x = (int) ((flt) (pos->x - g->min.x) / g->voxsize.x);
  index->y = (int) ((flt) (pos->y - g->min.y) / g->voxsize.y);
  index->z = (int) ((flt) (pos->z - g->min.z) / g->voxsize.z);
  
  if (index->x == g->xsize)
    index->x--;
  if (index->y == g->ysize)
    index->y--;
  if (index->z == g->zsize)
    index->z--;

  if (index->x < 0 || index->x > g->xsize ||
      index->y < 0 || index->y > g->ysize ||
      index->z < 0 || index->z > g->zsize) 
    return 0;

  if (pos->x < g->min.x || pos->x > g->max.x ||
      pos->y < g->min.y || pos->y > g->max.y ||
      pos->z < g->min.z || pos->z > g->max.z) 
    return 0; 

  return 1;
}
