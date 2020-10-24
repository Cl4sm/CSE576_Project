pm_format(struct f_path_mask *p, buffer *buf)
{
  buffer_puts(buf, "[= ");

  while (p)
  {
    switch(p->kind)
    {
    case PM_ASN:
      buffer_print(buf, "%u ", p->val);
      break;

    case PM_QUESTION:
      buffer_puts(buf, "? ");
      break;

    case PM_ASTERISK:
      buffer_puts(buf, "* ");
      break;

    case PM_ASN_EXPR:
      buffer_print(buf, "%u ", f_eval_asn((struct f_inst *) p->val));
      break;
    }

    p = p->next;
  }

  buffer_puts(buf, "=]");
}
