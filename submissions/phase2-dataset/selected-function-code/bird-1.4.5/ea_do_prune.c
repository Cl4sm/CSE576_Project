static inline void
ea_do_prune(ea_list *e)
{
  eattr *s, *d, *l, *s0;
  int i = 0;

  /* Discard duplicates and undefs. Do you remember sorting was stable? */
  s = d = e->attrs;
  l = e->attrs + e->count;
  while (s < l)
    {
      s0 = s++;
      while (s < l && s->id == s[-1].id)
	s++;
      /* s0 is the most recent version, s[-1] the oldest one */
      if ((s0->type & EAF_TYPE_MASK) != EAF_TYPE_UNDEF)
	{
	  *d = *s0;
	  d->type = (d->type & ~EAF_ORIGINATED) | (s[-1].type & EAF_ORIGINATED);
	  d++;
	  i++;
	}
    }
  e->count = i;
}
