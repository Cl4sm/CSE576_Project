static inline int
readonly_is_movable (DSO *dso, GElf_Ehdr *ehdr, GElf_Shdr *shdr, int k)
{
  if (! (shdr[k].sh_flags & (SHF_ALLOC | SHF_WRITE)))
    return 0;

  switch (shdr[k].sh_type)
    {
    case SHT_HASH:
    case SHT_GNU_HASH:
    case SHT_DYNSYM:
    case SHT_REL:
    case SHT_RELA:
    case SHT_STRTAB:
    case SHT_NOTE:
    case SHT_GNU_verdef:
    case SHT_GNU_verneed:
    case SHT_GNU_versym:
    case SHT_GNU_LIBLIST:
      return 1;
    default:
      if (strcmp (strptr (dso, ehdr->e_shstrndx,
			  shdr[k].sh_name), ".interp") == 0)
	return 1;
      return 0;
    }
}
