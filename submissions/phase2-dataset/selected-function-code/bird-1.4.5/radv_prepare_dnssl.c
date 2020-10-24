static int
radv_prepare_dnssl(struct radv_iface *ifa, list *dnssl_list, char **buf, char *bufend)
{
  struct radv_dnssl_config *dcf = HEAD(*dnssl_list);

  while(NODE_VALID(dcf))
  {
    struct radv_dnssl_config *dcf_base = dcf;
    struct radv_opt_dnssl *op = (void *) *buf;
    int bsize = bufend - *buf - sizeof(struct radv_opt_dnssl);
    int bpos = 0;

    if (bsize < 0)
      goto too_much;

    bsize = bsize & ~7; /* Round down to multiples of 8 */

    op->type = OPT_DNSSL;
    op->reserved = 0;

    if (dcf->lifetime_mult)
      op->lifetime = htonl(dcf->lifetime_mult * ifa->cf->max_ra_int);
    else
      op->lifetime = htonl(dcf->lifetime);

    while(NODE_VALID(dcf) && 
	  (dcf->lifetime == dcf_base->lifetime) &&
	  (dcf->lifetime_mult == dcf_base->lifetime_mult))
      {
	if (bpos + dcf->dlen_all + 1 > bsize)
	  goto too_much;

	op->domain[bpos++] = dcf->dlen_first;
	memcpy(op->domain + bpos, dcf->domain, dcf->dlen_all);
	bpos += dcf->dlen_all;

	dcf = NODE_NEXT(dcf);
      }

    int blen = (bpos + 7) / 8;
    bzero(op->domain + bpos, 8 * blen - bpos);
    op->length = 1 + blen;
    *buf += 8 * op->length;
  }

  return 0;

 too_much:
  log(L_WARN "%s: Too many RA options on interface %s",
      ifa->ra->p.name, ifa->iface->name);
  return -1;
}
