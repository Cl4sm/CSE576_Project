static int
x86_64_adjust_dyn (DSO *dso, int n, GElf_Dyn *dyn, GElf_Addr start,
		   GElf_Addr adjust)
{
  if (dyn->d_tag == DT_PLTGOT)
    {
      int sec = addr_to_sec (dso, dyn->d_un.d_ptr);
      Elf64_Addr data;

      if (sec == -1)
	return 0;

      data = read_ule64 (dso, dyn->d_un.d_ptr);
      /* If .got.plt[0] points to _DYNAMIC, it needs to be adjusted.  */
      if (data == dso->shdr[n].sh_addr && data >= start)
	write_le64 (dso, dyn->d_un.d_ptr, data + adjust);

      data = read_ule64 (dso, dyn->d_un.d_ptr + 8);
      /* If .got.plt[1] points to .plt + 0x16, it needs to be adjusted.  */
      if (data && data >= start)
	{
	  int i;

	  for (i = 1; i < dso->ehdr.e_shnum; i++)
	    if (data == dso->shdr[i].sh_addr + 0x16
		&& dso->shdr[i].sh_type == SHT_PROGBITS
		&& strcmp (strptr (dso, dso->ehdr.e_shstrndx,
					dso->shdr[i].sh_name), ".plt") == 0)
	      {
		write_le64 (dso, dyn->d_un.d_ptr + 8, data + adjust);
		break;
	      }
	}
    }
  return 0;
}
