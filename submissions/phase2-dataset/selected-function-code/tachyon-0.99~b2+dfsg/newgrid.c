object * newgrid(scenedef * scene, int xsize, int ysize, int zsize, vector min, vector max) {
  grid * g;
  int numcells;

  g = (grid *) malloc(sizeof(grid));
  memset(g, 0, sizeof(grid));  

  g->methods = &grid_methods;
  g->id = new_objectid(scene);

  g->xsize = xsize;
  g->ysize = ysize;
  g->zsize = zsize;

  numcells = xsize * ysize * zsize;

  g->min = min;
  g->max = max;

  VSub(&g->max, &g->min, &g->voxsize);
  g->voxsize.x /= (flt) g->xsize; 
  g->voxsize.y /= (flt) g->ysize; 
  g->voxsize.z /= (flt) g->zsize; 

  g->cells = (objectlist **) malloc(numcells * sizeof(objectlist *));
  memset(g->cells, 0, numcells * sizeof(objectlist *));

  return (object *) g;
}
