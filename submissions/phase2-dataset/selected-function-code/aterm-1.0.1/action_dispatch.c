int
action_dispatch(action_t * action)
{
#ifdef MENUBAR
    switch (action->type) {
    case MenuTerminalAction:
	cmd_write(action->str, action->len);
	break;

    case MenuAction:
	tt_write(action->str, action->len);
	break;

    default:
	return -1;
	break;
    }
#endif
    return 0;
}
