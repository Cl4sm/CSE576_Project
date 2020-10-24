ea_dump(ea_list *e)
{
  int i;

  if (!e)
    {
      debug("NONE");
      return;
    }
  while (e)
    {
      debug("[%c%c%c]",
	    (e->flags & EALF_SORTED) ? 'S' : 's',
	    (e->flags & EALF_BISECT) ? 'B' : 'b',
	    (e->flags & EALF_CACHED) ? 'C' : 'c');
      for(i=0; i<e->count; i++)
	{
	  eattr *a = &e->attrs[i];
	  debug(" %02x:%02x.%02x", EA_PROTO(a->id), EA_ID(a->id), a->flags);
	  if (a->type & EAF_TEMP)
	    debug("T");
	  debug("=%c", "?iO?I?P???S?????" [a->type & EAF_TYPE_MASK]);
	  if (a->type & EAF_ORIGINATED)
	    debug("o");
	  if (a->type & EAF_EMBEDDED)
	    debug(":%08x", a->u.data);
	  else
	    {
	      int j, len = a->u.ptr->length;
	      debug("[%d]:", len);
	      for(j=0; j<len; j++)
		debug("%02x", a->u.ptr->data[j]);
	    }
	}
      if (e = e->next)
	debug(" | ");
    }
}
