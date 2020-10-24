  scenedef * scene = (scenedef *) voidscene;
  list * cur, * next;

  if (scene != NULL) {
    if (scene->imginternal) {
      free(scene->img);
    }

    /* tear down and deallocate persistent rendering threads */
    destroy_render_threads(scene);

    /* tear down and deallocate persistent scanline receives */
    if (scene->parbuf != NULL)
      rt_delete_scanlinereceives(scene->parbuf);

    /* free all lights */
    cur = scene->lightlist;
    while (cur != NULL) {
      next = cur->next;

      /* free lights that have special data, or aren't freed */
      /* as part of the object list deallocation loop.       */
      free_light_special(cur->item);
      free(cur); 
      cur = next;
    }    

    /* free all textures */
    cur = scene->texlist;
    while (cur != NULL) {
      next = cur->next;
      ((texture *) cur->item)->methods->freetex(cur->item); /* free texture */
      free(cur); /* free list entry */
      cur = next;
    }

    /* free all clipping planes */
    cur = scene->cliplist;
    while (cur != NULL) {
      next = cur->next;
      free(((clip_group *) cur->item)->planes); /* free array of clip planes */
      free(cur->item);                          /* free clip group struct    */
      free(cur);                                /* free list entry           */
      cur = next;
    }    

    /* free all other textures, MIP Maps, and images */
    FreeTextures();
    
    free(scene->cpuinfo);
    free_objects(scene->objgroup.boundedobj);
    free_objects(scene->objgroup.unboundedobj);

    free(scene);
  }
}
