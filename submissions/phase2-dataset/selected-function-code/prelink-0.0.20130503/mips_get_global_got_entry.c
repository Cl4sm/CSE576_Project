mips_get_global_got_entry (struct mips_global_got_iterator *ggi)
{
  ggi->sym_index++;
  ggi->got_addr += ggi->entry_size;
  if (ggi->sym_index >= ggi->dso->info_DT_MIPS_SYMTABNO)
    return 0;

  ggi->got_entry = get_data_from_iterator (&ggi->got_iterator,
					   ggi->entry_size);
  if (ggi->got_entry == NULL
      || !get_sym_from_iterator (&ggi->sym_iterator, &ggi->sym))
    {
      error (0, 0, "%s: Malformed global GOT\n", ggi->dso->filename);
      ggi->failed = 1;
      return 0;
    }

  return 1;
}
