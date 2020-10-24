add_msg_element(struct MessageTree *mtree_p, struct Message *msg_p,
                const char *cmd)
{
  struct MessageTree *ntree_p = NULL;

  if (*cmd == '\0')
  {
    mtree_p->msg = msg_p;
    mtree_p->links++;  /* Have msg pointer, so up ref count */
  }
  else
  {
    /*
     * *cmd & (MAXPTRLEN-1)
     * convert the char pointed to at *cmd from ASCII to an integer
     * between 0 and MAXPTRLEN.
     * Thus 'A' -> 0x1 'B' -> 0x2 'c' -> 0x3 etc.
     */
    if ((ntree_p = mtree_p->pointers[*cmd & (MAXPTRLEN - 1)]) == NULL)
    {
      ntree_p = MyCalloc(sizeof(struct MessageTree));
      mtree_p->pointers[*cmd & (MAXPTRLEN - 1)] = ntree_p;

      mtree_p->links++;  /* Have new pointer, so up ref count */
    }

    add_msg_element(ntree_p, msg_p, cmd + 1);
  }
}
