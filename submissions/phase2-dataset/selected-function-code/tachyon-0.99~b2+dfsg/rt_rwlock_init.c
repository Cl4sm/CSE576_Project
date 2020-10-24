int rt_rwlock_init(rt_rwlock_t * rwp) {
  int status=0;

#ifdef THR  
#ifdef _MSC_VER
  rt_mutex_init(&rwp->lock);
  rt_cond_init(&rwp->rdrs_ok);
  rt_cond_init(&rwp->wrtr_ok);
  rwp->rwlock = 0;
  rwp->waiting_writers = 0;
#endif

#ifdef USEPOSIXTHREADS
  pthread_mutex_init(&rwp->lock, NULL);
  pthread_cond_init(&rwp->rdrs_ok, NULL);
  pthread_cond_init(&rwp->wrtr_ok, NULL);
  rwp->rwlock = 0;
  rwp->waiting_writers = 0;
#endif /* USEPOSIXTHREADS */

#ifdef USEUITHREADS
  status = rwlock_init(rwp, USYNC_THREAD, NULL);
#endif /* USEUITHREADS */
#endif /* THR */

  return status;
}
