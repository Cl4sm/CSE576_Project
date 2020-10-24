  int i, numvoxels;
  grid * g = (grid *) v;

  /* loop through all voxels and free the object lists */
  numvoxels = g->xsize * g->ysize * g->zsize; 
  for (i=0; i<numvoxels; i++) {
    objectlist * lcur;
    objectlist * lnext;

    lcur = g->cells[i];
    while (lcur != NULL) {
      lnext = lcur->next;
      free(lcur);
      lcur = lnext;
    }
  }

  /* free the grid cells */ 
  free(g->cells);

  /* free all objects on the grid object list */
  free_objects(g->objects);   

  free(g);
}
