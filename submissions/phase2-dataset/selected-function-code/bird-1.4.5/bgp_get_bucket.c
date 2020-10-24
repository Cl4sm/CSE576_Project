static struct bgp_bucket *
bgp_get_bucket(struct bgp_proto *p, net *n, ea_list *attrs, int originate)
{
  ea_list *new;
  unsigned i, cnt, hash, code;
  eattr *a, *d;
  u32 seen = 0;
  struct bgp_bucket *b;

  /* Merge the attribute list */
  new = alloca(ea_scan(attrs));
  ea_merge(attrs, new);
  ea_sort(new);

  /* Normalize attributes */
  d = new->attrs;
  cnt = new->count;
  new->count = 0;
  for(i=0; i<cnt; i++)
    {
      a = &new->attrs[i];
      if (EA_PROTO(a->id) != EAP_BGP)
	continue;
      code = EA_ID(a->id);
      if (ATTR_KNOWN(code))
	{
	  if (!bgp_attr_table[code].allow_in_ebgp && !p->is_internal)
	    continue;
	  /* The flags might have been zero if the attr was added by filters */
	  a->flags = (a->flags & BAF_PARTIAL) | bgp_attr_table[code].expected_flags;
	  if (code < 32)
	    seen |= 1 << code;
	}
      else
	{
	  /* Don't re-export unknown non-transitive attributes */
	  if (!(a->flags & BAF_TRANSITIVE))
	    continue;
	}
      *d = *a;
      if ((d->type & EAF_ORIGINATED) && !originate && (d->flags & BAF_TRANSITIVE) && (d->flags & BAF_OPTIONAL))
	d->flags |= BAF_PARTIAL;
      switch (d->type & EAF_TYPE_MASK)
	{
	case EAF_TYPE_INT_SET:
	  {
	    struct adata *z = alloca(sizeof(struct adata) + d->u.ptr->length);
	    z->length = d->u.ptr->length;
	    bgp_normalize_int_set((u32 *) z->data, (u32 *) d->u.ptr->data, z->length / 4);
	    d->u.ptr = z;
	    break;
	  }
	case EAF_TYPE_EC_SET:
	  {
	    struct adata *z = alloca(sizeof(struct adata) + d->u.ptr->length);
	    z->length = d->u.ptr->length;
	    bgp_normalize_ec_set(z, (u32 *) d->u.ptr->data, p->is_internal);
	    d->u.ptr = z;
	    break;
	  }
	default: ;
	}
      d++;
      new->count++;
    }

  /* Hash */
  hash = ea_hash(new);
  for(b=p->bucket_hash[hash & (p->hash_size - 1)]; b; b=b->hash_next)
    if (b->hash == hash && ea_same(b->eattrs, new))
      {
	DBG("Found bucket.\n");
	return b;
      }

  /* Ensure that there are all mandatory attributes */
  for(i=0; i<ARRAY_SIZE(bgp_mandatory_attrs); i++)
    if (!(seen & (1 << bgp_mandatory_attrs[i])))
      {
	log(L_ERR "%s: Mandatory attribute %s missing in route %I/%d", p->p.name, bgp_attr_table[bgp_mandatory_attrs[i]].name, n->n.prefix, n->n.pxlen);
	return NULL;
      }

  /* Check if next hop is valid */
  a = ea_find(new, EA_CODE(EAP_BGP, BA_NEXT_HOP));
  if (!a || ipa_equal(p->cf->remote_ip, *(ip_addr *)a->u.ptr->data))
    {
      log(L_ERR "%s: Invalid NEXT_HOP attribute in route %I/%d", p->p.name, n->n.prefix, n->n.pxlen);
      return NULL;
    }

  /* Create new bucket */
  DBG("Creating bucket.\n");
  return bgp_new_bucket(p, new, hash);
}
