mips_adjust_reloc (DSO *dso, GElf_Addr r_offset, GElf_Xword r_info,
		   GElf_Addr start, GElf_Addr adjust, GElf_Rela *rela)
{
  GElf_Addr value;
  GElf_Word r_sym;

  if (GELF_R_TYPE (r_info) == R_MIPS_REL32)
    {
      r_sym = GELF_R_SYM (r_info);
      if (r_sym < dso->info_DT_MIPS_GOTSYM)
	{
	  /* glibc's dynamic linker adds the symbol's st_value and the
	     base address to the addend.  It therefore treats all symbols
	     as being relative, even if they would normally be considered
	     absolute.  For example, the special null symbol should always
	     have the value zero, even when the base address is nonzero,
	     but R_MIPS_REL32 relocations against the null symbol must
	     nevertheles be adjusted as if that symbol were relative.
	     The same would apply to SHN_ABS symbols too.

	     Thus the result of the relocation calculation must always
	     be adjusted by ADJUST.  (We do not need to check START because
	     every adjustment requested by the caller will affect all
	     legitimate local relocation values.)  This means that we
	     should add ADJUST to the addend if and only if the symbol's
	     value is not being adjusted.

	     In general, we can only check whether a symbol's value is
	     being adjusted by reading its entry in the dynamic symbol
	     table and then querying adjust_symbol_p.  However, this
	     generality is fortunately not needed.  Modern versions
	     of binutils will never generate R_MIPS_REL32 relocations
	     against symbols in the range [1, DT_MIPS_GOTSYM), so we
	     only need to handle relocations against the null symbol.  */
	  if (r_sym != 0)
	    {
	      error (0, 0, "%s: The prelinker does not support R_MIPS_REL32"
		     " relocs against local symbols", dso->filename);
	      return 1;
	    }
	  value = mips_read_addend (dso, r_offset, rela);
	  mips_write_addend (dso, r_offset, rela, value + adjust);
	}
    }
  return 0;
}
