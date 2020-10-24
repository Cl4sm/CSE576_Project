int
val_compare(struct f_val v1, struct f_val v2)
{
  int rc;

  if (v1.type != v2.type) {
    if (v1.type == T_VOID)	/* Hack for else */
      return -1;
    if (v2.type == T_VOID)
      return 1;

#ifndef IPV6
    /* IP->Quad implicit conversion */
    if ((v1.type == T_QUAD) && (v2.type == T_IP))
      return uint_cmp(v1.val.i, ipa_to_u32(v2.val.px.ip));
    if ((v1.type == T_IP) && (v2.type == T_QUAD))
      return uint_cmp(ipa_to_u32(v1.val.px.ip), v2.val.i);
#endif

    debug( "Types do not match in val_compare\n" );
    return CMP_ERROR;
  }

  switch (v1.type) {
  case T_VOID:
    return 0;
  case T_ENUM:
  case T_INT:
  case T_BOOL:
  case T_PAIR:
  case T_QUAD:
    return uint_cmp(v1.val.i, v2.val.i);
  case T_EC:
    return u64_cmp(v1.val.ec, v2.val.ec);
  case T_IP:
    return ipa_compare(v1.val.px.ip, v2.val.px.ip);
  case T_PREFIX:
    if (rc = ipa_compare(v1.val.px.ip, v2.val.px.ip))
      return rc;
    return uint_cmp(v1.val.px.len, v2.val.px.len);
  case T_STRING:
    return strcmp(v1.val.s, v2.val.s);
  default:
    return CMP_ERROR;
  }
}
