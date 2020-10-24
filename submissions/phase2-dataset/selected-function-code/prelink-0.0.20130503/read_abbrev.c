read_abbrev (DSO *dso, unsigned char *ptr)
{
  htab_t h = htab_try_create (50, abbrev_hash, abbrev_eq, abbrev_del);
  unsigned int attr, form;
  struct abbrev_tag *t;
  int size;
  void **slot;

  if (h == NULL)
    {
no_memory:
      error (0, ENOMEM, "%s: Could not read .debug_abbrev", dso->filename);
      if (h)
	htab_delete (h);
      return NULL;
    }

  while ((attr = read_uleb128 (ptr)) != 0)
    {
      size = 10;
      t = malloc (sizeof (*t) + size * sizeof (struct abbrev_attr));
      if (t == NULL)
	goto no_memory;
      t->entry = attr;
      t->nattr = 0;
      slot = htab_find_slot (h, t, INSERT);
      if (slot == NULL)
	{
	  free (t);
	  goto no_memory;
	}
      if (*slot != NULL)
	{
	  error (0, 0, "%s: Duplicate DWARF abbreviation %d", dso->filename,
		 t->entry);
	  free (t);
	  htab_delete (h);
	  return NULL;
	}
      t->tag = read_uleb128 (ptr);
      ++ptr; /* skip children flag.  */
      while ((attr = read_uleb128 (ptr)) != 0)
	{
	  if (t->nattr == size)
	    {
	      size += 10;
	      t = realloc (t, sizeof (*t) + size * sizeof (struct abbrev_attr));
	      if (t == NULL)
		goto no_memory;
	    }
	  form = read_uleb128 (ptr);
	  if (form == 2
	      || (form > DW_FORM_flag_present
		  && form != DW_FORM_ref_sig8
		  && form != DW_FORM_GNU_ref_alt
		  && form != DW_FORM_GNU_strp_alt))
	    {
	      error (0, 0, "%s: Unknown DWARF DW_FORM_%d", dso->filename, form);
	      htab_delete (h);
	      return NULL;
	    }

	  t->attr[t->nattr].attr = attr;
	  t->attr[t->nattr++].form = form;
	}
      if (read_uleb128 (ptr) != 0)
	{
	  error (0, 0, "%s: DWARF abbreviation does not end with 2 zeros",
		 dso->filename);
	  htab_delete (h);
	  return NULL;
	}
      *slot = t;
    }

  return h;
}
