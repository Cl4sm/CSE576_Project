proto_relink(struct proto *p)
{
  list *l = NULL;

  switch (p->core_state)
    {
    case FS_HUNGRY:
      l = &inactive_proto_list;
      break;
    case FS_HAPPY:
      l = &active_proto_list;
      break;
    case FS_FLUSHING:
      l = &flush_proto_list;
      break;
    default:
      ASSERT(0);
    }

  rem_node(&p->n);
  add_tail(l, &p->n);
}
