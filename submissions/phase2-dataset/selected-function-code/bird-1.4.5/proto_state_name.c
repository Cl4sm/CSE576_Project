proto_state_name(struct proto *p)
{
#define P(x,y) ((x << 4) | y)
  switch (P(p->proto_state, p->core_state))
    {
    case P(PS_DOWN, FS_HUNGRY):		return "down";
    case P(PS_START, FS_HUNGRY):
    case P(PS_START, FS_HAPPY):		return "start";
    case P(PS_UP, FS_HAPPY):
      switch (p->export_state)
	{
	case ES_DOWN:			return "wait";
	case ES_FEEDING:		return "feed";
	case ES_READY:			return "up";
	default:      			return "???";
	}
    case P(PS_STOP, FS_HUNGRY):
    case P(PS_STOP, FS_FLUSHING):	return "stop";
    case P(PS_DOWN, FS_FLUSHING):	return "flush";
    default:      			return "???";
    }
#undef P
}
