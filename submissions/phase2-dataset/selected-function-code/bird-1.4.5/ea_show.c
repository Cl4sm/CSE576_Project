void
ea_show(struct cli *c, eattr *e)
{
  struct protocol *p;
  int status = GA_UNKNOWN;
  struct adata *ad = (e->type & EAF_EMBEDDED) ? NULL : e->u.ptr;
  byte buf[CLI_MSG_SIZE];
  byte *pos = buf, *end = buf + sizeof(buf);

  if (p = attr_class_to_protocol[EA_PROTO(e->id)])
    {
      pos += bsprintf(pos, "%s.", p->name);
      if (p->get_attr)
	status = p->get_attr(e, pos, end - pos);
      pos += strlen(pos);
    }
  else if (EA_PROTO(e->id))
    pos += bsprintf(pos, "%02x.", EA_PROTO(e->id));
  else 
    status = get_generic_attr(e, &pos, end - pos);

  if (status < GA_NAME)
    pos += bsprintf(pos, "%02x", EA_ID(e->id));
  if (status < GA_FULL)
    {
      *pos++ = ':';
      *pos++ = ' ';
      switch (e->type & EAF_TYPE_MASK)
	{
	case EAF_TYPE_INT:
	  bsprintf(pos, "%u", e->u.data);
	  break;
	case EAF_TYPE_OPAQUE:
	  opaque_format(ad, pos, end - pos);
	  break;
	case EAF_TYPE_IP_ADDRESS:
	  bsprintf(pos, "%I", *(ip_addr *) ad->data);
	  break;
	case EAF_TYPE_ROUTER_ID:
	  bsprintf(pos, "%R", e->u.data);
	  break;
	case EAF_TYPE_AS_PATH:
	  as_path_format(ad, pos, end - pos);
	  break;
	case EAF_TYPE_INT_SET:
	  ea_show_int_set(c, ad, 1, pos, buf, end);
	  return;
	case EAF_TYPE_EC_SET:
	  ea_show_ec_set(c, ad, pos, buf, end);
	  return;
	case EAF_TYPE_UNDEF:
	default:
	  bsprintf(pos, "<type %02x>", e->type);
	}
    }
  cli_printf(c, -1012, "\t%s", buf);
}
