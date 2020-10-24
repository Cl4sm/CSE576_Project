mips_need_rel_to_rela (DSO *dso, int first, int last)
{
  Elf_Data *data;
  Elf_Scn *scn;
  Elf32_Rel *rel, *relend;
  int n;

  for (n = first; n <= last; n++)
    {
      data = NULL;
      scn = dso->scn[n];
      while ((data = elf_getdata (scn, data)) != NULL)
	{
	  rel = (Elf32_Rel *) data->d_buf;
	  relend = rel + data->d_size / sizeof (Elf32_Rel);
	  for (; rel < relend; rel++)
	    switch (ELF32_R_TYPE (rel->r_info))
	      {
	      case R_MIPS_NONE:
		break;

	      case R_MIPS_REL32:
		/* The SVR4 definition was designed to allow exactly the
		   sort of prelinking we want to do here, in combination
		   with Quickstart.  Unfortunately, glibc's definition
		   makes it impossible for relocations against anything
		   other than the null symbol.  We get around this for
		   zero addends by using a R_MIPS_GLOB_DAT relocation
		   instead, where R_MIPS_GLOB_DAT is a GNU extension
		   added specifically for this purpose.  */
		if (ELF32_R_SYM (rel->r_info) != 0
		    && (ELF32_R_SYM (rel->r_info) < dso->info_DT_MIPS_GOTSYM
			|| read_une32 (dso, rel->r_offset) != 0))
		  return 1;
		break;

	      case R_MIPS_GLOB_DAT:
		/* This relocation has no addend.  */
		break;

	      case R_MIPS_TLS_DTPMOD32:
		/* The relocation will be resolved using a conflict.  */
		break;

	      case R_MIPS_TLS_DTPREL32:
		/* We can prelink these fields, and the addend is relative
		   to the symbol value.  A RELA entry is needed.  */
		return 1;

	      case R_MIPS_TLS_TPREL32:
		/* Relocations in shared libraries will be resolved by a
		   conflict.  Relocations in executables will not, and the
		   addend is relative to the symbol value.  */
		if (dso->ehdr.e_type == ET_EXEC)
		  return 1;
		break;

	      default:
		error (0, 0, "%s: Unknown MIPS relocation type %d",
		       dso->filename, (int) GELF_R_TYPE (rel->r_info));
		return 1;
	      }
	}
    }
  return 0;
}
