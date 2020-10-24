  vector min, max, cellmin, cellmax;
  objectlist * cur;
  int numinbounds = 0;

  cur = g->cells[index->z*g->xsize*g->ysize + index->y*g->xsize + index->x]; 

  if (cur == NULL)  /* don't bound non-existant objects */
    return 0;

  cellmin.x = voxel2x(g, index->x); 
  cellmin.y = voxel2y(g, index->y); 
  cellmin.z = voxel2z(g, index->z); 

  cellmax.x = cellmin.x + g->voxsize.x;
  cellmax.y = cellmin.y + g->voxsize.y;
  cellmax.z = cellmin.z + g->voxsize.z;

  cmin->x =  FHUGE;   cmin->y =  FHUGE;   cmin->z =  FHUGE;
  cmax->x = -FHUGE;   cmax->y = -FHUGE;   cmax->z = -FHUGE;

  while (cur != NULL)  {  /* Go! */
    min.x = -FHUGE; min.y = -FHUGE; min.z = -FHUGE;
    max.x =  FHUGE; max.y =  FHUGE; max.z =  FHUGE;

    if (cur->obj->methods->bbox((void *) cur->obj, &min, &max)) {
      if ((min.x >= cellmin.x) && (max.x <= cellmax.x) &&
          (min.y >= cellmin.y) && (max.y <= cellmax.y) &&
          (min.z >= cellmin.z) && (max.z <= cellmax.z)) {
      
        cmin->x = MYMIN( cmin->x , min.x);
        cmin->y = MYMIN( cmin->y , min.y);
        cmin->z = MYMIN( cmin->z , min.z);

        cmax->x = MYMAX( cmax->x , max.x);
        cmax->y = MYMAX( cmax->y , max.y);
        cmax->z = MYMAX( cmax->z , max.z);
      
        numinbounds++;
      }
    }

    cur=cur->next;
  }
 
  /* in case we get a 0.0 sized axis on the cell bounds, we'll */
  /* use the original cell bounds */
  if ((cmax->x - cmin->x) < EPSILON) {
    cmax->x += EPSILON;
    cmin->x -= EPSILON;
  }
  if ((cmax->y - cmin->y) < EPSILON) {
    cmax->y += EPSILON;
    cmin->y -= EPSILON;
  }
  if ((cmax->z - cmin->z) < EPSILON) {
    cmax->z += EPSILON;
    cmin->z -= EPSILON;
  }

  return numinbounds;
}
