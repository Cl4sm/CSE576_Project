void renderscene(scenedef * scene) {
  flt runtime;
  rt_timerhandle rtth; /* render time timer handle */

  /* if certain key aspects of the scene parameters have been changed */
  /* since the last frame rendered, or when rendering the scene the   */
  /* first time, various setup, initialization and memory allocation  */
  /* routines need to be run in order to prepare for rendering.       */
  if (scene->scenecheck)
    rendercheck(scene);

  if (scene->mynode == 0) 
    rt_ui_progress(0);     /* print 0% progress at start of rendering */


  /* 
   * Core Ray Tracing Code
   *
   * Ideally, as little as possible other than this code should be
   * executed for rendering a frame.  Most if not all memory allocations
   * should be done outside of the core code, and all setup should be
   * done outside of here.  This will give the best speed when rendering
   * walk-throughs and similar things.  
   */

  rtth=rt_timer_create();  /* create/init rendering timer              */
  rt_timer_start(rtth);    /* start ray tracing timer                  */

  camera_init(scene);      /* Initialize all aspects of camera system  */

#ifdef THR
  /* if using threads, wake up the child threads...  */
  rt_thread_barrier(((thr_parms *) scene->threadparms)[0].runbar, 1);
#endif

#ifdef MPI
  /* if using message passing, start persistent receives */
  rt_start_scanlinereceives(scene->parbuf); /* start scanline receives */
#endif

  /* Actually Ray Trace The Image */
  thread_trace(&((thr_parms *) scene->threadparms)[0]);

#ifdef MPI
  rt_waitscanlines(scene->parbuf);  /* wait for all scanlines to recv/send  */
#endif

  rt_timer_stop(rtth);              /* stop timer for ray tracing runtime   */
  runtime=rt_timer_time(rtth);
  rt_timer_destroy(rtth);

  /*
   * End of Core Ray Tracing Code
   *
   * Anything after here should be UI, tear-down, or reset code 
   *
   */

  if (scene->mynode == 0) {
    char msgtxt[256];

    rt_ui_progress(100); /* print 100% progress when finished rendering */

    sprintf(msgtxt, "\n  Ray Tracing Time: %10.4f seconds", runtime);
    rt_ui_message(MSG_0, msgtxt);
 
    if (scene->writeimagefile) 
      renderio(scene);
  }
} /* end of renderscene() */
