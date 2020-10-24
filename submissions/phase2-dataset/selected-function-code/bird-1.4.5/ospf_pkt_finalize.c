ospf_pkt_finalize(struct ospf_iface *ifa, struct ospf_packet *pkt)
{
  struct password_item *passwd = NULL;
  void *tail;
  struct MD5Context ctxt;
  char password[OSPF_AUTH_CRYPT_SIZE];

  pkt->checksum = 0;
  pkt->autype = htons(ifa->autype);
  bzero(&pkt->u, sizeof(union ospf_auth));

  /* Compatibility note: pkt->u may contain anything if autype is
     none, but nonzero values do not work with Mikrotik OSPF */

  switch(ifa->autype)
  {
    case OSPF_AUTH_SIMPLE:
      passwd = password_find(ifa->passwords, 1);
      if (!passwd)
      {
        log( L_ERR "No suitable password found for authentication" );
        return;
      }
      password_cpy(pkt->u.password, passwd->password, sizeof(union ospf_auth));
    case OSPF_AUTH_NONE:
      pkt->checksum = ipsum_calculate(pkt, sizeof(struct ospf_packet) -
                                  sizeof(union ospf_auth), (pkt + 1),
				  ntohs(pkt->length) -
				  sizeof(struct ospf_packet), NULL);
      break;
    case OSPF_AUTH_CRYPT:
      passwd = password_find(ifa->passwords, 0);
      if (!passwd)
      {
        log( L_ERR "No suitable password found for authentication" );
        return;
      }

      /* Perhaps use random value to prevent replay attacks after
	 reboot when system does not have independent RTC? */
      if (!ifa->csn)
	{
	  ifa->csn = (u32) now;
	  ifa->csn_use = now;
	}

      /* We must have sufficient delay between sending a packet and increasing 
	 CSN to prevent reordering of packets (in a network) with different CSNs */
      if ((now - ifa->csn_use) > 1)
	ifa->csn++;

      ifa->csn_use = now;

      pkt->u.md5.keyid = passwd->id;
      pkt->u.md5.len = OSPF_AUTH_CRYPT_SIZE;
      pkt->u.md5.zero = 0;
      pkt->u.md5.csn = htonl(ifa->csn);
      tail = ((void *)pkt) + ntohs(pkt->length);
      MD5Init(&ctxt);
      MD5Update(&ctxt, (char *) pkt, ntohs(pkt->length));
      password_cpy(password, passwd->password, OSPF_AUTH_CRYPT_SIZE);
      MD5Update(&ctxt, password, OSPF_AUTH_CRYPT_SIZE);
      MD5Final(tail, &ctxt);
      break;
    default:
      bug("Unknown authentication type");
  }
}
