static struct radv_prefix_config *
radv_prefix_match(struct radv_iface *ifa, struct ifa *a)
{
  struct proto *p = &ifa->ra->p;
  struct radv_config *cf = (struct radv_config *) (p->cf);
  struct radv_prefix_config *pc;

  if (a->scope <= SCOPE_LINK)
    return NULL;

  WALK_LIST(pc, ifa->cf->pref_list)
    if ((a->pxlen >= pc->pxlen) && ipa_in_net(a->prefix, pc->prefix, pc->pxlen))
      return pc;

  WALK_LIST(pc, cf->pref_list)
    if ((a->pxlen >= pc->pxlen) && ipa_in_net(a->prefix, pc->prefix, pc->pxlen))
      return pc;

  return &default_prefix;
}
