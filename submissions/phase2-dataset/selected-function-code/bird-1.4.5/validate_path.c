validate_path(struct bgp_proto *p, int as_path, int bs, byte *idata, unsigned int *ilength)
{
  int res = 0;
  u8 *a, *dst;
  int len, plen, copy;

  dst = a = idata;
  len = *ilength;

  while (len)
    {
      if (len < 2)
	return -1;

      plen = 2 + bs * a[1];
      if (len < plen)
	return -1;

      switch (a[0])
	{
	case AS_PATH_SET:
	  copy = 1;
	  res++;
	  break;

	case AS_PATH_SEQUENCE:
	  copy = 1;
	  res += a[1];
	  break;

	case AS_PATH_CONFED_SEQUENCE:
	case AS_PATH_CONFED_SET:
	  if (as_path && path_segment_contains(a, bs, p->remote_as))
	    {
	      log(L_WARN "%s: AS_CONFED_* segment with peer ASN found, misconfigured confederation?", p->p.name);
	      return -1;
	    }

	  log(L_WARN "%s: %s_PATH attribute contains AS_CONFED_* segment, skipping segment",
	      p->p.name, as_path ? "AS" : "AS4");
	  copy = 0;
	  break;

	default:
	  return -1;
	}

      if (copy)
	{
	  if (dst != a)
	    memmove(dst, a, plen);
	  dst += plen;
	}

      len -= plen;
      a += plen;
    }

  *ilength = dst - idata;
  return res;
}
