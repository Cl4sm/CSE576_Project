static_copy_routes(list *dlst, list *slst)
{
  struct static_route *dr, *sr;

  init_list(dlst);
  WALK_LIST(sr, *slst)
    {
      /* copy one route */
      dr = cfg_alloc(sizeof(struct static_route));
      memcpy(dr, sr, sizeof(struct static_route));

      /* This fn is supposed to be called on fresh src routes, which have 'live'
	 fields (like .chain, .neigh or .installed) zero, so no need to zero them */

      /* We need to copy multipath chain, because there are backptrs in 'if_name' */
      if (dr->dest == RTD_MULTIPATH)
	{
	  struct static_route *md, *ms, **mp_last;

	  mp_last = &(dr->mp_next);
	  for (ms = sr->mp_next; ms; ms = ms->mp_next)
	    {
	      md = cfg_alloc(sizeof(struct static_route));
	      memcpy(md, ms, sizeof(struct static_route));
	      md->if_name = (void *) dr; /* really */

	      *mp_last = md;
	      mp_last = &(md->mp_next);
	    }
	  *mp_last = NULL;
	}

      add_tail(dlst, (node *) dr);
    }
}
