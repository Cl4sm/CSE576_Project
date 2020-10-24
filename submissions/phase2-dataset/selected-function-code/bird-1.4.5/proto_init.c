static struct proto *
proto_init(struct proto_config *c)
{
  struct protocol *p = c->protocol;
  struct proto *q = p->init(c);

  q->proto_state = PS_DOWN;
  q->core_state = FS_HUNGRY;
  q->export_state = ES_DOWN;
  q->last_state_change = now;

  add_tail(&initial_proto_list, &q->n);

  if (p == &proto_unix_iface)
    initial_device_proto = q;

  add_tail(&proto_list, &q->glob_node);
  PD(q, "Initializing%s", q->disabled ? " [disabled]" : "");
  return q;
}
