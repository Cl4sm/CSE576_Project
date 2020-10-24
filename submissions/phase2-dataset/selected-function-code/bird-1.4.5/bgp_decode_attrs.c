bgp_decode_attrs(struct bgp_conn *conn, byte *attr, unsigned int len, struct linpool *pool, int mandatory)
{
  struct bgp_proto *bgp = conn->bgp;
  rta *a = lp_alloc(pool, sizeof(struct rta));
  unsigned int flags, code, l, i, type;
  int errcode;
  byte *z, *attr_start;
  byte seen[256/8];
  ea_list *ea;
  struct adata *ad;
  int withdraw = 0;

  bzero(a, sizeof(rta));
  a->source = RTS_BGP;
  a->scope = SCOPE_UNIVERSE;
  a->cast = RTC_UNICAST;
  /* a->dest = RTD_ROUTER;  -- set in bgp_set_next_hop() */
  a->from = bgp->cf->remote_ip;

  /* Parse the attributes */
  bzero(seen, sizeof(seen));
  DBG("BGP: Parsing attributes\n");
  while (len)
    {
      if (len < 2)
	goto malformed;
      attr_start = attr;
      flags = *attr++;
      code = *attr++;
      len -= 2;
      if (flags & BAF_EXT_LEN)
	{
	  if (len < 2)
	    goto malformed;
	  l = get_u16(attr);
	  attr += 2;
	  len -= 2;
	}
      else
	{
	  if (len < 1)
	    goto malformed;
	  l = *attr++;
	  len--;
	}
      if (l > len)
	goto malformed;
      len -= l;
      z = attr;
      attr += l;
      DBG("Attr %02x %02x %d\n", code, flags, l);
      if (seen[code/8] & (1 << (code%8)))
	goto malformed;
      if (ATTR_KNOWN(code))
	{
	  struct attr_desc *desc = &bgp_attr_table[code];
	  if (desc->expected_length >= 0 && desc->expected_length != (int) l)
	    { errcode = 5; goto err; }
	  if ((desc->expected_flags ^ flags) & (BAF_OPTIONAL | BAF_TRANSITIVE))
	    { errcode = 4; goto err; }
	  if (!desc->allow_in_ebgp && !bgp->is_internal)
	    continue;
	  if (desc->validate)
	    {
	      errcode = desc->validate(bgp, z, l);
	      if (errcode > 0)
		goto err;
	      if (errcode == IGNORE)
		continue;
	      if (errcode <= WITHDRAW)
		{
		  log(L_WARN "%s: Attribute %s is malformed, withdrawing update",
		      bgp->p.name, desc->name);
		  withdraw = 1;
		}
	    }
	  else if (code == BA_AS_PATH)
	    {
	      /* Special case as it might also trim the attribute */
	      if (validate_as_path(bgp, z, &l) < 0)
		{ errcode = 11; goto err; }
	    }
	  type = desc->type;
	}
      else				/* Unknown attribute */
	{
	  if (!(flags & BAF_OPTIONAL))
	    { errcode = 2; goto err; }
	  type = EAF_TYPE_OPAQUE;
	}
      
      // Only OPTIONAL and TRANSITIVE attributes may have non-zero PARTIAL flag
      // if (!((flags & BAF_OPTIONAL) && (flags & BAF_TRANSITIVE)) && (flags & BAF_PARTIAL))
      //   { errcode = 4; goto err; }

      seen[code/8] |= (1 << (code%8));
      ea = lp_alloc(pool, sizeof(ea_list) + sizeof(eattr));
      ea->next = a->eattrs;
      a->eattrs = ea;
      ea->flags = 0;
      ea->count = 1;
      ea->attrs[0].id = EA_CODE(EAP_BGP, code);
      ea->attrs[0].flags = flags;
      ea->attrs[0].type = type;
      if (type & EAF_EMBEDDED)
	ad = NULL;
      else
	{
	  ad = lp_alloc(pool, sizeof(struct adata) + l);
	  ea->attrs[0].u.ptr = ad;
	  ad->length = l;
	  memcpy(ad->data, z, l);
	}
      switch (type)
	{
	case EAF_TYPE_ROUTER_ID:
	case EAF_TYPE_INT:
	  if (l == 1)
	    ea->attrs[0].u.data = *z;
	  else
	    ea->attrs[0].u.data = get_u32(z);
	  break;
	case EAF_TYPE_IP_ADDRESS:
	  ipa_ntoh(*(ip_addr *)ad->data);
	  break;
	case EAF_TYPE_INT_SET:
	case EAF_TYPE_EC_SET:
	  {
	    u32 *z = (u32 *) ad->data;
	    for(i=0; i<ad->length/4; i++)
	      z[i] = ntohl(z[i]);
	    break;
	  }
	}
    }

  if (withdraw)
    goto withdraw;

#ifdef IPV6
  /* If we received MP_REACH_NLRI we should check mandatory attributes */
  if (bgp->mp_reach_len != 0)
    mandatory = 1;
#endif

  /* If there is no (reachability) NLRI, we should exit now */
  if (! mandatory)
    return a;

  /* Check if all mandatory attributes are present */
  for(i=0; i < ARRAY_SIZE(bgp_mandatory_attrs); i++)
    {
      code = bgp_mandatory_attrs[i];
      if (!(seen[code/8] & (1 << (code%8))))
	{
	  bgp_error(conn, 3, 3, &bgp_mandatory_attrs[i], 1);
	  return NULL;
	}
    }

  /* When receiving attributes from non-AS4-aware BGP speaker,
   * we have to reconstruct 4B AS_PATH and AGGREGATOR attributes
   */
  if (! bgp->as4_session)
    bgp_reconstruct_4b_atts(bgp, a, pool);

  bgp_remove_as4_attrs(bgp, a);

  /* If the AS path attribute contains our AS, reject the routes */
  if (bgp_as_path_loopy(bgp, a))
    goto withdraw;

  /* Two checks for IBGP loops caused by route reflection, RFC 4456 */ 
  if (bgp_originator_id_loopy(bgp, a) ||
      bgp_cluster_list_loopy(bgp, a))
    goto withdraw;

  /* If there's no local preference, define one */
  if (!(seen[0] & (1 << BA_LOCAL_PREF)))
    bgp_attach_attr(&a->eattrs, pool, BA_LOCAL_PREF, bgp->cf->default_local_pref);

  return a;

withdraw:
  return NULL;

malformed:
  bgp_error(conn, 3, 1, NULL, 0);
  return NULL;

err:
  bgp_error(conn, 3, errcode, attr_start, z+l-attr_start);
  return NULL;
}
