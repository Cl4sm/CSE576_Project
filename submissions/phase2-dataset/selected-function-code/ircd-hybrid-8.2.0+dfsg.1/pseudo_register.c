pseudo_register(const char *name, const char *nick,
                const char *serv, const char *prep,
                const char *command)
{
  struct pseudo_cmd *cmd = NULL;

  if (find_command(command))
    return;

  cmd = MyCalloc(sizeof(*cmd));
  cmd->name = xstrdup(name);
  cmd->nick = xstrdup(nick);
  cmd->serv = xstrdup(serv);
  cmd->prep = xstrdup(prep);
  cmd->command = xstrdup(command);

  cmd->msg.cmd = cmd->command;
  cmd->msg.args_max = 2;
  cmd->msg.flags = MFLG_EXTRA|MFLG_SLOW;
  cmd->msg.extra = cmd;
  cmd->msg.handlers[UNREGISTERED_HANDLER] = m_unregistered;
  cmd->msg.handlers[CLIENT_HANDLER] = m_pseudo;
  cmd->msg.handlers[SERVER_HANDLER] = m_ignore;
  cmd->msg.handlers[ENCAP_HANDLER] = m_ignore;
  cmd->msg.handlers[OPER_HANDLER] = m_pseudo;
  cmd->msg.handlers[DUMMY_HANDLER] = m_ignore;
  dlinkAdd(cmd, &cmd->node, &pseudo_cmd_list);

  mod_add_cmd(&cmd->msg);
}
