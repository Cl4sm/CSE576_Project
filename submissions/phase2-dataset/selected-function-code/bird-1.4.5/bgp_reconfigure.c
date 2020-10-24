static int
bgp_reconfigure(struct proto *P, struct proto_config *C)
{
  struct bgp_config *new = (struct bgp_config *) C;
  struct bgp_proto *p = (struct bgp_proto *) P;
  struct bgp_config *old = p->cf;

  if (proto_get_router_id(C) != p->local_id)
    return 0;

  int same = !memcmp(((byte *) old) + sizeof(struct proto_config),
		     ((byte *) new) + sizeof(struct proto_config),
		     // password item is last and must be checked separately
		     OFFSETOF(struct bgp_config, password) - sizeof(struct proto_config))
    && ((!old->password && !new->password)
	|| (old->password && new->password && !strcmp(old->password, new->password)))
    && (get_igp_table(old) == get_igp_table(new));

  if (same && (p->start_state > BSS_PREPARE))
    bgp_update_bfd(p, new->bfd);

  /* We should update our copy of configuration ptr as old configuration will be freed */
  if (same)
    p->cf = new;

  return same;
}
