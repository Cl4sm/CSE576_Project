static struct mpnh *
merge_nexthops(struct proto_ospf *po, struct mpnh *s1, struct mpnh *s2, int r1, int r2)
{
  struct mpnh *root = NULL;
  struct mpnh **n = &root;
  int count = po->ecmp;

  /*
   * r1, r2 signalize whether we can reuse nexthops from s1, s2.
   * New nexthops (s2, new) can be reused if they are not inherited
   * from the parent (i.e. it is allocated in calc_next_hop()).
   * Current nexthops (s1, en->nhs) can be reused if they weren't
   * inherited in previous steps (that is stored in nhs_reuse,
   * i.e. created by merging or allocalted in calc_next_hop()).
   *
   * Generally, a node first inherits shared nexthops from its
   * parent and later possibly gets reusable copy during merging.
   */

  while ((s1 || s2) && count--)
  {
    int cmp = cmp_nhs(s1, s2);
    if (cmp < 0)
    {
      *n = r1 ? s1 : copy_nexthop(po, s1);
      s1 = s1->next;
    }
    else if (cmp > 0)
    {
      *n = r2 ? s2 : copy_nexthop(po, s2);
      s2 = s2->next;
    }
    else
    {
      *n = r1 ? s1 : (r2 ? s2 : copy_nexthop(po, s1));
      s1 = s1->next;
      s2 = s2->next;
    }
    n = &((*n)->next);
  }
  *n = NULL;

  return root;
}
