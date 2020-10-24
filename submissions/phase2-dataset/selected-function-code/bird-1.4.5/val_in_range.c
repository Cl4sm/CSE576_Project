static int
val_in_range(struct f_val v1, struct f_val v2)
{
  if ((v1.type == T_PATH) && (v2.type == T_PATH_MASK))
    return as_path_match(v1.val.ad, v2.val.path_mask);

  if ((v1.type == T_INT) && (v2.type == T_PATH))
    return as_path_contains(v2.val.ad, v1.val.i, 1);

  if (((v1.type == T_PAIR) || (v1.type == T_QUAD)) && (v2.type == T_CLIST))
    return int_set_contains(v2.val.ad, v1.val.i);
#ifndef IPV6
  /* IP->Quad implicit conversion */
  if ((v1.type == T_IP) && (v2.type == T_CLIST))
    return int_set_contains(v2.val.ad, ipa_to_u32(v1.val.px.ip));
#endif

  if ((v1.type == T_EC) && (v2.type == T_ECLIST))
    return ec_set_contains(v2.val.ad, v1.val.ec);

  if ((v1.type == T_STRING) && (v2.type == T_STRING))
    return patmatch(v2.val.s, v1.val.s);

  if ((v1.type == T_IP) && (v2.type == T_PREFIX))
    return ipa_in_net(v1.val.px.ip, v2.val.px.ip, v2.val.px.len);

  if ((v1.type == T_PREFIX) && (v2.type == T_PREFIX))
    return net_in_net(v1.val.px.ip, v1.val.px.len, v2.val.px.ip, v2.val.px.len);

  if ((v1.type == T_PREFIX) && (v2.type == T_PREFIX_SET))
    return trie_match_fprefix(v2.val.ti, &v1.val.px);

  if (v2.type != T_SET)
    return CMP_ERROR;

  /* With integrated Quad<->IP implicit conversion */
  if ((v1.type == v2.val.t->from.type) ||
      ((IP_VERSION == 4) && (v1.type == T_QUAD) && (v2.val.t->from.type == T_IP)))
    return !!find_tree(v2.val.t, v1);

  if (v1.type == T_CLIST)
    return clist_match_set(v1.val.ad, v2.val.t);

  if (v1.type == T_ECLIST)
    return eclist_match_set(v1.val.ad, v2.val.t);

  if (v1.type == T_PATH)
    return as_path_match_set(v1.val.ad, v2.val.t);

  return CMP_ERROR;
}
