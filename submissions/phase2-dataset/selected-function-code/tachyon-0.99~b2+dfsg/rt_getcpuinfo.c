  int numnodes = rt_numnodes();
  int mynode = rt_mynode();
#ifdef MPI
  int namelen; 
  char namebuf[MPI_MAX_PROCESSOR_NAME];
#endif
 
  *nodes = (nodeinfo *) malloc(numnodes * sizeof(nodeinfo));
  (*nodes)[mynode].numcpus = rt_thread_numprocessors(); 
  (*nodes)[mynode].cpuspeed = 1.0; 
  (*nodes)[mynode].nodespeed = (*nodes)[mynode].numcpus * 
                                  (*nodes)[mynode].cpuspeed; 


#ifdef MPI
  MPI_Get_processor_name((char *) &namebuf, &namelen);
  strncpy((char *) &(*nodes)[mynode].machname, namebuf,
          (((namelen + 1) < 511) ? (namelen+1) : 511));
  MPI_Allgather(&(*nodes)[mynode], sizeof(nodeinfo), MPI_BYTE, 
                &(*nodes)[     0], sizeof(nodeinfo), MPI_BYTE, 
                MPI_COMM_WORLD);
#else
#if defined(_MSC_VER)
  strcpy((*nodes)[mynode].machname, "Windows");
#elif defined(MCOS)
  strcpy((*nodes)[mynode].machname, "Mercury");
#else
  gethostname((*nodes)[mynode].machname, 511);
#endif
#endif

  return numnodes;
}
