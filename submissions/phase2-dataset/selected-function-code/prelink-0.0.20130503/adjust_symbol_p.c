adjust_symbol_p (DSO *dso, GElf_Sym *sym)
{
  if (sym->st_shndx == SHN_ABS
      && sym->st_value != 0
      && GELF_ST_TYPE (sym->st_info) <= STT_FUNC)
    /* This is problematic.  How do we find out if
       we should relocate this?  Assume we should.  */
    return 1;

  /* If a MIPS object does not define a symbol, but has a lazy binding
     stub for it, st_value will point to that stub.  Note that unlike
     other targets, these stub addresses never participate in symbol
     lookup; the stubs can only be called by the object that defines them.
     st_values are only used in this way so that the associated GOT entry
     can store a Quickstart value without losing the original stub
     address.  */
  if (dso->ehdr.e_machine == EM_MIPS
      && sym->st_shndx == SHN_UNDEF
      && sym->st_value != 0)
    return 1;

  return (sym->st_shndx > SHN_UNDEF
	  && sym->st_shndx < dso->ehdr.e_shnum
	  && ELF32_ST_TYPE (sym->st_info) != STT_TLS
	  && RELOCATE_SCN (dso->shdr[sym->st_shndx].sh_flags));
}
