static int run_state_task(struct state_queue *task)
{
	struct autofs_point *ap;
	enum states next_state, state;
	unsigned long ret = 0;
 
	ap = task->ap;
	next_state = task->state;

	state = ap->state;

	if (next_state != state) {
		switch (next_state) {
		case ST_PRUNE:
			ret = st_prune(ap);
			break;

		case ST_EXPIRE:
			ret = st_expire(ap);
			break;

		case ST_READMAP:
			ret = st_readmap(ap);
			break;

		case ST_SHUTDOWN_PENDING:
			ret = st_prepare_shutdown(ap);
			break;

		case ST_SHUTDOWN_FORCE:
			ret = st_force_shutdown(ap);
			break;

		default:
			error(ap->logopt, "bad next state %d", next_state);
		}
	}

	return ret;
}