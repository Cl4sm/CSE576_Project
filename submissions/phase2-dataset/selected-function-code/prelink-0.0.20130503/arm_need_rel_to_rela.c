arm_need_rel_to_rela (DSO *dso, int first, int last)
{
  Elf_Data *data;
  Elf_Scn *scn;
  Elf32_Rel *rel, *relend;
  unsigned int val;

  while (first <= last)
    {
      data = NULL;
      scn = dso->scn[first++];
      while ((data = elf_getdata (scn, data)) != NULL)
	{
	  rel = (Elf32_Rel *) data->d_buf;
	  relend = rel + data->d_size / sizeof (Elf32_Rel);
	  for (; rel < relend; rel++)
	    switch (ELF32_R_TYPE (rel->r_info))
	      {
	      case R_ARM_ABS32:
		val = read_une32 (dso, rel->r_offset);
		/* R_ARM_ABS32 with addend 0 can be converted
		   to R_ARM_GLOB_DAT and we don't have to convert
		   to RELA because of that.  */
		if (val == 0)
		  break;
		/* FALLTHROUGH */
	      case R_ARM_PC24:
		return 1;
	      case R_ARM_TLS_TPOFF32:
		/* In shared libraries TPOFF is changed always into
		   conflicts, for executables we need to preserve
		   original addend.  */
		if (dso->ehdr.e_type == ET_EXEC)
		  return 1;

		break;
	      }
	}
    }
  return 0;
}
