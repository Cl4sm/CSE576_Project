  grid * g;
  int numobj, numcbrt;
  vector gmin={0,0,0};
  vector gmax={0,0,0};
  gridindex index;
  char msgtxt[128];
  int numsucceeded; 
  if (scene->objgroup.boundedobj == NULL)
    return 0;

  numobj = countobj(scene->objgroup.boundedobj);

  if (scene->mynode == 0) {
    sprintf(msgtxt, "Scene contains %d objects.", numobj);
    rt_ui_message(MSG_0, msgtxt);
  }

  if (numobj > boundthresh) {
    numcbrt = (int) cbrt(4*numobj);
    
    globalbound(&scene->objgroup.boundedobj, &gmin, &gmax);
    if (scene->verbosemode && scene->mynode == 0) {
      char t[256]; /* msgtxt */
      sprintf(t, "Global bounds: %g %g %g -> %g %g %g", 
              gmin.x, gmin.y, gmin.z, gmax.x, gmax.y, gmax.z);  
      rt_ui_message(MSG_0, t);

      sprintf(t, "Creating top level grid: X:%d Y:%d Z:%d", 
              numcbrt, numcbrt, numcbrt);
      rt_ui_message(MSG_0, t);
    }

    g = (grid *) newgrid(scene, numcbrt, numcbrt, numcbrt, gmin, gmax);
    numsucceeded = engrid_objlist(g, &scene->objgroup.boundedobj);
    if (scene->verbosemode && scene->mynode == 0)
      gridstats(numcbrt, numcbrt, numcbrt, numsucceeded); 

    if (scene->verbosemode && scene->mynode == 0) {
      char t[256]; /* msgtxt */
      numobj = countobj(scene->objgroup.boundedobj);
      sprintf(t, "Scene contains %d non-gridded objects\n", numobj);
      rt_ui_message(MSG_0, t);
    } 

    /* add this grid to the bounded object list removing the objects */
    /* now contained and managed by the grid                         */
    g->nextobj = scene->objgroup.boundedobj;
    scene->objgroup.boundedobj = (object *) g;

    /* create subgrids for overfull cell in the top level grid...    */
    for (index.z=0; index.z<g->zsize; index.z++) {
      for (index.y=0; index.y<g->ysize; index.y++) {
        for (index.x=0; index.x<g->xsize; index.x++) {
          engrid_cell(scene, boundthresh, g, &index);
        }
      }
    } 
  }

  return 1;
}
