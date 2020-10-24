pseudo_clear(void)
{
  dlink_node *ptr = NULL, *ptr_next = NULL;

  DLINK_FOREACH_SAFE(ptr, ptr_next, pseudo_cmd_list.head)
  {
    struct pseudo_cmd *cmd = ptr->data;
    assert(find_command(cmd->msg.cmd));

    mod_del_cmd(&cmd->msg);
    dlinkDelete(&cmd->node, &pseudo_cmd_list);

    MyFree(cmd->name);
    MyFree(cmd->nick);
    MyFree(cmd->serv);
    MyFree(cmd->prep);
    MyFree(cmd->command);
    MyFree(cmd);
  }
}
