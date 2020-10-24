static void change_sequence(xmp_context handle, struct xmp_module_info *mi, struct control *ctl, int i)
{
	int seq = ctl->sequence;

	seq += i;

	/* This should never happen with libxmp 4.0.5 or newer */
	while (mi->seq_data[seq].duration <= 0)
		seq += i;

	if (seq >= mi->num_sequences) {
		seq = 0;
	} else if (seq < 0) {
		seq = mi->num_sequences - 1;

		/* This should never happen with libxmp 4.0.5 or newer */
		while (mi->seq_data[seq].duration <= 0)
			seq--;
	}

	if (seq == ctl->sequence) {
		info_message("Sequence not changed: only one sequence available");
	} else {
		ctl->sequence = seq;
		info_message("Change to sequence %d", seq);
		xmp_set_position(handle, mi->seq_data[seq].entry_point);
	}
}