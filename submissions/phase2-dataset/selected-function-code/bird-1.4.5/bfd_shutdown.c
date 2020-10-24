bfd_shutdown(struct proto *P)
{
  struct bfd_proto *p = (struct bfd_proto *) P;
  struct bfd_config *cf = (struct bfd_config *) (P->cf);

  rem_node(&p->bfd_node);

  birdloop_stop(p->loop);

  struct bfd_neighbor *n;
  WALK_LIST(n, cf->neigh_list)
    bfd_stop_neighbor(p, n);

  bfd_drop_requests(p);

  /* FIXME: This is hack */
  birdloop_enter(p->loop);
  rfree(p->tpool);
  birdloop_leave(p->loop);

  birdloop_free(p->loop);

  return PS_DOWN;
}
