resolve_dso (struct prelink_info *info, GElf_Word r_sym,
	     int reloc_type)
{
  struct prelink_symbol *s;
  int reloc_class = info->dso->arch->reloc_class (reloc_type);

  for (s = & info->symbols[r_sym]; s; s = s->next)
    if (s->reloc_class == reloc_class)
      break;

  info->resolveent = NULL;
  info->resolvetls = NULL;

  if (s == NULL || s->u.ent == NULL)
    return 0;

  if (reloc_class == RTYPE_CLASS_TLS)
    {
      info->resolvetls = s->u.tls;
      return s->value;
    }

  info->resolveent = s->u.ent;
  return s->u.ent->base + s->value;
}
