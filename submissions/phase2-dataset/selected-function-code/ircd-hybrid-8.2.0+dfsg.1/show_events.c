void
show_events(struct Client *source_p)
{
  const dlink_node *node;

  sendto_one_numeric(source_p, &me, RPL_STATSDEBUG|SND_EXPLICIT,
                     "E :Operation                      Next Execution");
  sendto_one_numeric(source_p, &me, RPL_STATSDEBUG|SND_EXPLICIT,
                     "E :---------------------------------------------");

  DLINK_FOREACH(node, events.head)
  {
    const struct event *ev = node->data;

    sendto_one_numeric(source_p, &me, RPL_STATSDEBUG|SND_EXPLICIT,
                       "E :%-30s %-4d seconds",
                       ev->name,
                       (int)(ev->next - CurrentTime));
  }
}
