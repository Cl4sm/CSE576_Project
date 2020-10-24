  flt runtime;
  rt_timerhandle stth; /* setup time timer handle */

  if (scene->verbosemode && scene->mynode == 0) {
    char msgtxt[1024];
    int i, totalcpus;
    flt totalspeed;

    rt_ui_message(MSG_0, "CPU Information:");

    totalspeed = 0.0;
    totalcpus = 0;
    for (i=0; i<scene->nodes; i++) {
      sprintf(msgtxt,
            "  Node %4d: %2d CPUs, CPU Speed %4.2f, Node Speed %6.2f Name: %s",
            i, scene->cpuinfo[i].numcpus, scene->cpuinfo[i].cpuspeed,
            scene->cpuinfo[i].nodespeed, scene->cpuinfo[i].machname);
      rt_ui_message(MSG_0, msgtxt);

      totalcpus += scene->cpuinfo[i].numcpus;
      totalspeed += scene->cpuinfo[i].nodespeed;
    }

    sprintf(msgtxt, "  Total CPUs: %d", totalcpus);
    rt_ui_message(MSG_0, msgtxt);
    sprintf(msgtxt, "  Total Speed: %f\n", totalspeed);
    rt_ui_message(MSG_0, msgtxt);
  }

  rt_barrier_sync();     /* synchronize all nodes at this point             */
  stth=rt_timer_create();
  rt_timer_start(stth);  /* Time the preprocessing of the scene database    */
  rt_autoshader(scene);  /* Adapt to the shading features needed at runtime */

  /* Hierarchical grid ray tracing acceleration scheme */
  if (scene->boundmode == RT_BOUNDING_ENABLED) 
    engrid_scene(scene, scene->boundthresh); 

  /* if any clipping groups exist, we have to use appropriate */
  /* intersection testing logic                               */
  if (scene->cliplist != NULL) {
    scene->flags |= RT_SHADE_CLIPPING;
  }

  /* if there was a preexisting image, free it before continuing */
  if (scene->imginternal && (scene->img != NULL)) {
    free(scene->img);
    scene->img = NULL;
  }

  /* Allocate a new image buffer if necessary */
  if (scene->img == NULL) {
    scene->imginternal = 1;
    if (scene->verbosemode && scene->mynode == 0) { 
      rt_ui_message(MSG_0, "Allocating Image Buffer."); 
    }

    /* allocate the image buffer accordinate to pixel format */
    if (scene->imgbufformat == RT_IMAGE_BUFFER_RGB24) {
      scene->img = malloc(scene->hres * scene->vres * 3);
    } else if (scene->imgbufformat == RT_IMAGE_BUFFER_RGB96F) {
      scene->img = malloc(sizeof(float) * scene->hres * scene->vres * 3);
    } else {
      rt_ui_message(MSG_0, "Illegal image buffer format specifier!"); 
    }

    if (scene->img == NULL) {
      scene->imginternal = 0;
      rt_ui_message(MSG_0, "Warning: Failed To Allocate Image Buffer!"); 
    } 
  }

  /* if any threads are leftover from a previous scene, and the  */
  /* scene has changed significantly, we have to collect, and    */
  /* respawn the worker threads, since lots of things may have   */
  /* changed which would affect them.                            */
  destroy_render_threads(scene);
  create_render_threads(scene);

  /* allocate and initialize persistent scanline receive buffers */
  /* which are used by the parallel message passing code.        */
  scene->parbuf = rt_init_scanlinereceives(scene);

  /* the scene has been successfully prepared for rendering      */
  /* unless it gets modified in certain ways, we don't need to   */
  /* pre-process it ever again.                                  */
  scene->scenecheck = 0;

  rt_timer_stop(stth); /* Preprocessing is finished, stop timing */
  runtime=rt_timer_time(stth);   
  rt_timer_destroy(stth);

  /* Print out relevent timing info */
  if (scene->mynode == 0) {
    char msgtxt[256];
    sprintf(msgtxt, "Preprocessing Time: %10.4f seconds",runtime);
    rt_ui_message(MSG_0, msgtxt);
  }
}
