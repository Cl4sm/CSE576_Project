     int new;
     register char *ptr, *end;
{
  register int i, nwrite, ret;
  char buf[80];
  extern int errno;
  /* This is the normal amount to write at once.
     It is the size of block that NFS uses.  */
  int writesize = 1 << 13;
  int pagesize = getpagesize ();
  char zeros[1 << 13];

  bzero (zeros, sizeof (zeros));

  for (i = 0; ptr < end;)
    {
      /* Distance to next multiple of writesize.  */
      nwrite = (((int) ptr + writesize) & -writesize) - (int) ptr;
      /* But not beyond specified end.  */
      if (nwrite > end - ptr) nwrite = end - ptr;
      ret = write (new, ptr, nwrite);
      /* If write gets a page fault, it means we reached
	 a gap between the old text segment and the old data segment.
	 This gap has probably been remapped into part of the text segment.
	 So write zeros for it.  */
      if (ret == -1
#ifdef EFAULT
	  && errno == EFAULT
#endif
	  )
	{
	  /* Write only a page of zeros at once,
	     so that we we don't overshoot the start
	     of the valid memory in the old data segment.  */
	  if (nwrite > pagesize)
	    nwrite = pagesize;
	  write (new, zeros, nwrite);
	}
#if 0 /* Now that we have can ask `write' to write more than a page,
	 it is legit for write do less than the whole amount specified.  */
      else if (nwrite != ret)
	{
	  sprintf (buf,
		   "unexec write failure: addr 0x%x, fileno %d, size 0x%x, wrote 0x%x, errno %d",
		   ptr, new, nwrite, ret, errno);
	  PERROR (buf);
	}
#endif
      i += nwrite;
      ptr += nwrite;
    }
}
