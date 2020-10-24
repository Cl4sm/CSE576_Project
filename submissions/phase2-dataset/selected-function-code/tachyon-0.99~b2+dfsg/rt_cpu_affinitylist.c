  int *affinitylist = NULL;
  *cpuaffinitycount = -1; /* return count -1 if unimplemented or err occurs */

/* Win32 process affinity mask query */
#if 0 && defined(_MSC_VER)
  /* XXX untested, but based on the linux code, may work with a few tweaks */
  HANDLE myproc = GetCurrentProcess(); /* returns a psuedo-handle */
  DWORD affinitymask, sysaffinitymask;

  if (!GetProcessAffinityMask(myproc, &affinitymask, &sysaffinitymask)) {
    /* count length of affinity list */
    int affinitycount=0;
    int i;
    for (i=0; i<31; i++) {
      affinitycount += (affinitymask >> i) & 0x1;
    }

    /* build affinity list */
    if (affinitycount > 0) {
      affinitylist = (int *) malloc(affinitycount * sizeof(int));
      if (affinitylist == NULL)
        return NULL;

      int curcount = 0;
      for (i=0; i<CPU_SETSIZE; i++) {
        if (CPU_ISSET(i, &affinitymask)) {
          affinitylist[curcount] = i;
          curcount++;
        }
      }
    }

    *cpuaffinitycount = affinitycount; /* return final affinity list */
  }
#endif

/* Linux process affinity mask query */
#if defined(__linux)

/* protect ourselves from some older Linux distros */
#if defined(CPU_SETSIZE)
  int i;
  cpu_set_t affinitymask;
  int affinitycount=0;

  /* PID 0 refers to the current process */
  if (sched_getaffinity(0, sizeof(affinitymask), &affinitymask) < 0) {
    perror("rt_cpu_affinitylist: sched_getaffinity");
    return NULL;
  }

  /* count length of affinity list */
  for (i=0; i<CPU_SETSIZE; i++) {
    affinitycount += CPU_ISSET(i, &affinitymask);
  }

  /* build affinity list */
  if (affinitycount > 0) {
    affinitylist = (int *) malloc(affinitycount * sizeof(int));
    if (affinitylist == NULL)
      return NULL;

    int curcount = 0;
    for (i=0; i<CPU_SETSIZE; i++) {
      if (CPU_ISSET(i, &affinitymask)) {
        affinitylist[curcount] = i;
        curcount++;
      }
    }
  }

  *cpuaffinitycount = affinitycount; /* return final affinity list */
#endif
#endif

  /* MacOS X 10.5.x has a CPU affinity query/set capability finally      */
  /* http://developer.apple.com/releasenotes/Performance/RN-AffinityAPI/ */

  /* Solaris and HP-UX use pset_bind() and related functions, and they   */
  /* don't use the single-level mask-based scheduling mechanism that     */
  /* the others, use.  Instead, they use a hierarchical tree of          */
  /* processor sets and processes float within those, or are tied to one */
  /* processor that's a member of a particular set.                      */

  return affinitylist;
}
