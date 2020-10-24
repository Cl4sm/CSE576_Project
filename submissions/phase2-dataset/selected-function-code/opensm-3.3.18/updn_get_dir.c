static updn_switch_dir_t updn_get_dir(unsigned cur_rank, unsigned rem_rank,
				      uint64_t cur_id, uint64_t rem_id)
{
	/* HACK: comes to solve root nodes connection, in a classic subnet root nodes do not connect
	   directly, but in case they are we assign to root node an UP direction to allow UPDN to discover
	   the subnet correctly (and not from the point of view of the last root node).
	 */
	if (!cur_rank && !rem_rank)
		return UP;

	if (cur_rank < rem_rank)
		return DOWN;
	else if (cur_rank > rem_rank)
		return UP;
	else {
		/* Equal rank, decide by id number, bigger == UP direction */
		if (cur_id > rem_id)
			return UP;
		else
			return DOWN;
	}
}