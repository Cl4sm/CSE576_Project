extern int
snprint_status (char * buff, int len, struct vectors *vecs)
{
	int fwd = 0;
	int i;
	unsigned int count[PATH_MAX_STATE] = {0};
	struct path * pp;

	vector_foreach_slot (vecs->pathvec, pp, i) {
		count[pp->state]++;
	}
	fwd += snprintf(buff + fwd, len - fwd, "path checker states:\n");
	for (i=0; i<PATH_MAX_STATE; i++) {
		if (!count[i])
			continue;
		fwd += snprintf(buff + fwd, len - fwd, "%-20s%u\n",
				checker_state_name(i), count[i]);
	}

        int monitored_count = 0;

        vector_foreach_slot(vecs->pathvec, pp, i)
                if (pp->fd != -1)
                        monitored_count++;
        fwd += snprintf(buff + fwd, len - fwd, "\npaths: %d\nbusy: %s\n",
			monitored_count, is_uevent_busy()? "True" : "False");

	if (fwd > len)
		return len;
	return fwd;
}
