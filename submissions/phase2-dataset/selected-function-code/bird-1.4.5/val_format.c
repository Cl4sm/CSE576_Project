val_format(struct f_val v, buffer *buf)
{
  char buf2[1024];
  switch (v.type)
  {
  case T_VOID:	buffer_puts(buf, "(void)"); return;
  case T_BOOL:	buffer_puts(buf, v.val.i ? "TRUE" : "FALSE"); return;
  case T_INT:	buffer_print(buf, "%u", v.val.i); return;
  case T_STRING: buffer_print(buf, "%s", v.val.s); return;
  case T_IP:	buffer_print(buf, "%I", v.val.px.ip); return;
  case T_PREFIX: buffer_print(buf, "%I/%d", v.val.px.ip, v.val.px.len); return;
  case T_PAIR:	buffer_print(buf, "(%u,%u)", v.val.i >> 16, v.val.i & 0xffff); return;
  case T_QUAD:	buffer_print(buf, "%R", v.val.i); return;
  case T_EC:	ec_format(buf2, v.val.ec); buffer_print(buf, "%s", buf2); return;
  case T_PREFIX_SET: trie_format(v.val.ti, buf); return;
  case T_SET:	tree_format(v.val.t, buf); return;
  case T_ENUM:	buffer_print(buf, "(enum %x)%u", v.type, v.val.i); return;
  case T_PATH:	as_path_format(v.val.ad, buf2, 1000); buffer_print(buf, "(path %s)", buf2); return;
  case T_CLIST:	int_set_format(v.val.ad, 1, -1, buf2, 1000); buffer_print(buf, "(clist %s)", buf2); return;
  case T_ECLIST: ec_set_format(v.val.ad, -1, buf2, 1000); buffer_print(buf, "(eclist %s)", buf2); return;
  case T_PATH_MASK: pm_format(v.val.path_mask, buf); return;
  default:	buffer_print(buf, "[unknown type %x]", v.type); return;
  }
}
