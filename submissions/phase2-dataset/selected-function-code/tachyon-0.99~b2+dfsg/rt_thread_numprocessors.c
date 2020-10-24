int rt_thread_numprocessors(void) {
  int a=1;

#ifdef THR
  /* Allow the user to override the number of CPUs for use */
  /* in scalability testing, debugging, etc.               */
  char *forcecount = getenv("RTFORCECPUCOUNT");
  if (forcecount != NULL) {
    if (sscanf(forcecount, "%d", &a) == 1) {
      return a; /* if we got a valid count, return it */
    } else {
      a=1;      /* otherwise use the real available hardware CPU count */
    }
  }

  /* otherwise return the number of physical processors currently available */
  a = rt_thread_numphysprocessors();

  /* XXX we should add checking for the current CPU affinity masks here, */
  /* and return the min of the physical processor count and CPU affinity */
  /* mask enabled CPU count.                                             */
#endif /* THR */

  return a;
}
