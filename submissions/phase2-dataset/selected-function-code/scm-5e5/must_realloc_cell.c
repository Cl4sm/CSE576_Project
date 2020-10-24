void must_realloc_cell(z, olen, len, what)
     SCM z;
     long olen, len;
     const char *what;
{
  char *ptr, *where = CHARS(z);
  sizet size = len;
  long nm = mallocated + size - olen;
  VERIFY_INTS("must_realloc_cell", what);
#ifdef SHORT_SIZET
  ASRTER(len==size, MAKINUM(len), NALLOC, what);
#endif
  ASRTER(!errjmp_bad, MAKINUM(len), NALLOC, what);
/* printf("must_realloc_cell(%lx, %lu, %lu, %s)\n", z, olen, len, what); fflush(stdout); */
  if (nm <= mtrigger) SYSCALL(ptr = (char *)realloc(where, size););
  else ptr = 0;
  if (!ptr) ptr = igc_for_alloc(where, olen, size, what);
  else mallocated = nm;
  SETCHARS(z, ptr);
}
