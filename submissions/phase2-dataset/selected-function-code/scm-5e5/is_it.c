  char *filename;
{
  int fd;
  long filenames_cookie;
  struct exec hdr;

  /*
   * Open an executable  and check for a valid header!
   * Can't bcmp the header with what we had,  it may have been stripped!
   * so we may save looking at non executables with the same name, mostly
   * directories.
   */
  fd = open (filename, O_RDONLY);
  if (fd != -1)
    {
      if (read (fd, &hdr, sizeof (hdr)) == sizeof (hdr)
	  && !N_BADMAG (hdr) && N_DATOFF (hdr) == N_DATOFF (nhdr)
	  && N_TRELOFF (hdr) == N_TRELOFF (nhdr))
	{
	  /* compare cookies */
	  lseek (fd, N_DATOFF (hdr) + (int)&cookie - N_DATADDR (hdr), L_SET);
	  read (fd, &filenames_cookie, sizeof (filenames_cookie));
	  if (filenames_cookie == cookie)
	    {			/* Eureka */

	      /*
	       * Do the mapping
	       * The PROT_EXEC may not be needed,  but it is safer this way.
	       * should the shared library decide to indirect through
	       * addresses in the data segment not part of __DYNAMIC
	       */
	      mmap ((char *) data_start, rd_only_len, PROT_READ | PROT_EXEC,
		    MAP_FILE | MAP_SHARED | MAP_FIXED, fd,
		    N_DATOFF (hdr) + data_start - N_DATADDR (hdr));
	      close (fd);
	      return 1;
	    }
	}
      close (fd);
    }
  return 0;
}
