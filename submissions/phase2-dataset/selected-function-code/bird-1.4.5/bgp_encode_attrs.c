bgp_encode_attrs(struct bgp_proto *p, byte *w, ea_list *attrs, int remains)
{
  unsigned int i, code, type, flags;
  byte *start = w;
  int len, rv;

  for(i=0; i<attrs->count; i++)
    {
      eattr *a = &attrs->attrs[i];
      ASSERT(EA_PROTO(a->id) == EAP_BGP);
      code = EA_ID(a->id);

#ifdef IPV6
      /* When talking multiprotocol BGP, the NEXT_HOP attributes are used only temporarily. */
      if (code == BA_NEXT_HOP)
	continue;
#endif

      /* When AS4-aware BGP speaker is talking to non-AS4-aware BGP speaker,
       * we have to convert our 4B AS_PATH to 2B AS_PATH and send our AS_PATH 
       * as optional AS4_PATH attribute.
       */
      if ((code == BA_AS_PATH) && (! p->as4_session))
	{
	  len = a->u.ptr->length;

	  if (remains < (len + 4))
	    goto err_no_buffer;

	  /* Using temporary buffer because don't know a length of created attr
	   * and therefore a length of a header. Perhaps i should better always
	   * use BAF_EXT_LEN. */
	  
	  byte buf[len];
	  int new_used;
	  int nl = as_path_convert_to_old(a->u.ptr, buf, &new_used);

	  DBG("BGP: Encoding old AS_PATH\n");
	  rv = bgp_encode_attr_hdr(w, BAF_TRANSITIVE, BA_AS_PATH, nl);
	  ADVANCE(w, remains, rv);
	  memcpy(w, buf, nl);
	  ADVANCE(w, remains, nl);

	  if (! new_used)
	    continue;

	  if (remains < (len + 4))
	    goto err_no_buffer;

	  /* We should discard AS_CONFED_SEQUENCE or AS_CONFED_SET path segments 
	   * here but we don't support confederations and such paths we already
	   * discarded in bgp_check_as_path().
	   */

	  DBG("BGP: Encoding AS4_PATH\n");
	  rv = bgp_encode_attr_hdr(w, BAF_OPTIONAL | BAF_TRANSITIVE, BA_AS4_PATH, len);
	  ADVANCE(w, remains, rv);
	  memcpy(w, a->u.ptr->data, len);
	  ADVANCE(w, remains, len);

	  continue;
	}

      /* The same issue with AGGREGATOR attribute */
      if ((code == BA_AGGREGATOR) && (! p->as4_session))
	{
	  int new_used;

	  len = 6;
	  if (remains < (len + 3))
	    goto err_no_buffer;

	  rv = bgp_encode_attr_hdr(w, BAF_OPTIONAL | BAF_TRANSITIVE, BA_AGGREGATOR, len);
	  ADVANCE(w, remains, rv);
	  aggregator_convert_to_old(a->u.ptr, w, &new_used);
	  ADVANCE(w, remains, len);

	  if (! new_used)
	    continue;

	  len = 8;
	  if (remains < (len + 3))
	    goto err_no_buffer;

	  rv = bgp_encode_attr_hdr(w, BAF_OPTIONAL | BAF_TRANSITIVE, BA_AS4_AGGREGATOR, len);
	  ADVANCE(w, remains, rv);
	  memcpy(w, a->u.ptr->data, len);
	  ADVANCE(w, remains, len);

	  continue;
	}

      /* Standard path continues here ... */

      type = a->type & EAF_TYPE_MASK;
      flags = a->flags & (BAF_OPTIONAL | BAF_TRANSITIVE | BAF_PARTIAL);
      len = bgp_get_attr_len(a);

      /* Skip empty sets */ 
      if (((type == EAF_TYPE_INT_SET) || (type == EAF_TYPE_EC_SET)) && (len == 0))
	continue; 

      if (remains < len + 4)
	goto err_no_buffer;

      rv = bgp_encode_attr_hdr(w, flags, code, len);
      ADVANCE(w, remains, rv);

      switch (type)
	{
	case EAF_TYPE_INT:
	case EAF_TYPE_ROUTER_ID:
	  if (len == 4)
	    put_u32(w, a->u.data);
	  else
	    *w = a->u.data;
	  break;
	case EAF_TYPE_IP_ADDRESS:
	  {
	    ip_addr ip = *(ip_addr *)a->u.ptr->data;
	    ipa_hton(ip);
	    memcpy(w, &ip, len);
	    break;
	  }
	case EAF_TYPE_INT_SET:
	case EAF_TYPE_EC_SET:
	  {
	    u32 *z = int_set_get_data(a->u.ptr);
	    int i;
	    for(i=0; i<len; i+=4)
	      put_u32(w+i, *z++);
	    break;
	  }
	case EAF_TYPE_OPAQUE:
	case EAF_TYPE_AS_PATH:
	  memcpy(w, a->u.ptr->data, len);
	  break;
	default:
	  bug("bgp_encode_attrs: unknown attribute type %02x", a->type);
	}
      ADVANCE(w, remains, len);
    }
  return w - start;

 err_no_buffer:
  return -1;
}
