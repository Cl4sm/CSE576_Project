struct announce_hook *
proto_add_announce_hook(struct proto *p, struct rtable *t, struct proto_stats *stats)
{
  struct announce_hook *h;

  DBG("Connecting protocol %s to table %s\n", p->name, t->name);
  PD(p, "Connected to table %s", t->name);

  h = mb_allocz(rt_table_pool, sizeof(struct announce_hook));
  h->table = t;
  h->proto = p;
  h->stats = stats;

  h->next = p->ahooks;
  p->ahooks = h;

  if (p->rt_notify && (p->export_state != ES_DOWN))
    add_tail(&t->hooks, &h->n);
  return h;
}
