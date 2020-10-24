bfd_get_iface(struct bfd_proto *p, ip_addr local, struct iface *iface)
{
  struct bfd_iface *ifa;

  WALK_LIST(ifa, p->iface_list)
    if (ipa_equal(ifa->local, local) && (ifa->iface == iface))
      return ifa->uc++, ifa;

  struct bfd_config *cf = (struct bfd_config *) (p->p.cf);
  struct bfd_iface_config *ic = bfd_find_iface_config(cf, iface);

  ifa = mb_allocz(p->tpool, sizeof(struct bfd_iface));
  ifa->local = local;
  ifa->iface = iface;
  ifa->cf = ic;
  ifa->bfd = p;

  ifa->sk = bfd_open_tx_sk(p, local, iface);
  ifa->uc = 1;

  add_tail(&p->iface_list, &ifa->n);

  return ifa;
}
