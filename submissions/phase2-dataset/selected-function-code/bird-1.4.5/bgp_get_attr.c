int
bgp_get_attr(eattr *a, byte *buf, int buflen)
{
  unsigned int i = EA_ID(a->id);
  struct attr_desc *d;
  int len;

  if (ATTR_KNOWN(i))
    {
      d = &bgp_attr_table[i];
      len = bsprintf(buf, "%s", d->name);
      buf += len;
      if (d->format)
	{
	  *buf++ = ':';
	  *buf++ = ' ';
	  d->format(a, buf, buflen - len - 2);
	  return GA_FULL;
	}
      return GA_NAME;
    }
  bsprintf(buf, "%02x%s", i, (a->flags & BAF_TRANSITIVE) ? " [t]" : "");
  return GA_NAME;
}
