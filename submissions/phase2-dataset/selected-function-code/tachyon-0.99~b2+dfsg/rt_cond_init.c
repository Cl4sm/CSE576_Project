int rt_cond_init(rt_cond_t * cvp) {
  int status=0;

#ifdef THR
#ifdef _MSC_VER
#if defined(RTUSEWIN2008CONDVARS)
  InitializeConditionVariable(cvp);
#else
  /* XXX not implemented */
  cvp->waiters = 0;

  /* Create an auto-reset event. */
  cvp->events[RT_COND_SIGNAL] = CreateEvent(NULL,  /* no security */
                                            FALSE, /* auto-reset event */
                                            FALSE, /* non-signaled initially */
                                            NULL); /* unnamed */

  /* Create a manual-reset event. */
  cvp->events[RT_COND_BROADCAST] = CreateEvent(NULL,  /* no security */
                                               TRUE,  /* manual-reset */
                                               FALSE, /* non-signaled initially */
                                               NULL); /* unnamed */
#endif
#endif /* _MSC_VER */

#ifdef USEPOSIXTHREADS
  status = pthread_cond_init(cvp, NULL);
#endif /* USEPOSIXTHREADS */
#ifdef USEUITHREADS
  status = cond_init(cvp, USYNC_THREAD, NULL);
#endif
#endif /* THR */

  return status;
}
