static int
bfd_reconfigure(struct proto *P, struct proto_config *c)
{
  struct bfd_proto *p = (struct bfd_proto *) P;
  // struct bfd_config *old = (struct bfd_config *) (P->cf);
  struct bfd_config *new = (struct bfd_config *) c;
  struct bfd_iface *ifa;

  birdloop_mask_wakeups(p->loop);

  WALK_LIST(ifa, p->iface_list)
    bfd_reconfigure_iface(p, ifa, new);

  HASH_WALK(p->session_hash_id, next_id, s)
  {
    if (s->ifa->changed)
      bfd_reconfigure_session(p, s);
  }
  HASH_WALK_END;

  bfd_reconfigure_neighbors(p, new);

  birdloop_unmask_wakeups(p->loop);

  return 1;
}
