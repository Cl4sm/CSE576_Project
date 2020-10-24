interpret(struct f_inst *what)
{
  struct symbol *sym;
  struct f_val v1, v2, res, *vp;
  unsigned u1, u2;
  int i;
  u32 as;

  res.type = T_VOID;
  if (!what)
    return res;

  switch(what->code) {
  case ',':
    TWOARGS;
    break;

/* Binary operators */
  case '+':
    TWOARGS_C;
    switch (res.type = v1.type) {
    case T_VOID: runtime( "Can't operate with values of type void" );
    case T_INT: res.val.i = v1.val.i + v2.val.i; break;
    default: runtime( "Usage of unknown type" );
    }
    break;
  case '-':
    TWOARGS_C;
    switch (res.type = v1.type) {
    case T_VOID: runtime( "Can't operate with values of type void" );
    case T_INT: res.val.i = v1.val.i - v2.val.i; break;
    default: runtime( "Usage of unknown type" );
    }
    break;
  case '*':
    TWOARGS_C;
    switch (res.type = v1.type) {
    case T_VOID: runtime( "Can't operate with values of type void" );
    case T_INT: res.val.i = v1.val.i * v2.val.i; break;
    default: runtime( "Usage of unknown type" );
    }
    break;
  case '/':
    TWOARGS_C;
    switch (res.type = v1.type) {
    case T_VOID: runtime( "Can't operate with values of type void" );
    case T_INT: if (v2.val.i == 0) runtime( "Mother told me not to divide by 0" );
      	        res.val.i = v1.val.i / v2.val.i; break;
    default: runtime( "Usage of unknown type" );
    }
    break;
    
  case '&':
  case '|':
    ARG(v1, a1.p);
    if (v1.type != T_BOOL)
      runtime( "Can't do boolean operation on non-booleans" );
    if (v1.val.i == (what->code == '|')) {
      res.type = T_BOOL;
      res.val.i = v1.val.i;
      break;
    }

    ARG(v2, a2.p);
    if (v2.type != T_BOOL)
      runtime( "Can't do boolean operation on non-booleans" );
    res.type = T_BOOL;
    res.val.i = v2.val.i;
    break;

  case P('m','p'):
    TWOARGS;
    if ((v1.type != T_INT) || (v2.type != T_INT))
      runtime( "Can't operate with value of non-integer type in pair constructor" );
    u1 = v1.val.i;
    u2 = v2.val.i;
    if ((u1 > 0xFFFF) || (u2 > 0xFFFF))
      runtime( "Can't operate with value out of bounds in pair constructor" );
    res.val.i = (u1 << 16) | u2;
    res.type = T_PAIR;
    break;

  case P('m','c'):
    {
      TWOARGS;

      int check, ipv4_used;
      u32 key, val;

      if (v1.type == T_INT) {
	ipv4_used = 0; key = v1.val.i;
      } 
      else if (v1.type == T_QUAD) {
	ipv4_used = 1; key = v1.val.i;
      }
#ifndef IPV6
      /* IP->Quad implicit conversion */
      else if (v1.type == T_IP) {
	ipv4_used = 1; key = ipa_to_u32(v1.val.px.ip);
      }
#endif
      else
	runtime("Can't operate with key of non-integer/IPv4 type in EC constructor");

      if (v2.type != T_INT)
	runtime("Can't operate with value of non-integer type in EC constructor");
      val = v2.val.i;

      res.type = T_EC;

      if (what->aux == EC_GENERIC) {
	check = 0; res.val.ec = ec_generic(key, val);
      }
      else if (ipv4_used) {
	check = 1; res.val.ec = ec_ip4(what->aux, key, val);
      }
      else if (key < 0x10000) {
	check = 0; res.val.ec = ec_as2(what->aux, key, val);
      }
      else {
	check = 1; res.val.ec = ec_as4(what->aux, key, val);
      }

      if (check && (val > 0xFFFF))
	runtime("Can't operate with value out of bounds in EC constructor");

      break;
    }

/* Relational operators */

#define COMPARE(x) \
    TWOARGS; \
    i = val_compare(v1, v2); \
    if (i==CMP_ERROR) \
      runtime( "Can't compare values of incompatible types" ); \
    res.type = T_BOOL; \
    res.val.i = (x); \
    break;

#define SAME(x) \
    TWOARGS; \
    i = val_same(v1, v2); \
    res.type = T_BOOL; \
    res.val.i = (x); \
    break;

  case P('!','='): SAME(!i);
  case P('=','='): SAME(i);
  case '<': COMPARE(i==-1);
  case P('<','='): COMPARE(i!=1);

  case '!':
    ONEARG;
    if (v1.type != T_BOOL)
      runtime( "Not applied to non-boolean" );
    res = v1;
    res.val.i = !res.val.i;
    break;

  case '~':
    TWOARGS;
    res.type = T_BOOL;
    res.val.i = val_in_range(v1, v2);
    if (res.val.i == CMP_ERROR)
      runtime( "~ applied on unknown type pair" );
    res.val.i = !!res.val.i;
    break;
  case P('d','e'):
    ONEARG;
    res.type = T_BOOL;
    res.val.i = (v1.type != T_VOID);
    break;

  /* Set to indirect value, a1 = variable, a2 = value */
  case 's':
    ARG(v2, a2.p);
    sym = what->a1.p;
    vp = sym->def;
    if ((sym->class != (SYM_VARIABLE | v2.type)) && (v2.type != T_VOID)) {
#ifndef IPV6
      /* IP->Quad implicit conversion */
      if ((sym->class == (SYM_VARIABLE | T_QUAD)) && (v2.type == T_IP)) {
	vp->type = T_QUAD;
	vp->val.i = ipa_to_u32(v2.val.px.ip);
	break;
      }
#endif
      runtime( "Assigning to variable of incompatible type" );
    }
    *vp = v2; 
    break;

    /* some constants have value in a2, some in *a1.p, strange. */
  case 'c':	/* integer (or simple type) constant, string, set, or prefix_set */
    res.type = what->aux;

    if (res.type == T_PREFIX_SET)
      res.val.ti = what->a2.p;
    else if (res.type == T_SET)
      res.val.t = what->a2.p;
    else if (res.type == T_STRING)
      res.val.s = what->a2.p;
    else
      res.val.i = what->a2.i;
    break;
  case 'V':
  case 'C':
    res = * ((struct f_val *) what->a1.p);
    break;
  case 'p':
    ONEARG;
    val_format(v1, &f_buf);
    break;
  case '?':	/* ? has really strange error value, so we can implement if ... else nicely :-) */
    ONEARG;
    if (v1.type != T_BOOL)
      runtime( "If requires boolean expression" );
    if (v1.val.i) {
      ARG(res,a2.p);
      res.val.i = 0;
    } else res.val.i = 1;
    res.type = T_BOOL;
    break;
  case '0':
    debug( "No operation\n" );
    break;
  case P('p',','):
    ONEARG;
    if (what->a2.i == F_NOP || (what->a2.i != F_NONL && what->a1.p))
      log_commit(*L_INFO, &f_buf);

    switch (what->a2.i) {
    case F_QUITBIRD:
      die( "Filter asked me to die" );
    case F_ACCEPT:
      /* Should take care about turning ACCEPT into MODIFY */
    case F_ERROR:
    case F_REJECT:	/* FIXME (noncritical) Should print complete route along with reason to reject route */
      res.type = T_RETURN;
      res.val.i = what->a2.i;
      return res;	/* We have to return now, no more processing. */
    case F_NONL:
    case F_NOP:
      break;
    default:
      bug( "unknown return type: Can't happen");
    }
    break;
  case 'a':	/* rta access */
    {
      ACCESS_RTE;
      struct rta *rta = (*f_rte)->attrs;
      res.type = what->aux;

      switch (what->a2.i)
      {
      case SA_FROM:	res.val.px.ip = rta->from; break;
      case SA_GW:	res.val.px.ip = rta->gw; break;
      case SA_NET:	res.val.px.ip = (*f_rte)->net->n.prefix;
			res.val.px.len = (*f_rte)->net->n.pxlen; break;
      case SA_PROTO:	res.val.s = rta->src->proto->name; break;
      case SA_SOURCE:	res.val.i = rta->source; break;
      case SA_SCOPE:	res.val.i = rta->scope; break;
      case SA_CAST:	res.val.i = rta->cast; break;
      case SA_DEST:	res.val.i = rta->dest; break;
      case SA_IFNAME:	res.val.s = rta->iface ? rta->iface->name : ""; break;
      case SA_IFINDEX:	res.val.i = rta->iface ? rta->iface->index : 0; break;

      default:
	bug("Invalid static attribute access (%x)", res.type);
      }
    }
    break;
  case P('a','S'):
    ACCESS_RTE;
    ONEARG;
    if (what->aux != v1.type)
      runtime( "Attempt to set static attribute to incompatible type" );

    f_rta_cow();
    {
      struct rta *rta = (*f_rte)->attrs;

      switch (what->a2.i)
      {
      case SA_FROM:
	rta->from = v1.val.px.ip;
	break;

      case SA_GW:
	{
	  ip_addr ip = v1.val.px.ip;
	  neighbor *n = neigh_find(rta->src->proto, &ip, 0);
	  if (!n || (n->scope == SCOPE_HOST))
	    runtime( "Invalid gw address" );

	  rta->dest = RTD_ROUTER;
	  rta->gw = ip;
	  rta->iface = n->iface;
	  rta->nexthops = NULL;
	  rta->hostentry = NULL;
	}
	break;

      case SA_SCOPE:
	rta->scope = v1.val.i;
	break;

      case SA_DEST:
	i = v1.val.i;
	if ((i != RTD_BLACKHOLE) && (i != RTD_UNREACHABLE) && (i != RTD_PROHIBIT))
	  runtime( "Destination can be changed only to blackhole, unreachable or prohibit" );

	rta->dest = i;
	rta->gw = IPA_NONE;
	rta->iface = NULL;
	rta->nexthops = NULL;
	rta->hostentry = NULL;
	break;

      default:
	bug("Invalid static attribute access (%x)", res.type);
      }
    }
    break;
  case P('e','a'):	/* Access to extended attributes */
    ACCESS_RTE;
    {
      eattr *e = NULL;
      if (!(f_flags & FF_FORCE_TMPATTR))
	e = ea_find( (*f_rte)->attrs->eattrs, what->a2.i );
      if (!e) 
	e = ea_find( (*f_tmp_attrs), what->a2.i );
      if ((!e) && (f_flags & FF_FORCE_TMPATTR))
	e = ea_find( (*f_rte)->attrs->eattrs, what->a2.i );

      if (!e) {
	/* A special case: undefined int_set looks like empty int_set */
	if ((what->aux & EAF_TYPE_MASK) == EAF_TYPE_INT_SET) {
	  res.type = T_CLIST;
	  res.val.ad = adata_empty(f_pool, 0);
	  break;
	}
	/* The same special case for ec_set */
	else if ((what->aux & EAF_TYPE_MASK) == EAF_TYPE_EC_SET) {
	  res.type = T_ECLIST;
	  res.val.ad = adata_empty(f_pool, 0);
	  break;
	}

	/* Undefined value */
	res.type = T_VOID;
	break;
      }

      switch (what->aux & EAF_TYPE_MASK) {
      case EAF_TYPE_INT:
	res.type = T_INT;
	res.val.i = e->u.data;
	break;
      case EAF_TYPE_ROUTER_ID:
	res.type = T_QUAD;
	res.val.i = e->u.data;
	break;
      case EAF_TYPE_OPAQUE:
	res.type = T_ENUM_EMPTY;
	res.val.i = 0;
	break;
      case EAF_TYPE_IP_ADDRESS:
	res.type = T_IP;
	struct adata * ad = e->u.ptr;
	res.val.px.ip = * (ip_addr *) ad->data;
	break;
      case EAF_TYPE_AS_PATH:
        res.type = T_PATH;
	res.val.ad = e->u.ptr;
	break;
      case EAF_TYPE_INT_SET:
	res.type = T_CLIST;
	res.val.ad = e->u.ptr;
	break;
      case EAF_TYPE_EC_SET:
	res.type = T_ECLIST;
	res.val.ad = e->u.ptr;
	break;
      case EAF_TYPE_UNDEF:
	res.type = T_VOID;
	break;
      default:
	bug("Unknown type in e,a");
      }
    }
    break;
  case P('e','S'):
    ACCESS_RTE;
    ONEARG;
    {
      struct ea_list *l = lp_alloc(f_pool, sizeof(struct ea_list) + sizeof(eattr));

      l->next = NULL;
      l->flags = EALF_SORTED;
      l->count = 1;
      l->attrs[0].id = what->a2.i;
      l->attrs[0].flags = 0;
      l->attrs[0].type = what->aux | EAF_ORIGINATED;
      switch (what->aux & EAF_TYPE_MASK) {
      case EAF_TYPE_INT:
	if (v1.type != T_INT)
	  runtime( "Setting int attribute to non-int value" );
	l->attrs[0].u.data = v1.val.i;
	break;

      case EAF_TYPE_ROUTER_ID:
#ifndef IPV6
	/* IP->Quad implicit conversion */
	if (v1.type == T_IP) {
	  l->attrs[0].u.data = ipa_to_u32(v1.val.px.ip);
	  break;
	}
#endif
	/* T_INT for backward compatibility */
	if ((v1.type != T_QUAD) && (v1.type != T_INT))
	  runtime( "Setting quad attribute to non-quad value" );
	l->attrs[0].u.data = v1.val.i;
	break;

      case EAF_TYPE_OPAQUE:
	runtime( "Setting opaque attribute is not allowed" );
	break;
      case EAF_TYPE_IP_ADDRESS:
	if (v1.type != T_IP)
	  runtime( "Setting ip attribute to non-ip value" );
	int len = sizeof(ip_addr);
	struct adata *ad = lp_alloc(f_pool, sizeof(struct adata) + len);
	ad->length = len;
	(* (ip_addr *) ad->data) = v1.val.px.ip;
	l->attrs[0].u.ptr = ad;
	break;
      case EAF_TYPE_AS_PATH:
	if (v1.type != T_PATH)
	  runtime( "Setting path attribute to non-path value" );
	l->attrs[0].u.ptr = v1.val.ad;
	break;
      case EAF_TYPE_INT_SET:
	if (v1.type != T_CLIST)
	  runtime( "Setting clist attribute to non-clist value" );
	l->attrs[0].u.ptr = v1.val.ad;
	break;
      case EAF_TYPE_EC_SET:
	if (v1.type != T_ECLIST)
	  runtime( "Setting eclist attribute to non-eclist value" );
	l->attrs[0].u.ptr = v1.val.ad;
	break;
      case EAF_TYPE_UNDEF:
	if (v1.type != T_VOID)
	  runtime( "Setting void attribute to non-void value" );
	l->attrs[0].u.data = 0;
	break;
      default: bug("Unknown type in e,S");
      }

      if (!(what->aux & EAF_TEMP) && (!(f_flags & FF_FORCE_TMPATTR))) {
	f_rta_cow();
	l->next = (*f_rte)->attrs->eattrs;
	(*f_rte)->attrs->eattrs = l;
      } else {
	l->next = (*f_tmp_attrs);
	(*f_tmp_attrs) = l;
      }
    }
    break;
  case 'P':
    ACCESS_RTE;
    res.type = T_INT;
    res.val.i = (*f_rte)->pref;
    break;
  case P('P','S'):
    ACCESS_RTE;
    ONEARG;
    if (v1.type != T_INT)
      runtime( "Can't set preference to non-integer" );
    if ((v1.val.i < 0) || (v1.val.i > 0xFFFF))
      runtime( "Setting preference value out of bounds" );
    f_rte_cow();
    (*f_rte)->pref = v1.val.i;
    break;
  case 'L':	/* Get length of */
    ONEARG;
    res.type = T_INT;
    switch(v1.type) {
    case T_PREFIX: res.val.i = v1.val.px.len; break;
    case T_PATH:   res.val.i = as_path_getlen(v1.val.ad); break;
    case T_CLIST:  res.val.i = int_set_get_size(v1.val.ad); break;
    case T_ECLIST: res.val.i = ec_set_get_size(v1.val.ad); break;
    default: runtime( "Prefix, path, clist or eclist expected" );
    }
    break;
  case P('c','p'):	/* Convert prefix to ... */
    ONEARG;
    if (v1.type != T_PREFIX)
      runtime( "Prefix expected" );
    res.type = what->aux;
    switch(res.type) {
      /*    case T_INT:	res.val.i = v1.val.px.len; break; Not needed any more */
    case T_IP: res.val.px.ip = v1.val.px.ip; break;
    default: bug( "Unknown prefix to conversion" );
    }
    break;
  case P('a','f'):	/* Get first ASN from AS PATH */
    ONEARG;
    if (v1.type != T_PATH)
      runtime( "AS path expected" );

    as = 0;
    as_path_get_first(v1.val.ad, &as);
    res.type = T_INT;
    res.val.i = as;
    break;
  case P('a','l'):	/* Get last ASN from AS PATH */
    ONEARG;
    if (v1.type != T_PATH)
      runtime( "AS path expected" );

    as = 0;
    as_path_get_last(v1.val.ad, &as);
    res.type = T_INT;
    res.val.i = as;
    break;
  case 'r':
    ONEARG;
    res = v1;
    res.type |= T_RETURN;
    return res;
  case P('c','a'): /* CALL: this is special: if T_RETURN and returning some value, mask it out  */
    ONEARG;
    res = interpret(what->a2.p);
    if (res.type == T_RETURN)
      return res;
    res.type &= ~T_RETURN;    
    break;
  case P('c','v'):	/* Clear local variables */
    for (sym = what->a1.p; sym != NULL; sym = sym->aux2)
      ((struct f_val *) sym->def)->type = T_VOID;
    break;
  case P('S','W'):
    ONEARG;
    {
      struct f_tree *t = find_tree(what->a2.p, v1);
      if (!t) {
	v1.type = T_VOID;
	t = find_tree(what->a2.p, v1);
	if (!t) {
	  debug( "No else statement?\n");
	  break;
	}
      }	
      /* It is actually possible to have t->data NULL */

      res = interpret(t->data);
      if (res.type & T_RETURN)
	return res;
    }
    break;
  case P('i','M'): /* IP.MASK(val) */
    TWOARGS;
    if (v2.type != T_INT)
      runtime( "Integer expected");
    if (v1.type != T_IP)
      runtime( "You can mask only IP addresses" );
    {
      ip_addr mask = ipa_mkmask(v2.val.i);
      res.type = T_IP;
      res.val.px.ip = ipa_and(mask, v1.val.px.ip);
    }
    break;

  case 'E':	/* Create empty attribute */
    res.type = what->aux;
    res.val.ad = adata_empty(f_pool, 0);
    break;
  case P('A','p'):	/* Path prepend */
    TWOARGS;
    if (v1.type != T_PATH)
      runtime("Can't prepend to non-path");
    if (v2.type != T_INT)
      runtime("Can't prepend non-integer");

    res.type = T_PATH;
    res.val.ad = as_path_prepend(f_pool, v1.val.ad, v2.val.i);
    break;

  case P('C','a'):	/* (Extended) Community list add or delete */
    TWOARGS;
    if (v1.type == T_PATH)
    {
      struct f_tree *set = NULL;
      u32 key = 0;
      int pos;

      if (v2.type == T_INT)
	key = v2.val.i;
      else if ((v2.type == T_SET) && (v2.val.t->from.type == T_INT))
	set = v2.val.t;
      else
	runtime("Can't delete non-integer (set)");

      switch (what->aux)
      {
      case 'a':	runtime("Can't add to path");
      case 'd':	pos = 0; break;
      case 'f':	pos = 1; break;
      default:	bug("unknown Ca operation");
      }

      if (pos && !set)
	runtime("Can't filter integer");

      res.type = T_PATH;
      res.val.ad = as_path_filter(f_pool, v1.val.ad, set, key, pos);
    }
    else if (v1.type == T_CLIST)
    {
      /* Community (or cluster) list */
      struct f_val dummy;
      int arg_set = 0;
      uint n = 0;

      if ((v2.type == T_PAIR) || (v2.type == T_QUAD))
	n = v2.val.i;
#ifndef IPV6
      /* IP->Quad implicit conversion */
      else if (v2.type == T_IP)
	n = ipa_to_u32(v2.val.px.ip);
#endif
      else if ((v2.type == T_SET) && clist_set_type(v2.val.t, &dummy))
	arg_set = 1;
      else if (v2.type == T_CLIST)
	arg_set = 2;
      else
	runtime("Can't add/delete non-pair");

      res.type = T_CLIST;
      switch (what->aux)
      {
      case 'a':
	if (arg_set == 1)
	  runtime("Can't add set");
	else if (!arg_set)
	  res.val.ad = int_set_add(f_pool, v1.val.ad, n);
	else 
	  res.val.ad = int_set_union(f_pool, v1.val.ad, v2.val.ad);
	break;
      
      case 'd':
	if (!arg_set)
	  res.val.ad = int_set_del(f_pool, v1.val.ad, n);
	else
	  res.val.ad = clist_filter(f_pool, v1.val.ad, v2, 0);
	break;

      case 'f':
	if (!arg_set)
	  runtime("Can't filter pair");
	res.val.ad = clist_filter(f_pool, v1.val.ad, v2, 1);
	break;

      default:
	bug("unknown Ca operation");
      }
    }
    else if (v1.type == T_ECLIST)
    {
      /* Extended community list */
      int arg_set = 0;
      
      /* v2.val is either EC or EC-set */
      if ((v2.type == T_SET) && eclist_set_type(v2.val.t))
	arg_set = 1;
      else if (v2.type == T_ECLIST)
	arg_set = 2;
      else if (v2.type != T_EC)
	runtime("Can't add/delete non-pair");

      res.type = T_ECLIST;
      switch (what->aux)
      {
      case 'a':
	if (arg_set == 1)
	  runtime("Can't add set");
	else if (!arg_set)
	  res.val.ad = ec_set_add(f_pool, v1.val.ad, v2.val.ec);
	else 
	  res.val.ad = ec_set_union(f_pool, v1.val.ad, v2.val.ad);
	break;
      
      case 'd':
	if (!arg_set)
	  res.val.ad = ec_set_del(f_pool, v1.val.ad, v2.val.ec);
	else
	  res.val.ad = eclist_filter(f_pool, v1.val.ad, v2, 0);
	break;

      case 'f':
	if (!arg_set)
	  runtime("Can't filter ec");
	res.val.ad = eclist_filter(f_pool, v1.val.ad, v2, 1);
	break;

      default:
	bug("unknown Ca operation");
      }
    }
    else
      runtime("Can't add/delete to non-(e)clist");

    break;

  case P('R','C'):	/* ROA Check */
    if (what->arg1)
    {
      TWOARGS;
      if ((v1.type != T_PREFIX) || (v2.type != T_INT))
	runtime("Invalid argument to roa_check()");

      as = v2.val.i;
    }
    else
    {
      ACCESS_RTE;
      v1.val.px.ip = (*f_rte)->net->n.prefix;
      v1.val.px.len = (*f_rte)->net->n.pxlen;

      /* We ignore temporary attributes, probably not a problem here */
      /* 0x02 is a value of BA_AS_PATH, we don't want to include BGP headers */
      eattr *e = ea_find((*f_rte)->attrs->eattrs, EA_CODE(EAP_BGP, 0x02));

      if (!e || e->type != EAF_TYPE_AS_PATH)
	runtime("Missing AS_PATH attribute");

      as_path_get_last(e->u.ptr, &as);
    }

    struct roa_table_config *rtc = ((struct f_inst_roa_check *) what)->rtc;
    if (!rtc->table)
      runtime("Missing ROA table");

    res.type = T_ENUM_ROA;
    res.val.i = roa_check(rtc->table, v1.val.px.ip, v1.val.px.len, as);
    break;

  default:
    bug( "Unknown instruction %d (%c)", what->code, what->code & 0xff);
  }
  if (what->next)
    return interpret(what->next);
  return res;
}
