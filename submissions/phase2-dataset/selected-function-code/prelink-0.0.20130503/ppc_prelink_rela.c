static int
ppc_prelink_rela (struct prelink_info *info, GElf_Rela *rela,
		  GElf_Addr relaaddr)
{
  DSO *dso = info->dso;
  GElf_Addr value;

  if (GELF_R_TYPE (rela->r_info) == R_PPC_NONE
      || GELF_R_TYPE (rela->r_info) == R_PPC_IRELATIVE)
    return 0;
  else if (GELF_R_TYPE (rela->r_info) == R_PPC_RELATIVE)
    {
      write_be32 (dso, rela->r_offset, rela->r_addend);
      return 0;
    }
  value = info->resolve (info, GELF_R_SYM (rela->r_info),
			 GELF_R_TYPE (rela->r_info));
  value += rela->r_addend;
  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_PPC_GLOB_DAT:
    case R_PPC_ADDR32:
    case R_PPC_UADDR32:
      write_be32 (dso, rela->r_offset, value);
      break;
    case R_PPC_DTPREL32:
      write_be32 (dso, rela->r_offset, value - 0x8000);
      break;
    case R_PPC_JMP_SLOT:
      if (dynamic_info_is_set (dso, DT_PPC_GOT_BIT))
	write_be32 (dso, rela->r_offset, value);
      else
	ppc_fixup_plt (dso, rela, value);
      break;
    case R_PPC_ADDR16:
    case R_PPC_UADDR16:
    case R_PPC_ADDR16_LO:
      write_be16 (dso, rela->r_offset, value);
      break;
    case R_PPC_DTPREL16:
    case R_PPC_DTPREL16_LO:
      write_be16 (dso, rela->r_offset, value - 0x8000);
      break;
    case R_PPC_ADDR16_HI:
    case R_PPC_DTPREL16_HA:
      write_be16 (dso, rela->r_offset, value >> 16);
      break;
    case R_PPC_DTPREL16_HI:
      write_be16 (dso, rela->r_offset, (value - 0x8000) >> 16);
      break;
    case R_PPC_ADDR16_HA:
      write_be16 (dso, rela->r_offset, (value + 0x8000) >> 16);
      break;
    case R_PPC_ADDR24:
      write_be32 (dso, rela->r_offset,
		  (value & 0x03fffffc)
		  | (read_ube32 (dso, rela->r_offset) & 0xfc000003));
      break;
    case R_PPC_ADDR14:
      write_be32 (dso, rela->r_offset,
		  (value & 0xfffc)
		  | (read_ube32 (dso, rela->r_offset) & 0xffff0003));
      break;
    case R_PPC_ADDR14_BRTAKEN:
    case R_PPC_ADDR14_BRNTAKEN:
      write_be32 (dso, rela->r_offset,
		  (value & 0xfffc)
		  | (read_ube32 (dso, rela->r_offset) & 0xffdf0003)
		  | ((((GELF_R_TYPE (rela->r_info) == R_PPC_ADDR14_BRTAKEN)
		       << 21)
		      ^ (value >> 10)) & 0x00200000));
      break;
    case R_PPC_REL24:
      write_be32 (dso, rela->r_offset,
		  ((value - rela->r_offset) & 0x03fffffc)
		  | (read_ube32 (dso, rela->r_offset) & 0xfc000003));
      break;
    case R_PPC_REL32:
      write_be32 (dso, rela->r_offset, value - rela->r_offset);
      break;
    /* DTPMOD32 and TPREL* is impossible to predict in shared libraries
       unless prelink sets the rules.  */
    case R_PPC_DTPMOD32:
      if (dso->ehdr.e_type == ET_EXEC)
	{
	  error (0, 0, "%s: R_PPC_DTPMOD32 reloc in executable?",
		 dso->filename);
	  return 1;
	}
      break;
    case R_PPC_TPREL32:
    case R_PPC_TPREL16:
    case R_PPC_TPREL16_LO:
    case R_PPC_TPREL16_HI:
    case R_PPC_TPREL16_HA:
      if (dso->ehdr.e_type == ET_EXEC && info->resolvetls)
	{
	  value += info->resolvetls->offset - 0x7000;
	  switch (GELF_R_TYPE (rela->r_info))
	    {
	    case R_PPC_TPREL32:
	      write_be32 (dso, rela->r_offset, value);
	      break;
	    case R_PPC_TPREL16:
	    case R_PPC_TPREL16_LO:
	      write_be16 (dso, rela->r_offset, value);
	      break;
	    case R_PPC_TPREL16_HI:
	      write_be16 (dso, rela->r_offset, value >> 16);
	      break;
	    case R_PPC_TPREL16_HA:
	      write_be16 (dso, rela->r_offset, (value + 0x8000) >> 16);
	      break;
	    }
	}
      break;
    case R_PPC_COPY:
      if (dso->ehdr.e_type == ET_EXEC)
	/* COPY relocs are handled specially in generic code.  */
	return 0;
      error (0, 0, "%s: R_PPC_COPY reloc in shared library?", dso->filename);
      return 1;
    default:
      error (0, 0, "%s: Unknown ppc relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rela->r_info));
      return 1;
    }
  return 0;
}
