cmd_show_symbols(struct sym_show_data *sd)
{
  int pos = 0;
  struct symbol *sym = sd->sym;

  if (sym)
    cli_msg(1010, "%-8s\t%s", sym->name, cf_symbol_class_name(sym));
  else
    {
      while (sym = cf_walk_symbols(config, sym, &pos))
	{
	  if (sd->type && (sym->class != sd->type))
	    continue;

	  cli_msg(-1010, "%-8s\t%s", sym->name, cf_symbol_class_name(sym));
	}
      cli_msg(0, "");
    }
}
