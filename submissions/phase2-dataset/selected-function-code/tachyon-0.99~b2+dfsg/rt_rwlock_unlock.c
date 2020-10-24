  int status=0;

#ifdef THR  
#ifdef _MSC_VER
  int ww, wr;
  rt_mutex_lock(&rwp->lock);
  if (rwp->rwlock > 0) {
    rwp->rwlock--;
  } else {
    rwp->rwlock = 0;
  } 
  ww = (rwp->waiting_writers && rwp->rwlock == 0);
  wr = (rwp->waiting_writers == 0);
  rt_mutex_unlock(&rwp->lock);
  if (ww) 
    rt_cond_signal(&rwp->wrtr_ok);
  else if (wr)
    rt_cond_signal(&rwp->rdrs_ok);
#endif

#ifdef USEPOSIXTHREADS
  int ww, wr;
  pthread_mutex_lock(&rwp->lock);
  if (rwp->rwlock > 0) {
    rwp->rwlock--;
  } else {
    rwp->rwlock = 0;
  } 
  ww = (rwp->waiting_writers && rwp->rwlock == 0);
  wr = (rwp->waiting_writers == 0);
  pthread_mutex_unlock(&rwp->lock);
  if (ww) 
    pthread_cond_signal(&rwp->wrtr_ok);
  else if (wr)
    pthread_cond_signal(&rwp->rdrs_ok);
#endif /* USEPOSIXTHREADS */

#ifdef USEUITHREADS
  status = rw_unlock(rwp);
#endif /* USEUITHREADS */
#endif /* THR */

  return status;
}
