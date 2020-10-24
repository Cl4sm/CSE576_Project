prelink_save_cache_check (struct prelink_entry *ent)
{
  int i;

  for (i = 0; i < ent->ndepends; ++i)
    switch (ent->depends[i]->type)
      {
      case ET_DYN:
	if (ent->depends[i]->done < 2
	    || (quick && (ent->depends[i]->flags & PCF_PRELINKED)))
	  return 1;
	break;
      case ET_CACHE_DYN:
	if (prelink_save_cache_check (ent->depends[i]))
	  return 1;
	break;
      case ET_UNPRELINKABLE:
	if (ent->type != ET_UNPRELINKABLE)
	  return 1;
	if (prelink_save_cache_check (ent->depends[i]))
	  return 1;
	break;
      default:
	return 1;
      }

  return 0;
}
