     char *where;
     long olen, len;
     const char *what;
{
  char *ptr;
  sizet size = len;
  long nm = mallocated + size - olen;
  VERIFY_INTS("must_realloc", what);
#ifdef SHORT_SIZET
  ASRTER(len==size, MAKINUM(len), NALLOC, what);
#endif
  ASRTER(!errjmp_bad, MAKINUM(len), NALLOC, what);
/* printf("must_realloc(%lx, %lu, %lu, %s)\n", where, olen, len, what); fflush(stdout);
   printf("nm = %ld <= mtrigger = %ld: %d; size = %u\n", nm, mtrigger, (nm <= mtrigger), size); fflush(stdout); */
  if (nm <= mtrigger) SYSCALL(ptr = (char *)realloc(where, size););
  else ptr = 0;
  if (!ptr) ptr = igc_for_alloc(where, olen, size, what);
  else mallocated = nm;
  return ptr;
}
