#ifdef MPI
  pardata * p = (pardata *) voidhandle;

  if (p->mynode == 0) {
#if   MPI_TUNE == 0  || !defined(MPI_TUNE)
    /* 
     * Default Technique 
     */
    int outcount;
    int numtotest;
    
    numtotest = ((p->nodes - 1) < (p->count - p->curmsg)) ?
                 (p->nodes - 1) : (p->count - p->curmsg);

    MPI_Testsome(numtotest, &p->requests[p->curmsg], &outcount,
                 &p->indices[p->curmsg], &p->statuses[p->curmsg]);
    p->curmsg += p->nodes - 1;
#elif MPI_TUNE == 1
    /* 
     * Technique number 1
     */
    int index, flag;
    MPI_Testany(p->count, p->requests, &index, &flag, p->statuses);
#elif MPI_TUNE == 2
    /* 
     * Technique number 2 
     */
    int flag;
    MPI_Testall(p->count, p->requests, &flag, p->statuses);
#elif MPI_TUNE == 3
    /* 
     * Technique number 3 
     */
    int i, index, flag;
    for (i=1; i<p->nodes; i++)
      MPI_Testany(p->count, p->requests, &index, &flag, p->statuses);
#endif
  }
  else {
    MPI_Start(&p->requests[p->curmsg]);
    p->curmsg++;
  }
#endif
}
