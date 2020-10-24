add_area_nets(struct ospf_area *oa, struct ospf_area_config *ac)
{
    struct proto_ospf *po = oa->po;
    struct area_net_config *anc;
    struct area_net *an;

    fib_init(&oa->net_fib, po->proto.pool, sizeof(struct area_net), 0, ospf_area_initfib);
    fib_init(&oa->enet_fib, po->proto.pool, sizeof(struct area_net), 0, ospf_area_initfib);

    WALK_LIST(anc, ac->net_list)
    {
      an = (struct area_net *) fib_get(&oa->net_fib, &anc->px.addr, anc->px.len);
      an->hidden = anc->hidden;
    }

    WALK_LIST(anc, ac->enet_list)
    {
      an = (struct area_net *) fib_get(&oa->enet_fib, &anc->px.addr, anc->px.len);
      an->hidden = anc->hidden;
      an->tag = anc->tag;
    }
}
