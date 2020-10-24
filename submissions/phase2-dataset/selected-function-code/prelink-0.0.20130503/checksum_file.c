checksum_file (int fd, size_t count,
	       void (*sum) (const void *, size_t, void *), void *arg)
{
  char buf[65536+64], *b, *p, *q;
  size_t todo = count, len;
  ssize_t n;

  b = mmap (NULL, count, PROT_READ, MAP_PRIVATE, fd, 0);
  if (b != MAP_FAILED)
    {
      sum (b, count, arg);
      munmap (b, count);
      return 0;
    }

  b = (char *) (((uintptr_t) buf + 63) & ~(uintptr_t) 63);
  while (todo > 0)
    {
      len = todo > 65536 ? 65536 : todo;
      p = b;
      q = b + len;
      while (p != q)
	{
	  n = TEMP_FAILURE_RETRY (read (fd, p, q - p));
	  if (n < 0)
	    return 1;
	  p += n;
	}
      sum (b, len, arg);
      todo -= len;
    }
  return 0;
}
