  int status=-1; /* unsupported by default */

#ifdef THR

#if defined(__linux) && defined(CPU_ZERO) && defined(CPU_SET)
#if 0
  /* XXX this code is too new even for RHEL4, though it runs on Fedora 7 */
  /* and other newer revs.                                               */
  /* NPTL systems can assign per-thread affinities this way              */
  cpu_set_t affinitymask;
  CPU_ZERO(&affinitymask);
  CPU_SET(cpu, &affinitymask);
  status = pthread_setaffinity_np(pthread_self(), sizeof(affinitymask), &affinitymask);
#else
  /* non-NPTL systems based on the clone() API must use this method      */
  cpu_set_t affinitymask;
  CPU_ZERO(&affinitymask);
  CPU_SET(cpu, &affinitymask);

  /* PID 0 refers to the current process */
  if ((status=sched_setaffinity(0, sizeof(affinitymask), &affinitymask)) < 0) {
    perror("rt_thread_set_self_cpuaffinitylist: sched_setaffinity");
    return status;
  }
#endif

  /* call sched_yield() so new affinity mask takes effect immediately */
  sched_yield();
#endif /* linux */

  /* MacOS X 10.5.x has a CPU affinity query/set capability finally      */
  /* http://developer.apple.com/releasenotes/Performance/RN-AffinityAPI/ */

  /* Solaris and HP-UX use pset_bind() and related functions, and they   */
  /* don't use the single-level mask-based scheduling mechanism that     */
  /* the others, use.  Instead, they use a hierarchical tree of          */
  /* processor sets and processes float within those, or are tied to one */
  /* processor that's a member of a particular set.                      */
#endif

  return status;
}
