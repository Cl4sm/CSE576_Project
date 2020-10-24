void
print_sections (DSO *dso, GElf_Ehdr *ehdr, GElf_Shdr *shdr)
{
  int elf64 = ehdr->e_ident[EI_CLASS] == ELFCLASS64;
  int i, j, shf, flag;
  char buf[32], *q;
  const char *p;
  static struct { int sh_type; const char *type_name; } types[] =
    {
      { SHT_NULL, "NULL" },
      { SHT_PROGBITS, "PROGBITS" },
      { SHT_SYMTAB, "SYMTAB" },
      { SHT_STRTAB, "STRTAB" },
      { SHT_RELA, "RELA" },
      { SHT_HASH, "HASH" },
      { SHT_DYNAMIC, "DYNAMIC" },
      { SHT_NOTE, "NOTE" },
      { SHT_NOBITS, "NOBITS" },
      { SHT_REL, "REL" },
      { SHT_SHLIB, "SHLIB" },
      { SHT_DYNSYM, "DYNSYM" },
      { SHT_INIT_ARRAY, "INIT_ARRAY" },
      { SHT_FINI_ARRAY, "FINI_ARRAY" },
      { SHT_PREINIT_ARRAY, "PREINIT_ARRAY" },
      { SHT_GROUP, "GROUP" },
      { SHT_SYMTAB_SHNDX, "SYMTAB SECTION INDICIES" },
      { SHT_GNU_verdef, "VERDEF" },
      { SHT_GNU_verneed, "VERNEED" },
      { SHT_GNU_versym, "VERSYM" },
      { SHT_GNU_LIBLIST, "LIBLIST" },
      { SHT_GNU_HASH, "GNU_HASH" },
      { 0, NULL }
    };

  if (elf64)
    printf ("  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al\n");
  else
    printf ("  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n");
  for (i = 0; i < ehdr->e_shnum; ++i)
    {
      p = NULL;
      for (j = 0; types[j].type_name; ++j)
	if (types[j].sh_type == shdr[i].sh_type)
	  {
	    p = types[j].type_name;
	    break;
	  }

      if (p == NULL)
	{
	  if (shdr[i].sh_type >= SHT_LOPROC && shdr[i].sh_type <= SHT_HIPROC)
	    sprintf (buf, "LOPROC+%x", shdr[i].sh_type - SHT_LOPROC);
	  else if (shdr[i].sh_type >= SHT_LOOS && shdr[i].sh_type <= SHT_HIOS)
	    sprintf (buf, "LOOS+%x", shdr[i].sh_type - SHT_LOOS);
	  else if (shdr[i].sh_type >= SHT_LOUSER && shdr[i].sh_type <= SHT_HIUSER)
	    sprintf (buf, "LOUSER+%x", shdr[i].sh_type - SHT_LOUSER);
	  else
	    sprintf (buf, "Unknown: %x", shdr[i].sh_type);
	  p = buf;
	}

      printf ("  [%2d] %-17.17s %-15.15s ", i,
	      strptr (dso, ehdr->e_shstrndx, shdr[i].sh_name), p);

      q = buf;
      shf = shdr[i].sh_flags;
      while (shf)
	{
	  flag = shf & -shf;
	  shf &= ~flag;
	  switch (flag)
	    {
	    case SHF_WRITE:		*q++ = 'W'; break;
	    case SHF_ALLOC:		*q++ = 'A'; break;
	    case SHF_EXECINSTR:		*q++ = 'X'; break;
	    case SHF_MERGE:		*q++ = 'M'; break;
	    case SHF_STRINGS:		*q++ = 'S'; break;
	    case SHF_INFO_LINK:		*q++ = 'I'; break;
	    case SHF_LINK_ORDER:	*q++ = 'L'; break;
	    case SHF_OS_NONCONFORMING:	*q++ = 'O'; break;
	    case SHF_TLS:		*q++ = 'T'; break;
	    default:
	      if (flag & SHF_MASKOS)
		*q++ = 'o', shf &= ~SHF_MASKOS;
	      else if (flag & SHF_MASKPROC)
		*q++ = 'p', shf &= ~SHF_MASKPROC;
	      else
		*q++ = 'x';
	      break;
	    }
	}
      *q = '\0';
      if (elf64)
	printf (" %16.16llx %6.6llx %6.6llx %2.2lx %3s %2ld %3lx %2ld\n",
		(long long) shdr[i].sh_addr, (long long) shdr[i].sh_offset,
		(long long) shdr[i].sh_size, (long) shdr[i].sh_entsize,
		buf, (long) shdr[i].sh_link, (long) shdr[i].sh_info,
		(long) shdr[i].sh_addralign);
      else
	printf (" %8.8lx %6.6lx %6.6lx %2.2lx %3s %2ld %3lx %2ld\n",
		(long) shdr[i].sh_addr, (long) shdr[i].sh_offset,
		(long) shdr[i].sh_size, (long) shdr[i].sh_entsize,
		buf, (long) shdr[i].sh_link, (long) shdr[i].sh_info,
		(long) shdr[i].sh_addralign);
    }
}
