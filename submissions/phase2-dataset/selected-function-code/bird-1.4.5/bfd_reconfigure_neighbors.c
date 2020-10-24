static void
bfd_reconfigure_neighbors(struct bfd_proto *p, struct bfd_config *new)
{
  struct bfd_config *old = (struct bfd_config *) (p->p.cf);
  struct bfd_neighbor *on, *nn;

  WALK_LIST(on, old->neigh_list)
  {
    WALK_LIST(nn, new->neigh_list)
      if (bfd_same_neighbor(nn, on))
      {
	nn->neigh = on->neigh;
	if (nn->neigh)
	  nn->neigh->data = nn;

	nn->req = on->req;
	nn->active = 1;
	return;
      }

    bfd_stop_neighbor(p, on);
  }

  WALK_LIST(nn, new->neigh_list)
    if (!nn->active)
      bfd_start_neighbor(p, nn);
}
