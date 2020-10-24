radv_prepare_rdnss(struct radv_iface *ifa, list *rdnss_list, char **buf, char *bufend)
{
  struct radv_rdnss_config *rcf = HEAD(*rdnss_list);

  while(NODE_VALID(rcf))
  {
    struct radv_rdnss_config *rcf_base = rcf;
    struct radv_opt_rdnss *op = (void *) *buf;
    int max_i = (bufend - *buf - sizeof(struct radv_opt_rdnss)) / sizeof(ip_addr);
    int i = 0;

    if (max_i < 1)
      goto too_much;

    op->type = OPT_RDNSS;
    op->reserved = 0;

    if (rcf->lifetime_mult)
      op->lifetime = htonl(rcf->lifetime_mult * ifa->cf->max_ra_int);
    else
      op->lifetime = htonl(rcf->lifetime);

    while(NODE_VALID(rcf) && 
	  (rcf->lifetime == rcf_base->lifetime) &&
	  (rcf->lifetime_mult == rcf_base->lifetime_mult))
      {
	if (i >= max_i)
	  goto too_much;

	op->servers[i] = rcf->server;
	ipa_hton(op->servers[i]);
	i++;

	rcf = NODE_NEXT(rcf);
      }
  
    op->length = 1+2*i;
    *buf += 8 * op->length;
  }

  return 0;

 too_much:
  log(L_WARN "%s: Too many RA options on interface %s",
      ifa->ra->p.name, ifa->iface->name);
  return -1;
}
