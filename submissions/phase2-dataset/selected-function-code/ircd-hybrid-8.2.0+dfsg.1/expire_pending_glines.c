expire_pending_glines(struct gline_pending *in)
{
  dlink_node *ptr = NULL, *ptr_next = NULL;

  for (unsigned int i = 0; i < GLINE_PENDING_ADD_TYPE + 1; ++i)
  {
    DLINK_FOREACH_SAFE(ptr, ptr_next, pending_glines[i].head)
    {
      struct gline_pending *glp_ptr = ptr->data;

      if ((glp_ptr->last_gline_time + ConfigGeneral.gline_request_time) <= CurrentTime ||
          glp_ptr == in)
      {
        dlinkDelete(&glp_ptr->node, &pending_glines[i]);
        MyFree(glp_ptr);
      }
    }
  }
}
