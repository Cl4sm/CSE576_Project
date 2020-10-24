ia64_create_opd (struct prelink_info *info, int first, int last, int plt)
{
  Elf_Data *d;
  Elf_Scn *scn;
  Elf64_Rela *rela, *relaend;
  DSO *dso = info->dso;
  int sec;

  if (opd_init (info))
    return -1;

  if (plt)
    info->ent->opd->plt_start = dso->shdr[dso->shdr[plt].sh_info].sh_addr;
  else
    info->ent->opd->plt_start = dso->shdr[dso->dynamic].sh_addr;
  sec = first;
  while (sec <= last)
    {
      d = NULL;
      scn = dso->scn[sec++];
      while ((d = elf_getdata (scn, d)) != NULL)
	{
	  rela = (Elf64_Rela *) d->d_buf;
	  relaend = rela + d->d_size / sizeof (Elf64_Rela);
	  for (; rela < relaend; rela++)
	    if ((ELF64_R_TYPE (rela->r_info) & ~3) == R_IA64_FPTR32MSB
		&& opd_add (info, ELF64_R_SYM (rela->r_info),
			    R_IA64_FPTR64LSB))
	      return -1;
	}
    }

  sec = first;
  while (sec)
    {
      d = NULL;
      if (sec == plt)
	break;
      scn = dso->scn[sec++];
      if (sec == last + 1)
	sec = plt;
      while ((d = elf_getdata (scn, d)) != NULL)
	{
	  rela = (Elf64_Rela *) d->d_buf;
	  relaend = rela + d->d_size / sizeof (Elf64_Rela);
	  for (; rela < relaend; rela++)
	    if ((ELF64_R_TYPE (rela->r_info) & ~1) == R_IA64_IPLTMSB)
	      opd_note_plt (info, ELF64_R_SYM (rela->r_info), R_IA64_IPLTLSB,
			    rela->r_offset);
	}
    }

  return opd_size (info, 16);
}
