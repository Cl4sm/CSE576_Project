int rt_thread_numphysprocessors(void) {
  int a=1;

#ifdef THR
#if defined(__APPLE__)
  a = MPProcessorsScheduled();       /**< Number of active/running CPUs */
#endif

#ifdef _MSC_VER
  struct _SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  a = sysinfo.dwNumberOfProcessors;  /**< total number of CPUs */
#endif /* _MSC_VER */

#if defined(__PARAGON__) 
  a=2; /**< Threads-capable Paragons have 2 CPUs for computation */
#endif /* __PARAGON__ */ 

#if defined(_CRAY)
  a = sysconf(_SC_CRAY_NCPU);        /**< total number of CPUs */
#endif

#if defined(__sun) || defined(__linux) || defined(__osf__) || defined(_AIX)
  a = sysconf(_SC_NPROCESSORS_ONLN); /**< Number of active/running CPUs */
#endif /* SunOS */

#if defined(__irix)
  a = sysconf(_SC_NPROC_ONLN);       /**< Number of active/running CPUs */
#endif /* IRIX */

#if defined(__hpux)
  a = mpctl(MPC_GETNUMSPUS, 0, 0);   /**< total number of CPUs */
#endif /* HPUX */
#endif /* THR */

  return a;
}
