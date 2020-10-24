copy_text_and_data (new, a_out)
     int new, a_out;
{
  register char *end;
  register char *ptr;

#ifdef COFF

#ifdef USG_SHARED_LIBRARIES

  int scns;
  struct scnhdr scntemp;		/* Temporary section header */

  /* The purpose of this code is to write out the new file's section
   * contents.
   *
   * Step through the section table.  If we know the section (.text,
   * .data) do the appropriate thing.  Otherwise, if the section has
   * no allocated space in the file (.bss), do nothing.  Otherwise,
   * the section has space allocated in the file, and is not a section
   * we know.  So just copy it.
   */

  lseek (a_out, sizeof (struct filehdr) + sizeof (struct aouthdr), 0);

  for (scns = f_hdr.f_nscns; scns > 0; scns--)
    {
      if (read (a_out, &scntemp, sizeof (scntemp)) != sizeof (scntemp))
	PERROR ("temacs");

      if (!strcmp (scntemp.s_name, ".text"))
	{
	  lseek (new, (long) text_scnptr, 0);
	  ptr = (char *) f_ohdr.text_start;
	  end = ptr + f_ohdr.tsize;
	  write_segment (new, ptr, end);
	}
      else if (!strcmp (scntemp.s_name, ".data"))
	{
	  lseek (new, (long) data_scnptr, 0);
	  ptr = (char *) f_ohdr.data_start;
	  end = ptr + f_ohdr.dsize;
	  write_segment (new, ptr, end);
	}
      else if (!scntemp.s_scnptr)
	; /* do nothing - no data for this section */
      else
	{
	  char page[BUFSIZ];
	  int size, n;
	  long old_a_out_ptr = lseek (a_out, 0, 1);

	  lseek (a_out, scntemp.s_scnptr, 0);
	  for (size = scntemp.s_size; size > 0; size -= sizeof (page))
	    {
	      n = size > sizeof (page) ? sizeof (page) : size;
	      if (read (a_out, page, n) != n || write (new, page, n) != n)
		PERROR ("emacs");
	    }
	  lseek (a_out, old_a_out_ptr, 0);
	}
    }

#else /* COFF, but not USG_SHARED_LIBRARIES */

#ifdef MSDOS
#if __DJGPP__ >= 2
  /* Dump the original table of exception handlers, not the one
     where our exception hooks are registered.  */
  __djgpp_exception_toggle ();

  /* Switch off startup flags that might have been set at runtime
     and which might change the way that dumped Emacs works.  */
  save_djgpp_startup_flags = _crt0_startup_flags;
  _crt0_startup_flags &= ~(_CRT0_FLAG_NO_LFN | _CRT0_FLAG_NEARPTR);
#endif
#endif

  lseek (new, (long) text_scnptr, 0);
  ptr = (char *) f_ohdr.text_start;
#ifdef HEADER_INCL_IN_TEXT
  /* For Gould UTX/32, text starts after headers */
  ptr = (char *) (ptr + text_scnptr);
#endif /* HEADER_INCL_IN_TEXT */
  end = ptr + f_ohdr.tsize;
  write_segment (new, ptr, end);

  lseek (new, (long) data_scnptr, 0);
  ptr = (char *) f_ohdr.data_start;
  end = ptr + f_ohdr.dsize;
  write_segment (new, ptr, end);

#ifdef MSDOS
#if __DJGPP__ >= 2
  /* Restore our exception hooks.  */
  __djgpp_exception_toggle ();

  /* Restore the startup flags.  */
  _crt0_startup_flags = save_djgpp_startup_flags;
#endif
#endif

#endif /* USG_SHARED_LIBRARIES */

#else /* if not COFF */

/* Some machines count the header as part of the text segment.
   That is to say, the header appears in core
   just before the address that start_of_text returns.
   For them, N_TXTOFF is the place where the header goes.
   We must adjust the seek to the place after the header.
   Note that at this point hdr.a_text does *not* count
   the extra A_TEXT_OFFSET bytes, only the actual bytes of code.  */

#ifdef A_TEXT_SEEK
  lseek (new, (long) A_TEXT_SEEK (hdr), 0);
#else
  lseek (new, (long) N_TXTOFF (hdr), 0);
#endif /* no A_TEXT_SEEK */

#ifdef RISCiX

  /* Acorn's RISC-iX has a wacky way of initialising the position of the heap.
   * There is a little table in crt0.o that is filled at link time with
   * the min and current brk positions, among other things.  When start
   * runs, it copies the table to where these parameters live during
   * execution.  This data is in text space, so it cannot be modified here
   * before saving the executable, so the data is written manually.  In
   * addition, the table does not have a label, and the nearest accessible
   * label (mcount) is not prefixed with a '_', thus making it inaccessible
   * from within C programs.  To overcome this, emacs's executable is passed
   * through the command 'nm %s | fgrep mcount' into a pipe, and the
   * resultant output is then used to find the address of 'mcount'.  As far as
   * is possible to determine, in RISC-iX releases prior to 1.2, the negative
   * offset of the table from mcount is 0x2c, whereas from 1.2 onwards it is
   * 0x30.  bss_end has been rounded up to page boundary.  This solution is
   * based on suggestions made by Kevin Welton and Steve Hunt of Acorn, and
   * avoids the need for a custom version of crt0.o for emacs which has its
   * table in data space.
   */

  {
    char command[1024];
    char errbuf[1024];
    char address_text[32];
    int  proforma[4];
    FILE *pfile;
    char *temp_ptr;
    char c;
    int mcount_address, mcount_offset, count;
    extern char *_execname;
   

    /* The use of _execname is incompatible with RISCiX 1.1 */
    sprintf (command, "nm %s | fgrep mcount", _execname);

    if ( (pfile = popen(command, "r")) == NULL)
    {
      sprintf (errbuf, "Could not open pipe");
      PERROR (errbuf);
    }

    count=0;
    while ( ((c=getc(pfile)) != EOF) && (c != ' ') && (count < 31))
      address_text[count++]=c;
    address_text[count]=0;

    if ((count == 0) || pclose(pfile) != NULL)
    {
      sprintf (errbuf, "Failed to execute the command '%s'\n", command);
      PERROR (errbuf);
    }  

    sscanf(address_text, "%x", &mcount_address);
    ptr = (char *) unexec_text_start;
    mcount_offset = (char *)mcount_address - ptr;

#ifdef RISCiX_1_1
#define EDATA_OFFSET 0x2c
#else
#define EDATA_OFFSET 0x30
#endif

    end = ptr + mcount_offset - EDATA_OFFSET;

    write_segment (new, ptr, end);

    proforma[0] = bss_end;	/* becomes _edata */
    proforma[1] = bss_end;	/* becomes _end */
    proforma[2] = bss_end;	/* becomes _minbrk */
    proforma[3] = bss_end;	/* becomes _curbrk */

    write (new, proforma, 16);

    temp_ptr = ptr;
    ptr = end + 16;
    end = temp_ptr + hdr.a_text;

    write_segment (new, ptr, end);
  }

#else /* !RISCiX */
  ptr = (char *) unexec_text_start;
  end = ptr + hdr.a_text;
  write_segment (new, ptr, end);
#endif /* RISCiX */

  ptr = (char *) unexec_data_start;
  end = ptr + hdr.a_data;
/*  This lseek is certainly incorrect when A_TEXT_OFFSET
    and I believe it is a no-op otherwise.
    Let's see if its absence ever fails.  */
/*  lseek (new, (long) N_TXTOFF (hdr) + hdr.a_text, 0); */
  write_segment (new, ptr, end);

#endif /* not COFF */

  return 0;
}
