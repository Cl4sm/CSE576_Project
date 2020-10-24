finalize_mdebug (DSO *dso)
{
  int i;
  struct mdebug mdebug;
  GElf_Addr adj;

  for (i = 1; i < dso->ehdr.e_shnum; i++)
    if ((dso->arch->machine == EM_ALPHA
	 && dso->shdr[i].sh_type == SHT_ALPHA_DEBUG)
	|| (dso->arch->machine == EM_MIPS
	    && dso->shdr[i].sh_type == SHT_MIPS_DEBUG))
      break;

  assert (i < dso->ehdr.e_shnum);

  /* If .mdebug's file position did not change, there is nothing to do.  */
  adj = dso->shdr[i].sh_offset - dso->mdebug_orig_offset;
  if (! adj)
    return 0;

  if (start_mdebug (dso, i, &mdebug))
    return 1;

#define ADJUST(x) \
do {									\
  unsigned char *tmp;							\
  GElf_Addr val;							\
  tmp = mdebug.buf + OFFSETOF (mdebug_hdr, x);				\
  val = mdebug.read_ptr (tmp);						\
  if (! val)								\
    break;								\
  val += adj;								\
  if (val < dso->shdr[i].sh_offset					\
      || val >= dso->shdr[i].sh_offset + dso->shdr[i].sh_size)		\
    {									\
      error (0, 0, "%s: File offsets in .mdebug header point outside of .mdebug section", \
	     dso->filename);						\
      return 1;								\
    }									\
  mdebug.write_ptr (tmp, val);						\
} while (0)

  ADJUST (cbLineOffset);
  ADJUST (cbDnOffset);
  ADJUST (cbPdOffset);
  ADJUST (cbSymOffset);
  ADJUST (cbOptOffset);
  ADJUST (cbAuxOffset);
  ADJUST (cbSsOffset);
  ADJUST (cbSsExtOffset);
  ADJUST (cbFdOffset);
  ADJUST (cbRfdOffset);
  ADJUST (cbExtOffset);

#undef ADJUST
  return 0;
}
