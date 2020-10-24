static int
make_hdr (new, a_out, data_start, bss_start, entry_address, a_name, new_name)
     int new, a_out;
     unsigned data_start, bss_start, entry_address;
     char *a_name;
     char *new_name;
{
  int tem;
#ifdef COFF
  auto struct scnhdr f_thdr;		/* Text section header */
  auto struct scnhdr f_dhdr;		/* Data section header */
  auto struct scnhdr f_bhdr;		/* Bss section header */
  auto struct scnhdr scntemp;		/* Temporary section header */
  register int scns;
#endif /* COFF */
#ifdef USG_SHARED_LIBRARIES
  extern unsigned int bss_end;
#else
  unsigned int bss_end;
#endif

  pagemask = getpagesize () - 1;

  /* Adjust text/data boundary. */
#ifdef NO_REMAP
  data_start = (int) start_of_data ();
#else /* not NO_REMAP */
  if (!data_start)
    data_start = (int) start_of_data ();
#endif /* not NO_REMAP */
  data_start = ADDR_CORRECT (data_start);

#ifdef SEGMENT_MASK
  data_start = data_start & ~SEGMENT_MASK; /* (Down) to segment boundary. */
#else
  data_start = data_start & ~pagemask; /* (Down) to page boundary. */
#endif

  bss_end = ADDR_CORRECT (sbrk (0)) + pagemask;
  bss_end &= ~ pagemask;

  /* Adjust data/bss boundary. */
  if (bss_start != 0)
    {
      bss_start = (ADDR_CORRECT (bss_start) + pagemask);
      /* (Up) to page bdry. */
      bss_start &= ~ pagemask;
      if (bss_start > bss_end)
	{
	  ERROR1 ("unexec: Specified bss_start (%u) is past end of program",
		  bss_start);
	}
    }
  else
    bss_start = bss_end;

  if (data_start > bss_start)	/* Can't have negative data size. */
    {
      ERROR2 ("unexec: data_start (%u) can't be greater than bss_start (%u)",
	      data_start, bss_start);
    }

#ifdef COFF
  /* Salvage as much info from the existing file as possible */
  if (a_out >= 0)
    {
      if (read (a_out, &f_hdr, sizeof (f_hdr)) != sizeof (f_hdr))
	{
	  PERROR (a_name);
	}
      block_copy_start += sizeof (f_hdr);
      if (f_hdr.f_opthdr > 0)
	{
	  if (read (a_out, &f_ohdr, sizeof (f_ohdr)) != sizeof (f_ohdr))
	    {
	      PERROR (a_name);
	    }
	  block_copy_start += sizeof (f_ohdr);
	}
      /* Loop through section headers, copying them in */
      lseek (a_out, sizeof (f_hdr) + f_hdr.f_opthdr, 0);
      for (scns = f_hdr.f_nscns; scns > 0; scns--) {
	if (read (a_out, &scntemp, sizeof (scntemp)) != sizeof (scntemp))
	  {
	    PERROR (a_name);
	  }
	if (scntemp.s_scnptr > 0L)
	  {
            if (block_copy_start < scntemp.s_scnptr + scntemp.s_size)
	      block_copy_start = scntemp.s_scnptr + scntemp.s_size;
	  }
	if (strcmp (scntemp.s_name, ".text") == 0)
	  {
	    f_thdr = scntemp;
	  }
	else if (strcmp (scntemp.s_name, ".data") == 0)
	  {
	    f_dhdr = scntemp;
	  }
	else if (strcmp (scntemp.s_name, ".bss") == 0)
	  {
	    f_bhdr = scntemp;
	  }
      }
    }
  else
    {
      ERROR0 ("can't build a COFF file from scratch yet");
    }

  /* Now we alter the contents of all the f_*hdr variables
     to correspond to what we want to dump.  */

#ifdef USG_SHARED_LIBRARIES

  /* The amount of data we're adding to the file is distance from the
   * end of the original .data space to the current end of the .data
   * space.
   */

  bias = bss_start - (f_ohdr.data_start + f_dhdr.s_size);

#endif

  f_hdr.f_flags |= (F_RELFLG | F_EXEC);
#ifdef TPIX
  f_hdr.f_nscns = 3;
#endif
#ifdef EXEC_MAGIC
  f_ohdr.magic = EXEC_MAGIC;
#endif
#ifndef NO_REMAP
  f_ohdr.text_start = (long) start_of_text ();
  f_ohdr.tsize = data_start - f_ohdr.text_start;
  f_ohdr.data_start = data_start;
#endif /* NO_REMAP */
  f_ohdr.dsize = bss_start - f_ohdr.data_start;
  f_ohdr.bsize = bss_end - bss_start;
#ifndef KEEP_OLD_TEXT_SCNPTR
  /* On some machines, the old values are right.
     ??? Maybe on all machines with NO_REMAP.  */
  f_thdr.s_size = f_ohdr.tsize;
  f_thdr.s_scnptr = sizeof (f_hdr) + sizeof (f_ohdr);
  f_thdr.s_scnptr += (f_hdr.f_nscns) * (sizeof (f_thdr));
#endif /* KEEP_OLD_TEXT_SCNPTR */
#ifdef ADJUST_TEXT_SCNHDR_SIZE
  /* On some machines, `text size' includes all headers.  */
  f_thdr.s_size -= f_thdr.s_scnptr;
#endif /* ADJUST_TEST_SCNHDR_SIZE */
  lnnoptr = f_thdr.s_lnnoptr;
#ifdef SECTION_ALIGNMENT
  /* Some systems require special alignment
     of the sections in the file itself.  */
  f_thdr.s_scnptr
    = (f_thdr.s_scnptr + SECTION_ALIGNMENT) & ~SECTION_ALIGNMENT;
#endif /* SECTION_ALIGNMENT */
#ifdef TPIX
  f_thdr.s_scnptr = 0xd0;
#endif
  text_scnptr = f_thdr.s_scnptr;
#ifdef ADJUST_TEXTBASE
  text_scnptr = sizeof (f_hdr) + sizeof (f_ohdr) + (f_hdr.f_nscns) * (sizeof (f_thdr));
#endif
#ifndef KEEP_OLD_PADDR
  f_dhdr.s_paddr = f_ohdr.data_start;
#endif /* KEEP_OLD_PADDR */
  f_dhdr.s_vaddr = f_ohdr.data_start;
  f_dhdr.s_size = f_ohdr.dsize;
  f_dhdr.s_scnptr = f_thdr.s_scnptr + f_thdr.s_size;
#ifdef SECTION_ALIGNMENT
  /* Some systems require special alignment
     of the sections in the file itself.  */
  f_dhdr.s_scnptr
    = (f_dhdr.s_scnptr + SECTION_ALIGNMENT) & ~SECTION_ALIGNMENT;
#endif /* SECTION_ALIGNMENT */
#ifdef DATA_SECTION_ALIGNMENT
  /* Some systems require special alignment
     of the data section only.  */
  f_dhdr.s_scnptr
    = (f_dhdr.s_scnptr + DATA_SECTION_ALIGNMENT) & ~DATA_SECTION_ALIGNMENT;
#endif /* DATA_SECTION_ALIGNMENT */
  data_scnptr = f_dhdr.s_scnptr;
#ifndef KEEP_OLD_PADDR
  f_bhdr.s_paddr = f_ohdr.data_start + f_ohdr.dsize;
#endif /* KEEP_OLD_PADDR */
  f_bhdr.s_vaddr = f_ohdr.data_start + f_ohdr.dsize;
  f_bhdr.s_size = f_ohdr.bsize;
  f_bhdr.s_scnptr = 0L;
#ifndef USG_SHARED_LIBRARIES
  bias = f_dhdr.s_scnptr + f_dhdr.s_size - block_copy_start;
#endif

  if (f_hdr.f_symptr > 0L)
    {
      f_hdr.f_symptr += bias;
    }

  if (f_thdr.s_lnnoptr > 0L)
    {
      f_thdr.s_lnnoptr += bias;
    }

#ifdef ADJUST_EXEC_HEADER
  ADJUST_EXEC_HEADER;
#endif /* ADJUST_EXEC_HEADER */

  if (write (new, &f_hdr, sizeof (f_hdr)) != sizeof (f_hdr))
    {
      PERROR (new_name);
    }

  if (write (new, &f_ohdr, sizeof (f_ohdr)) != sizeof (f_ohdr))
    {
      PERROR (new_name);
    }

#ifndef USG_SHARED_LIBRARIES

  if (write (new, &f_thdr, sizeof (f_thdr)) != sizeof (f_thdr))
    {
      PERROR (new_name);
    }

  if (write (new, &f_dhdr, sizeof (f_dhdr)) != sizeof (f_dhdr))
    {
      PERROR (new_name);
    }

  if (write (new, &f_bhdr, sizeof (f_bhdr)) != sizeof (f_bhdr))
    {
      PERROR (new_name);
    }

#else /* USG_SHARED_LIBRARIES */

  /* The purpose of this code is to write out the new file's section
   * header table.
   *
   * Scan through the original file's sections.  If the encountered
   * section is one we know (.text, .data or .bss), write out the
   * correct header.  If it is a section we do not know (such as
   * .lib), adjust the address of where the section data is in the
   * file, and write out the header.
   *
   * If any section precedes .text or .data in the file, this code
   * will not adjust the file pointer for that section correctly.
   */

  /* This used to use sizeof (f_ohdr) instead of .f_opthdr.
     .f_opthdr is said to be right when there is no optional header.  */
  lseek (a_out, sizeof (f_hdr) + f_hdr.f_opthdr, 0);

  for (scns = f_hdr.f_nscns; scns > 0; scns--)
    {
      if (read (a_out, &scntemp, sizeof (scntemp)) != sizeof (scntemp))
	PERROR (a_name);

      if (!strcmp (scntemp.s_name, f_thdr.s_name))	/* .text */
	{
	  if (write (new, &f_thdr, sizeof (f_thdr)) != sizeof (f_thdr))
	    PERROR (new_name);
	}
      else if (!strcmp (scntemp.s_name, f_dhdr.s_name))	/* .data */
	{
	  if (write (new, &f_dhdr, sizeof (f_dhdr)) != sizeof (f_dhdr))
	    PERROR (new_name);
	}
      else if (!strcmp (scntemp.s_name, f_bhdr.s_name))	/* .bss */
	{
	  if (write (new, &f_bhdr, sizeof (f_bhdr)) != sizeof (f_bhdr))
	    PERROR (new_name);
	}
      else
	{
	  if (scntemp.s_scnptr)
	    scntemp.s_scnptr += bias;
	  if (write (new, &scntemp, sizeof (scntemp)) != sizeof (scntemp))
	    PERROR (new_name);
	}
    }
#endif /* USG_SHARED_LIBRARIES */

  return (0);

#else /* if not COFF */

  /* Get symbol table info from header of a.out file if given one. */
  if (a_out >= 0)
    {
#ifdef COFF_ENCAPSULATE
      if (read (a_out, &coffheader, sizeof coffheader) != sizeof coffheader)
	{
	  PERROR(a_name);
	}
      if (coffheader.f_magic != COFF_MAGIC)
	{
	  ERROR1("%s doesn't have legal coff magic number\n", a_name);
	}
#endif
      if (read (a_out, &ohdr, sizeof hdr) != sizeof hdr)
	{
	  PERROR (a_name);
	}

      if (N_BADMAG (ohdr))
	{
	  ERROR1 ("invalid magic number in %s", a_name);
	}
      hdr = ohdr;
    }
  else
    {
#ifdef COFF_ENCAPSULATE
      /* We probably could without too much trouble. The code is in gld
       * but I don't have that much time or incentive.
       */
      ERROR0 ("can't build a COFF file from scratch yet");
#else
#ifdef MSDOS	/* Demacs 1.1.1 91/10/16 HIRANO Satoshi */
      bzero ((void *)&hdr, sizeof hdr);
#else
      bzero (&hdr, sizeof hdr);
#endif
#endif
    }

  unexec_text_start = (long) start_of_text ();
  unexec_data_start = data_start;

  /* Machine-dependent fixup for header, or maybe for unexec_text_start */
#ifdef ADJUST_EXEC_HEADER
  ADJUST_EXEC_HEADER;
#endif /* ADJUST_EXEC_HEADER */

  hdr.a_trsize = 0;
  hdr.a_drsize = 0;
  if (entry_address != 0)
    hdr.a_entry = entry_address;

  hdr.a_bss = bss_end - bss_start;
  hdr.a_data = bss_start - data_start;
#ifdef NO_REMAP
  hdr.a_text = ohdr.a_text;
#else /* not NO_REMAP */
  hdr.a_text = data_start - unexec_text_start;

#ifdef A_TEXT_OFFSET
  hdr.a_text += A_TEXT_OFFSET (ohdr);
#endif

#endif /* not NO_REMAP */

#ifdef COFF_ENCAPSULATE
  /* We are encapsulating BSD format within COFF format.  */
  {
    struct coffscn *tp, *dp, *bp;
    tp = &coffheader.scns[0];
    dp = &coffheader.scns[1];
    bp = &coffheader.scns[2];
    tp->s_size = hdr.a_text + sizeof(struct exec);
    dp->s_paddr = data_start;
    dp->s_vaddr = data_start;
    dp->s_size = hdr.a_data;
    bp->s_paddr = dp->s_vaddr + dp->s_size;
    bp->s_vaddr = bp->s_paddr;
    bp->s_size = hdr.a_bss;
    coffheader.tsize = tp->s_size;
    coffheader.dsize = dp->s_size;
    coffheader.bsize = bp->s_size;
    coffheader.text_start = tp->s_vaddr;
    coffheader.data_start = dp->s_vaddr;
  }
  if (write (new, &coffheader, sizeof coffheader) != sizeof coffheader)
    {
      PERROR(new_name);
    }
#endif /* COFF_ENCAPSULATE */

  if (write (new, &hdr, sizeof hdr) != sizeof hdr)
    {
      PERROR (new_name);
    }

#if 0 /* This #ifndef caused a bug on Linux when using QMAGIC.  */
  /* This adjustment was done above only #ifndef NO_REMAP,
     so only undo it now #ifndef NO_REMAP.  */
  /* #ifndef NO_REMAP  */
#endif
#ifdef A_TEXT_OFFSET
  hdr.a_text -= A_TEXT_OFFSET (ohdr);
#endif

  return 0;

#endif /* not COFF */
}
