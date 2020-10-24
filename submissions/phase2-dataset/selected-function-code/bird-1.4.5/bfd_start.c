static int
bfd_start(struct proto *P)
{
  struct bfd_proto *p = (struct bfd_proto *) P;
  struct bfd_config *cf = (struct bfd_config *) (P->cf);

  p->loop = birdloop_new();
  p->tpool = rp_new(NULL, "BFD thread root");
  pthread_spin_init(&p->lock, PTHREAD_PROCESS_PRIVATE);

  p->session_slab = sl_new(P->pool, sizeof(struct bfd_session));
  HASH_INIT(p->session_hash_id, P->pool, 8);
  HASH_INIT(p->session_hash_ip, P->pool, 8);

  init_list(&p->iface_list);

  init_list(&p->notify_list);
  bfd_notify_init(p);

  add_tail(&bfd_proto_list, &p->bfd_node);

  birdloop_enter(p->loop);
  p->rx_1 = bfd_open_rx_sk(p, 0);
  p->rx_m = bfd_open_rx_sk(p, 1);
  birdloop_leave(p->loop);

  bfd_take_requests(p);

  struct bfd_neighbor *n;
  WALK_LIST(n, cf->neigh_list)
    bfd_start_neighbor(p, n);

  birdloop_start(p->loop);

  return PS_UP;
}
