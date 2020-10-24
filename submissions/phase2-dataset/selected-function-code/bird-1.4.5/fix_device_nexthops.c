static struct mpnh *
fix_device_nexthops(struct proto_ospf *po, const struct mpnh *n, ip_addr gw)
{
  struct mpnh *root1 = NULL;
  struct mpnh *root2 = NULL;
  struct mpnh **nn1 = &root1;
  struct mpnh **nn2 = &root2;

  /* This is a bit tricky. We cannot just copy the list and update n->gw,
     because the list should stay sorted, so we create two lists, one with new
     gateways and one with old ones, and then merge them. */

  for (; n; n = n->next)
  {
    struct mpnh *nn = new_nexthop(po, ipa_zero(n->gw) ? gw : n->gw, n->iface, n->weight);

    if (ipa_zero(n->gw))
    {
      *nn1 = nn;
      nn1 = &(nn->next);
    }
    else
    {
      *nn2 = nn;
      nn2 = &(nn->next);
    }
  }

  return merge_nexthops(po, root1, root2, 1, 1);
}
