                    rt_tasktile_t *tile) {
  rt_shared_iterator_t iter;
  rt_threadlaunch_t *parms=NULL;
  rt_thread_t * threads=NULL;
  int i, rc;

  /* XXX have to ponder what the right thing to do is here */
#if !defined(THR)
  numprocs=1;
#endif

  /* initialize shared iterator and set the iteration and range */
  rt_shared_iterator_init(&iter);
  if (rt_shared_iterator_set(&iter, tile))
    return -1;

  /* allocate array of threads */
  threads = (rt_thread_t *) calloc(numprocs * sizeof(rt_thread_t), 1);
  if (threads == NULL)
    return -1;

  /* allocate and initialize array of thread parameters */
  parms = (rt_threadlaunch_t *) malloc(numprocs * sizeof(rt_threadlaunch_t));
  if (parms == NULL) {
    free(threads);
    return -1;
  }
  for (i=0; i<numprocs; i++) {
    parms[i].iter = &iter;
    parms[i].threadid = i;
    parms[i].threadcount = numprocs;
    parms[i].clientdata = clientdata;
  }

#if defined(THR)
  if (numprocs == 1) {
    /* XXX we special-case the single worker thread  */
    /*     scenario because this greatly reduces the */
    /*     GPU kernel launch overhead since a new    */
    /*     contexts doesn't have to be created, and  */
    /*     in the simplest case with a single-GPU we */
    /*     will just be using the same device anyway */
    /*     Ideally we shouldn't need to do this....  */
    /* single thread does all of the work */
    fctn((void *) &parms[0]);
  } else {
    /* spawn child threads to do the work */
    for (i=0; i<numprocs; i++) {
      rt_thread_create(&threads[i], fctn, &parms[i]);
    }

    /* join the threads after work is done */
    for (i=0; i<numprocs; i++) {
      rt_thread_join(threads[i], NULL);
    }
  }
#else
  /* single thread does all of the work */
  fctn((void *) &parms[0]);
#endif

  /* free threads/parms */
  free(parms);
  free(threads);

  /* query error status for return to caller */
  rc=rt_shared_iterator_getfatalerror(&iter);

  /* destroy the shared iterator */
  rt_shared_iterator_destroy(&iter);

  return rc;
}
