static int
ospf_pkt_checkauth(struct ospf_neighbor *n, struct ospf_iface *ifa, struct ospf_packet *pkt, int size)
{
  struct proto_ospf *po = ifa->oa->po;
  struct proto *p = &po->proto;
  struct password_item *pass = NULL, *ptmp;
  void *tail;
  char md5sum[OSPF_AUTH_CRYPT_SIZE];
  char password[OSPF_AUTH_CRYPT_SIZE];
  struct MD5Context ctxt;


  if (pkt->autype != htons(ifa->autype))
  {
    OSPF_TRACE(D_PACKETS, "OSPF_auth: Method differs (%d)", ntohs(pkt->autype));
    return 0;
  }

  switch(ifa->autype)
  {
    case OSPF_AUTH_NONE:
      return 1;
      break;
    case OSPF_AUTH_SIMPLE:
      pass = password_find(ifa->passwords, 1);
      if (!pass)
      {
        OSPF_TRACE(D_PACKETS, "OSPF_auth: no password found");
	return 0;
      }
      password_cpy(password, pass->password, sizeof(union ospf_auth));

      if (memcmp(pkt->u.password, password, sizeof(union ospf_auth)))
      {
        char ppass[sizeof(union ospf_auth) + 1];
        bzero(ppass, (sizeof(union ospf_auth) + 1));
        memcpy(ppass, pkt->u.password, sizeof(union ospf_auth));
        OSPF_TRACE(D_PACKETS, "OSPF_auth: different passwords (%s)", ppass);
	return 0;
      }
      return 1;
      break;
    case OSPF_AUTH_CRYPT:
      if (pkt->u.md5.len != OSPF_AUTH_CRYPT_SIZE)
      {
        OSPF_TRACE(D_PACKETS, "OSPF_auth: wrong size of md5 digest");
        return 0;
      }

      if (ntohs(pkt->length) + OSPF_AUTH_CRYPT_SIZE > size)
      {
        OSPF_TRACE(D_PACKETS, "OSPF_auth: size mismatch (%d vs %d)",
	  ntohs(pkt->length) + OSPF_AUTH_CRYPT_SIZE, size);
        return 0;
      }

      tail = ((void *)pkt) + ntohs(pkt->length);

      if (ifa->passwords)
      {
	WALK_LIST(ptmp, *(ifa->passwords))
	{
	  if (pkt->u.md5.keyid != ptmp->id) continue;
	  if ((ptmp->accfrom > now_real) || (ptmp->accto < now_real)) continue;
	  pass = ptmp;
	  break;
	}
      }

      if (!pass)
      {
        OSPF_TRACE(D_PACKETS, "OSPF_auth: no suitable md5 password found");
        return 0;
      }

      if (n)
      {
	u32 rcv_csn = ntohl(pkt->u.md5.csn);
	if(rcv_csn < n->csn)
	{
	  OSPF_TRACE(D_PACKETS, "OSPF_auth: lower sequence number (rcv %d, old %d)", rcv_csn, n->csn);
	  return 0;
	}

	n->csn = rcv_csn;
      }

      MD5Init(&ctxt);
      MD5Update(&ctxt, (char *) pkt, ntohs(pkt->length));
      password_cpy(password, pass->password, OSPF_AUTH_CRYPT_SIZE);
      MD5Update(&ctxt, password, OSPF_AUTH_CRYPT_SIZE);
      MD5Final(md5sum, &ctxt);
      if (memcmp(md5sum, tail, OSPF_AUTH_CRYPT_SIZE))
      {
        OSPF_TRACE(D_PACKETS, "OSPF_auth: wrong md5 digest");
        return 0;
      }
      return 1;
      break;
    default:
      OSPF_TRACE(D_PACKETS, "OSPF_auth: unknown auth type");
      return 0;
  }
}
