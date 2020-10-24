bgp_start_locked(struct object_lock *lock)
{
  struct bgp_proto *p = lock->data;
  struct bgp_config *cf = p->cf;

  if (p->p.proto_state != PS_START)
    {
      DBG("BGP: Got lock in different state %d\n", p->p.proto_state);
      return;
    }

  DBG("BGP: Got lock\n");

  if (cf->multihop)
    {
      /* Multi-hop sessions do not use neighbor entries */
      bgp_initiate(p);
      return;
    }

  p->neigh = neigh_find2(&p->p, &cf->remote_ip, cf->iface, NEF_STICKY);
  if (!p->neigh || (p->neigh->scope == SCOPE_HOST))
    {
      log(L_ERR "%s: Invalid remote address %I%J", p->p.name, cf->remote_ip, cf->iface);
      /* As we do not start yet, we can just disable protocol */
      p->p.disabled = 1;
      bgp_store_error(p, NULL, BE_MISC, BEM_INVALID_NEXT_HOP);
      proto_notify_state(&p->p, PS_DOWN);
      return;
    }
  
  if (p->neigh->scope > 0)
    bgp_start_neighbor(p);
  else
    BGP_TRACE(D_EVENTS, "Waiting for %I%J to become my neighbor", cf->remote_ip, cf->iface);
}
