int rt_thread_create(rt_thread_t * thr, void * fctn(void *), void * arg) {
  int status=0;

#ifdef THR
#ifdef _MSC_VER
  DWORD tid; /* thread id, msvc only */
  *thr = CreateThread(NULL, 8192, (LPTHREAD_START_ROUTINE) fctn, arg, 0, &tid);
  if (*thr == NULL) {
    status = -1;
  }
#endif /* _MSC_VER */

#ifdef USEPOSIXTHREADS 
#if defined(_AIX)
  /* AIX schedule threads in system scope by default, have to ask explicitly */
  {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    status = pthread_create(thr, &attr, (RTTHREAD_START_ROUTINE)fctn, arg);
    pthread_attr_destroy(&attr);
  }
#elif defined(__PARAGON__)
  status = pthread_create(thr, pthread_attr_default, fctn, arg);
#else   
  status = pthread_create(thr, NULL, (RTTHREAD_START_ROUTINE)fctn, arg);
#endif 
#endif /* USEPOSIXTHREADS */

#ifdef USEUITHREADS 
  status = thr_create(NULL, 0, (RTTHREAD_START_ROUTINE)fctn, arg, 0, thr); 
#endif /* USEUITHREADS */
#endif /* THR */
 
  return status;
}
