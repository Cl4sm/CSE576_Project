send_file (int outfd, int infd, off_t *poff, size_t count)
{
  char buf[65536], *b, *p, *q;
  size_t todo = count, len;
  ssize_t n;

  b = mmap (NULL, count, PROT_READ, MAP_PRIVATE, infd, *poff);
  if (b != MAP_FAILED)
    {
      p = b;
      q = p + count;
      while (p != q)
	{
	  n = TEMP_FAILURE_RETRY (write (outfd, p, q - p));
	  if (n <= 0)
	    {
	      munmap (b, count);
	      return -1;
	    }
	  p += n;
	}
      munmap (b, count);
      return count;
    }

  if (lseek (infd, *poff, SEEK_SET) != *poff)
    return -1;
  while (todo > 0)
    {
      len = todo > sizeof (buf) ? sizeof (buf) : todo;
      p = buf;
      q = buf + len;
      while (p != q)
	{
	  n = TEMP_FAILURE_RETRY (read (infd, p, q - p));
	  if (n <= 0)
	    return -1;
	  p += n;
	}
      p = buf;
      while (p != q)
	{
	  n = TEMP_FAILURE_RETRY (write (outfd, p, q - p));
	  if (n <= 0)
	    return -1;
	  p += n;
	}
      todo -= len;
    }
  return count;
}
