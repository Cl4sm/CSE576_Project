int
as_path_match(struct adata *path, struct f_path_mask *mask)
{
  struct pm_pos pos[2048 + 1];
  int plen = parse_path(path, pos);
  int l, h, i, nh, nl;
  u32 val = 0;

  /* l and h are bound of interval of positions where
     are marked states */

  pos[plen].set = 0;
  pos[plen].mark = 0;

  l = h = 0;
  pos[0].mark = 1;
  
  while (mask)
    {
      /* We remove this mark to not step after pos[plen] */
      pos[plen].mark = 0;

      switch (mask->kind)
	{
	case PM_ASTERISK:
	  for (i = l; i <= plen; i++)
	    pos[i].mark = 1;
	  h = plen;
	  break;

	case PM_ASN:
	  val = mask->val;
	  goto step;
	case PM_ASN_EXPR:
	  val = f_eval_asn((struct f_inst *) mask->val);
	  goto step;
	case PM_QUESTION:
	step:
	  nh = nl = -1;
	  for (i = h; i >= l; i--)
	    if (pos[i].mark)
	      {
		pos[i].mark = 0;
		if ((mask->kind == PM_QUESTION) || pm_match(pos + i, val))
		  pm_mark(pos, i, plen, &nl, &nh);
	      }

	  if (nh < 0)
	    return 0;

	  h = nh;
	  l = nl;
	  break;
	}

      mask = mask->next;
    }

  return pos[plen].mark;
}
