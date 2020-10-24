int rt_rwlock_writelock(rt_rwlock_t * rwp) {
  int status=0;

#ifdef THR  
#ifdef _MSC_VER
  rt_mutex_lock(&rwp->lock);
  while (rwp->rwlock != 0) {
    rwp->waiting_writers++;
    rt_cond_wait(&rwp->wrtr_ok, &rwp->lock);
    rwp->waiting_writers--;
  }
  rwp->rwlock=-1;
  rt_mutex_unlock(&rwp->lock);
#endif

#ifdef USEPOSIXTHREADS
  pthread_mutex_lock(&rwp->lock);
  while (rwp->rwlock != 0) {
    rwp->waiting_writers++;
    pthread_cond_wait(&rwp->wrtr_ok, &rwp->lock);
    rwp->waiting_writers--;
  }
  rwp->rwlock=-1;
  pthread_mutex_unlock(&rwp->lock);
#endif /* USEPOSIXTHREADS */

#ifdef USEUITHREADS
  status = rw_wrlock(rwp);
#endif /* USEUITHREADS */
#endif /* THR */

  return status;
}
