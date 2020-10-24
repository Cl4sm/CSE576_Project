int rt_threadpool_destroy(rt_threadpool_t *thrpool) {
#if defined(THR)
  int i;
#endif

  /* wake threads and tell them to shutdown */
  rt_thread_run_barrier(&thrpool->runbar, NULL, NULL, NULL);

#if defined(THR)
  /* join the pool of worker threads */
  for (i=0; i<thrpool->workercount; i++) {
    rt_thread_join(thrpool->threads[i], NULL);
  }
#endif

  /* destroy the thread barrier */
  rt_thread_run_barrier_destroy(&thrpool->runbar);

  /* destroy the shared iterator */
  rt_shared_iterator_destroy(&thrpool->iter);

  /* destroy tile stack for error handling */
  rt_tilestack_destroy(&thrpool->errorstack);

  free(thrpool->devlist);
  free(thrpool->threads);
  free(thrpool->workerdata);
  free(thrpool);

  return 0;
}
