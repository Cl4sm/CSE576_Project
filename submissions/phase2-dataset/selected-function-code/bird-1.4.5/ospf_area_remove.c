ospf_area_remove(struct ospf_area *oa)
{
  struct proto *p = &oa->po->proto;
  OSPF_TRACE(D_EVENTS, "Removing area %R", oa->areaid);

  /* We suppose that interfaces are already removed */
  ospf_flush_area(oa->po, oa->areaid);
 
  fib_free(&oa->rtr);
  fib_free(&oa->net_fib);
  fib_free(&oa->enet_fib);

  if (oa->translator_timer)
    rfree(oa->translator_timer);

  oa->po->areano--;
  rem_node(NODE oa);
  mb_free(oa);
}
