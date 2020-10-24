  int status=0;

#ifdef THR
#ifdef _MSC_VER
#if defined(RTUSEWIN2008CONDVARS)
  /* XXX not implemented */
#else
  CloseHandle(cvp->events[RT_COND_SIGNAL]);
  CloseHandle(cvp->events[RT_COND_BROADCAST]);
#endif
#endif /* _MSC_VER */

#ifdef USEPOSIXTHREADS
  status = pthread_cond_destroy(cvp);
#endif /* USEPOSIXTHREADS */
#ifdef USEUITHREADS
  status = cond_destroy(cvp);
#endif
#endif /* THR */

  return status;
}
