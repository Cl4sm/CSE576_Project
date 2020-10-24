int
adjust_mdebug (DSO *dso, int n, GElf_Addr start, GElf_Addr adjust)
{
  struct mdebug mdebug;
  struct { GElf_Off offset; GElf_Off size; size_t entsize; } regions [11];
  int i = 0;
  unsigned char *symptr, *endptr;

  if (start_mdebug (dso, n, &mdebug))
    return 1;

#define READ(x, y, longsize, sz) \
do {									\
  unsigned char *tmp;							\
  tmp = mdebug.buf + OFFSETOF (mdebug_hdr, x);				\
  regions[i].offset = mdebug.read_ptr (tmp);				\
  tmp = mdebug.buf + OFFSETOF (mdebug_hdr, y);				\
  if (longsize)								\
    regions[i].size = mdebug.read_ptr (tmp);				\
  else									\
    regions[i].size = mdebug.read_32 (tmp);				\
  regions[i].entsize = sz;						\
  ++i;									\
} while (0)

  READ (cbLineOffset, cbLine, 1, sizeof (char));
  READ (cbDnOffset, idnMax, 0, sizeof (mdebug_dnr));
  READ (cbPdOffset, ipdMax, 0, SIZEOF (mdebug_pdr));
  READ (cbSymOffset, isymMax, 0, SIZEOF (mdebug_sym));
  READ (cbOptOffset, ioptMax, 0, sizeof (mdebug_opt));
  READ (cbAuxOffset, iauxMax, 0, 4 * sizeof (char));
  READ (cbSsOffset, issMax, 0, sizeof (char));
  READ (cbSsExtOffset, issExtMax, 0, sizeof (char));
  READ (cbFdOffset, ifdMax, 0, SIZEOF (mdebug_fdr));
  READ (cbRfdOffset, crfd, 0, sizeof (mdebug_rfd));
  READ (cbExtOffset, iextMax, 0, SIZEOF (mdebug_ext));

#undef READ

  for (i = 0; i < 11; ++i)
    {
      if (regions[i].offset)
	regions[i].offset -= dso->mdebug_orig_offset;
      regions[i].size *= regions[i].entsize;
      if (regions[i].offset >= dso->shdr[n].sh_size
	  || regions[i].offset + regions[i].size > dso->shdr[n].sh_size)
	{
	  error (0, 0, "%s: File offsets in .mdebug header point outside of .mdebug section",
		 dso->filename);
	  return 1;
	}
    }

  /* Adjust symbols.  */
  if (regions[3].offset)
    for (symptr = mdebug.buf + regions[3].offset,
	 endptr = symptr + regions[3].size;
	 symptr < endptr;
	 symptr += regions[3].entsize)
      mdebug.adjust_sym (&mdebug, symptr, start, adjust);

  /* Adjust file descriptor's addresses.  */
  if (regions[8].offset)
    for (symptr = mdebug.buf + regions[8].offset,
	 endptr = symptr + regions[8].size;
	 symptr < endptr;
	 symptr += regions[8].entsize)
      {
	GElf_Addr addr;

	assert (offsetof (mdebug_fdr_32, adr) == 0);
	assert (offsetof (mdebug_fdr_64, adr) == 0);
	addr = mdebug.read_ptr (symptr);
	if (addr >= start)
	  mdebug.write_ptr (symptr, addr + adjust);
      }

  /* Adjust extended symbols.  */
  if (regions[10].offset)
    for (symptr = mdebug.buf + regions[10].offset
		  + OFFSETOF (mdebug_ext, asym),
	 endptr = symptr + regions[10].size;
	 symptr < endptr;
	 symptr += regions[10].entsize)
      mdebug.adjust_sym (&mdebug, symptr, start, adjust);

  return 0;
}
