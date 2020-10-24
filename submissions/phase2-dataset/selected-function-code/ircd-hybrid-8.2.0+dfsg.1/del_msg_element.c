del_msg_element(struct MessageTree *mtree_p, const char *cmd)
{
  struct MessageTree *ntree_p = NULL;

  /*
   * In case this is called for a nonexistent command
   * check that there is a msg pointer here, else links-- goes -ve
   * -db
   */
  if (*cmd == '\0' && mtree_p->msg)
  {
    mtree_p->msg = NULL;
    mtree_p->links--;
  }
  else
  {
    if ((ntree_p = mtree_p->pointers[*cmd & (MAXPTRLEN - 1)]))
    {
      del_msg_element(ntree_p, cmd + 1);

      if (ntree_p->links == 0)
      {
        mtree_p->pointers[*cmd & (MAXPTRLEN - 1)] = NULL;
        mtree_p->links--;
        MyFree(ntree_p);
      }
    }
  }
}
