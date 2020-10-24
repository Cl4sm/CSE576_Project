void
event_add(struct event *ev, void *data)
{
  dlink_node *node;

  event_delete(ev);

  ev->data = data;
  ev->next = CurrentTime + ev->when;
  ev->enabled = 1;

  DLINK_FOREACH(node, events.head)
  {
    struct event *e = node->data;

    if (e->next > ev->next)
    {
      dlinkAddBefore(node, ev, &ev->node, &events);
      return;
    }
  }

  dlinkAddTail(ev, &ev->node, &events);
}
