  int status=0;  

#ifdef THR
#ifdef _MSC_VER
  DWORD wstatus = 0;
 
  wstatus = WAIT_TIMEOUT;
 
  while (wstatus != WAIT_OBJECT_0) {
    wstatus = WaitForSingleObject(thr, INFINITE);
  }
#endif /* _MSC_VER */

#ifdef USEPOSIXTHREADS
  status = pthread_join(thr, stat);
#endif /* USEPOSIXTHREADS */

#ifdef USEUITHREADS
  status = thr_join(thr, NULL, stat);
#endif /* USEPOSIXTHREADS */
#endif /* THR */

  return status;
}  
