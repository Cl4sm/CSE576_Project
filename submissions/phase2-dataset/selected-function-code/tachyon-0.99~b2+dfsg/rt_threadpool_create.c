  int i;
  rt_threadpool_t *thrpool = NULL;
  thrpool = (rt_threadpool_t *) malloc(sizeof(rt_threadpool_t));
  if (thrpool == NULL)
    return NULL;

  memset(thrpool, 0, sizeof(rt_threadpool_t));

#if !defined(THR)
  workercount=1;
#endif

  /* if caller provides a device list, use it, otherwise we assume */
  /* all workers are CPU cores */
  thrpool->devlist = (int *) malloc(sizeof(int) * workercount);
  if (devlist == NULL) {
    for (i=0; i<workercount; i++)
      thrpool->devlist[i] = -1; /* mark as a CPU core */
  } else {
    memcpy(thrpool->devlist, devlist, sizeof(int) * workercount);
  }

  /* initialize shared iterator */
  rt_shared_iterator_init(&thrpool->iter);

  /* initialize tile stack for error handling */
  rt_tilestack_init(&thrpool->errorstack, 64);

  /* create a run barrier with N+1 threads: N workers, 1 master */
  thrpool->workercount = workercount;
  rt_thread_run_barrier_init(&thrpool->runbar, workercount+1);

  /* allocate and initialize thread pool */
  thrpool->threads = (rt_thread_t *) malloc(sizeof(rt_thread_t) * workercount);
  thrpool->workerdata = (rt_threadpool_workerdata_t *) malloc(sizeof(rt_threadpool_workerdata_t) * workercount);
  memset(thrpool->workerdata, 0, sizeof(rt_threadpool_workerdata_t) * workercount);

  /* setup per-worker data */
  for (i=0; i<workercount; i++) {
    thrpool->workerdata[i].iter=&thrpool->iter;
    thrpool->workerdata[i].errorstack=&thrpool->errorstack;
    thrpool->workerdata[i].threadid=i;
    thrpool->workerdata[i].threadcount=workercount;
    thrpool->workerdata[i].devid=thrpool->devlist[i];
    thrpool->workerdata[i].devspeed=1.0f; /* must be reset by dev setup code */
    thrpool->workerdata[i].thrpool=thrpool;
  }

#if defined(THR)
  /* launch thread pool */
  for (i=0; i<workercount; i++) {
    rt_thread_create(&thrpool->threads[i], rt_threadpool_workerproc, &thrpool->workerdata[i]);
  }
#endif

  return thrpool;
}
