  int status=0;

#ifdef THR
#ifdef _MSC_VER
#if defined(RTUSEWIN2008CONDVARS)
  WakeConditionVariable(cvp);
#else
#if !defined(RTUSEINTERLOCKEDATOMICOPS)
  EnterCriticalSection(&cvp->waiters_lock);
  int have_waiters = (cvp->waiters > 0);
  LeaveCriticalSection(&cvp->waiters_lock);
  if (have_waiters)
    SetEvent (cvp->events[RT_COND_SIGNAL]);
#else
  if (InterlockedExchangeAdd(&cvp->waiters, 0) > 0)
    SetEvent(cvp->events[RT_COND_SIGNAL]);
#endif
#endif
#endif /* _MSC_VER */

#ifdef USEPOSIXTHREADS
  status = pthread_cond_signal(cvp);
#endif /* USEPOSIXTHREADS */
#ifdef USEUITHREADS
  status = cond_signal(cvp);
#endif
#endif /* THR */

  return status;
}
