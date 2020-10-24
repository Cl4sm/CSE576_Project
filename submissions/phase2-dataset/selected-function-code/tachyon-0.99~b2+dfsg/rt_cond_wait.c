  int status=0;
#if defined(THR) && defined(_MSC_VER)
  int result=0;
  LONG last_waiter;
  LONG my_waiter;
#endif

#ifdef THR
#ifdef _MSC_VER
#if defined(RTUSEWIN2008CONDVARS)
  SleepConditionVariableCS(cvp, mp, INFINITE)
#else
#if !defined(RTUSEINTERLOCKEDATOMICOPS)
  EnterCriticalSection(&cvp->waiters_lock);
  cvp->waiters++;
  LeaveCriticalSection(&cvp->waiters_lock);
#else
  InterlockedIncrement(&cvp->waiters);
#endif

  LeaveCriticalSection(mp); /* SetEvent() keeps state, avoids lost wakeup */

  /* Wait either a single or broadcast even to become signalled */
  result = WaitForMultipleObjects(2, cvp->events, FALSE, INFINITE);

#if !defined(RTUSEINTERLOCKEDATOMICOPS)
  EnterCriticalSection (&cvp->waiters_lock);
  cvp->waiters--;
  last_waiter =
    ((result == (WAIT_OBJECT_0 + RT_COND_BROADCAST)) && cvp->waiters == 0);
  LeaveCriticalSection (&cvp->waiters_lock);
#else
  my_waiter = InterlockedDecrement(&cvp->waiters);
  last_waiter =
    ((result == (WAIT_OBJECT_0 + RT_COND_BROADCAST)) && my_waiter == 0);
#endif

  /* Some thread called cond_broadcast() */
  if (last_waiter)
    /* We're the last waiter to be notified or to stop waiting, so */
    /* reset the manual event.                                     */
    ResetEvent(cvp->events[RT_COND_BROADCAST]);

  EnterCriticalSection(mp);
#endif
#endif /* _MSC_VER */

#ifdef USEPOSIXTHREADS
  status = pthread_cond_wait(cvp, mp);
#endif /* USEPOSIXTHREADS */
#ifdef USEUITHREADS
  status = cond_wait(cvp, mp);
#endif
#endif /* THR */

  return status;
}
