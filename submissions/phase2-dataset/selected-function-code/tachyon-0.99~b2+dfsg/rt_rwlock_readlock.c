  int status=0;

#ifdef THR  
#ifdef _MSC_VER
  rt_mutex_lock(&rwp->lock);
  while (rwp->rwlock < 0 || rwp->waiting_writers) 
    rt_cond_wait(&rwp->rdrs_ok, &rwp->lock);   
  rwp->rwlock++;   /* increment number of readers holding the lock */
  rt_mutex_unlock(&rwp->lock);
#endif

#ifdef USEPOSIXTHREADS
  pthread_mutex_lock(&rwp->lock);
  while (rwp->rwlock < 0 || rwp->waiting_writers) 
    pthread_cond_wait(&rwp->rdrs_ok, &rwp->lock);   
  rwp->rwlock++;   /* increment number of readers holding the lock */
  pthread_mutex_unlock(&rwp->lock);
#endif /* USEPOSIXTHREADS */

#ifdef USEUITHREADS
  status = rw_rdlock(rwp);
#endif /* USEUITHREADS */
#endif /* THR */

  return status;
}
