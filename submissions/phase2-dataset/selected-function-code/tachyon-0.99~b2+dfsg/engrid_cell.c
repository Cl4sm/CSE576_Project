static int engrid_cell(scenedef * scene, int boundthresh, grid * gold, gridindex *index) {
  vector gmin, gmax, gsize;
  flt len;
  int numobj, numcbrt, xs, ys, zs;
  grid * g;
  objectlist **list;
  objectlist * newobj;
  int numsucceeded;

  list = &gold->cells[index->z*gold->xsize*gold->ysize + 
                     index->y*gold->xsize  + index->x];

  if (*list == NULL)
    return 0;

  numobj =  cellbound(gold, index, &gmin, &gmax);

  VSub(&gmax, &gmin, &gsize);
  len = 1.0 / (MYMAX( MYMAX(gsize.x, gsize.y), gsize.z ));
  gsize.x *= len;  
  gsize.y *= len;  
  gsize.z *= len;  

  if (numobj > boundthresh) {
    numcbrt = (int) cbrt(2*numobj); 
    
    xs = (int) ((flt) numcbrt * gsize.x);
    if (xs < 1) xs = 1;
    ys = (int) ((flt) numcbrt * gsize.y);
    if (ys < 1) ys = 1;
    zs = (int) ((flt) numcbrt * gsize.z);
    if (zs < 1) zs = 1;

    g = (grid *) newgrid(scene, xs, ys, zs, gmin, gmax);
    numsucceeded = engrid_objectlist(g, list);

    if (scene->verbosemode && scene->mynode == 0)
      gridstats(xs, ys, zs, numsucceeded); 

    newobj = (objectlist *) malloc(sizeof(objectlist));    
    newobj->obj = (object *) g;
    newobj->next = *list;
    *list = newobj;

    g->nextobj = gold->objects;
    gold->objects = (object *) g;
  }

  return 1;
}
