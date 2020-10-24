ospf_pkt_fill_hdr(struct ospf_iface *ifa, void *buf, u8 h_type)
{
  struct proto_ospf *po = ifa->oa->po;
  struct ospf_packet *pkt;

  pkt = (struct ospf_packet *) buf;

  pkt->version = OSPF_VERSION;

  pkt->type = h_type;

  pkt->routerid = htonl(po->router_id);
  pkt->areaid = htonl(ifa->oa->areaid);

#ifdef OSPFv3
  pkt->instance_id = ifa->instance_id;
#endif

#ifdef OSPFv2
  pkt->autype = htons(ifa->autype);
#endif

  pkt->checksum = 0;
}
