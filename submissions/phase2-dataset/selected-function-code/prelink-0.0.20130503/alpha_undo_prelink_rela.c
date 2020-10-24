static int
alpha_undo_prelink_rela (DSO *dso, GElf_Rela *rela, GElf_Addr relaaddr)
{
  int sec;
  Elf_Scn *scn;
  Elf_Data *data;
  GElf_Sym sym;

  switch (GELF_R_TYPE (rela->r_info))
    {
    case R_ALPHA_NONE:
    case R_ALPHA_RELATIVE:
      break;
    case R_ALPHA_JMP_SLOT:
      relaaddr -= dso->info[DT_JMPREL];
      relaaddr /= sizeof (Elf64_Rela);
      relaaddr *= 12;
      relaaddr += dso->info[DT_PLTGOT] + 32;
      /* br at,.plt  */
      write_le32 (dso, relaaddr,
		  0xc39fffff - (relaaddr - dso->info[DT_PLTGOT]) / 4);
      write_le64 (dso, relaaddr + 4, 0);
      write_le64 (dso, rela->r_offset, relaaddr);
      break;
    case R_ALPHA_GLOB_DAT:
      /* This is ugly.  Linker doesn't clear memory at r_offset of GLOB_DAT
	 reloc, but instead puts in sym.st_value + addend.  */
      sec = addr_to_sec (dso, relaaddr);
      assert (sec != -1);
      sec = dso->shdr[sec].sh_link;
      assert (sec > 0 && sec < dso->ehdr.e_shnum);
      scn = dso->scn[sec];
      data = elf_getdata (scn, NULL);
      assert (data != NULL && elf_getdata (scn, data) == NULL);
      assert (GELF_R_SYM (rela->r_info)
	      <= dso->shdr[sec].sh_size / sizeof (Elf64_Sym));
      gelfx_getsym (dso->elf, data, GELF_R_SYM (rela->r_info), &sym);
      write_le64 (dso, rela->r_offset, sym.st_value + rela->r_addend);
      break;
    case R_ALPHA_REFQUAD:
    case R_ALPHA_DTPMOD64:
    case R_ALPHA_DTPREL64:
    case R_ALPHA_TPREL64:
      write_le64 (dso, rela->r_offset, 0);
      break;
    default:
      error (0, 0, "%s: Unknown alpha relocation type %d", dso->filename,
	     (int) GELF_R_TYPE (rela->r_info));
      return 1;
    }
  return 0;
}
