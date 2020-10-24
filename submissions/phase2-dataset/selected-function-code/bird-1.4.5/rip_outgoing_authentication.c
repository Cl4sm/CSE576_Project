rip_outgoing_authentication( struct proto *p, struct rip_block_auth *block, struct rip_packet *packet, int num )
{
  struct password_item *passwd = password_find(P_CF->passwords, 1);

  if (!P_CF->authtype)
    return PACKETLEN(num);

  DBG( "Outgoing authentication: " );

  if (!passwd) {
    log( L_ERR "No suitable password found for authentication" );
    return PACKETLEN(num);
  }

  block->authtype = htons(P_CF->authtype);
  block->mustbeFFFF = 0xffff;
  switch (P_CF->authtype) {
  case AT_PLAINTEXT:
    password_cpy( (char *) (&block->packetlen), passwd->password, 16);
    return PACKETLEN(num);
  case AT_MD5:
    {
      struct rip_md5_tail *tail;
      struct MD5Context ctxt;
      static u32 sequence = 0;

      if (num > PACKET_MD5_MAX)
	bug(  "We can not add MD5 authentication to this long packet" );

      /* need to preset the sequence number to a sane value */
      if (!sequence)
	sequence = (u32) time(NULL);

      block->keyid = passwd->id;
      block->authlen = sizeof(struct rip_block_auth);
      block->seq = sequence++;
      block->zero0 = 0;
      block->zero1 = 0;
      block->packetlen = htons(PACKETLEN(num));
      tail = (struct rip_md5_tail *) ((char *) packet + PACKETLEN(num) );
      tail->mustbeFFFF = 0xffff;
      tail->mustbe0001 = 0x0100;

      password_cpy(tail->md5, passwd->password, 16);
      MD5Init(&ctxt);
      MD5Update(&ctxt, (char *) packet, PACKETLEN(num) + sizeof(struct  rip_md5_tail));
      MD5Final(tail->md5, &ctxt);
      return PACKETLEN(num) + block->authlen;
    }
  default:
    bug( "Unknown authtype in outgoing authentication?" );
  }
}
