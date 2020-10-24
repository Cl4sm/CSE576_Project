     long len;
     SCM c;
     const char *what;
{
  SCM z;
  char *ptr;
  sizet size = len;
  long nm = mallocated + size;
  VERIFY_INTS("must_malloc_cell", what);
#ifdef SHORT_SIZET
  ASRTER(len==size, MAKINUM(len), NALLOC, what);
#endif
  NEWCELL(z);
  if (nm <= mtrigger) SYSCALL(ptr = (char *)malloc(size););
  else ptr = 0;
  if (!ptr) ptr = igc_for_alloc(0L, 0L, size, what);
  else mallocated = nm;
/* printf("must_malloc_cell(%lu, %lx, %s) => %lx\n", len, c, what, ptr); fflush(stdout); */
  SETCHARS(z, ptr);
  CAR(z) = c;
  return z;
}
