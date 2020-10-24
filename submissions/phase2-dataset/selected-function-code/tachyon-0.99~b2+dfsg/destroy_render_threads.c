void destroy_render_threads(scenedef * scene) {
  thr_parms * parms = (thr_parms *) scene->threadparms;
  rt_thread_t * threads = (rt_thread_t *) scene->threads;
  int thr;

  if (scene->threads != NULL) {
    /* wake up sleepers and tell them to exit */
    rt_thread_barrier(parms[0].runbar, 0); 

    /* wait for all sleepers to exit */
    for (thr=1; thr<parms[0].nthr; thr++) 
      rt_thread_join(threads[thr], NULL);
  
    /* destroy the thread barrier */
    rt_thread_barrier_destroy(parms[0].runbar);

    free(scene->threads);
  }

  if (scene->threadparms != NULL) {
    /* deallocate thread parameter buffers 
     * NOTE: This has to use the remembered number of threads stored in the
     *       thread parameter area for thread 0, since the one in the scene
     *       may have changed on us.
     */
    for (thr=0; thr < parms[0].nthr; thr++) {
      if (parms[thr].local_mbox != NULL) 
        free(parms[thr].local_mbox);
    }

    free(scene->threadparms);
  }

  scene->threads = NULL;
  scene->threadparms = NULL;
}
