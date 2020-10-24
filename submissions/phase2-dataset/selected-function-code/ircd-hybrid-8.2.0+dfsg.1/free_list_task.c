void
free_list_task(struct Client *source_p)
{
  struct ListTask *lt = source_p->localClient->list_task;
  dlink_node *ptr = NULL, *ptr_next = NULL;

  if ((ptr = dlinkFindDelete(&listing_client_list, source_p)))
    free_dlink_node(ptr);

  DLINK_FOREACH_SAFE(ptr, ptr_next, lt->show_mask.head)
  {
    MyFree(ptr->data);
    free_dlink_node(ptr);
  }

  DLINK_FOREACH_SAFE(ptr, ptr_next, lt->hide_mask.head)
  {
    MyFree(ptr->data);
    free_dlink_node(ptr);
  }

  MyFree(lt);

  if (MyConnect(source_p))
    source_p->localClient->list_task = NULL;
}
