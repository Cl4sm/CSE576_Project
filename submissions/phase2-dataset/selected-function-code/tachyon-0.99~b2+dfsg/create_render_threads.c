  thr_parms * parms;
  rt_thread_t * threads;
  rt_barrier_t * bar;
  int thr;

  /* allocate and initialize thread parameter buffers */
  threads = (rt_thread_t *) malloc(scene->numthreads * sizeof(rt_thread_t));
  parms = (thr_parms *) malloc(scene->numthreads * sizeof(thr_parms));

  bar = rt_thread_barrier_init(scene->numthreads);

  for (thr=0; thr<scene->numthreads; thr++) {
    parms[thr].tid=thr;
    parms[thr].nthr=scene->numthreads;
    parms[thr].scene=scene;

    /* the sizes of these arrays are padded to avoid cache aliasing */
    /* and false sharing between threads.                           */
    parms[thr].local_mbox = 
#if !defined(DISABLEMBOX)
      (unsigned long *) calloc(sizeof(unsigned long)*scene->objgroup.numobjects + 32, 1);
#else
      NULL;
#endif

    parms[thr].serialno = 1;
    parms[thr].runbar = bar;

    /* For a threads-only build (or MPI nodes == 1), we distribute  */
    /* work round-robin by scanlines.  For MPI-only builds, we also */
    /* distribute by scanlines.  For mixed MPI+threads builds, we   */
    /* distribute work to nodes by scanline, and to the threads     */
    /* within a node on a pixel-by-pixel basis.                     */
    if (scene->nodes == 1) {
      parms[thr].startx = 1;
      parms[thr].stopx  = scene->hres;
      parms[thr].xinc   = 1;
      parms[thr].starty = thr + 1;
      parms[thr].stopy  = scene->vres;
      parms[thr].yinc   = scene->numthreads;
    } else {
      parms[thr].startx = thr + 1;
      parms[thr].stopx  = scene->hres;
      parms[thr].xinc   = scene->numthreads;
      parms[thr].starty = scene->mynode + 1;
      parms[thr].stopy  = scene->vres;
      parms[thr].yinc   = scene->nodes;
    }
  }

  scene->threadparms = (void *) parms;
  scene->threads = (void *) threads;

  for (thr=1; thr < scene->numthreads; thr++) 
    rt_thread_create(&threads[thr], thread_slave, (void *) (&parms[thr]));

}
