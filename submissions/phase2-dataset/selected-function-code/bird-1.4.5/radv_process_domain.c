int
radv_process_domain(struct radv_dnssl_config *cf)
{
  /* Format of domain in search list is <size> <label> <size> <label> ... 0 */

  char *dom = cf->domain;
  char *dom_end = dom; /* Just to  */
  u8 *dlen_save = &cf->dlen_first;
  int len;

  while (dom_end)
  {
    dom_end = strchr(dom, '.');
    len = dom_end ? (dom_end - dom) : strlen(dom);

    if (len < 1 || len > 63)
      return -1;

    *dlen_save = len;
    dlen_save = (u8 *) dom_end;

    dom += len + 1;
  }

  len = dom - cf->domain;
  if (len > 254)
    return -1;

  cf->dlen_all = len;

  return 0;
}
