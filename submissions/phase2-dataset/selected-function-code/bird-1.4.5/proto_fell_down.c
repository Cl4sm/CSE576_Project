proto_fell_down(struct proto *p)
{
  DBG("Protocol %s down\n", p->name);

  u32 all_routes = p->stats.imp_routes + p->stats.filt_routes;
  if (all_routes != 0)
    log(L_ERR "Protocol %s is down but still has %d routes", p->name, all_routes);

  bzero(&p->stats, sizeof(struct proto_stats));
  proto_free_ahooks(p);

  if (! p->proto->multitable)
    rt_unlock_table(p->table);

  if (p->proto->cleanup)
    p->proto->cleanup(p);

  proto_rethink_goal(p);
}
