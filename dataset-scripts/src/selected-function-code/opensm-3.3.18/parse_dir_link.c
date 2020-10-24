static
bool parse_dir_link(int c_dir, struct torus *t, const char *parse_sep)
{
	guid_t sw_guid0, sw_guid1;
	struct link *l;
	bool success = false;

	if (!parse_guid(t, &sw_guid0, parse_sep))
		goto out;

	if (!parse_guid(t, &sw_guid1, parse_sep))
		goto out;

	if (!t) {
		success = true;
		goto out;
	}

	switch (c_dir) {
	case -1:
		l = &t->seed[t->seed_cnt - 1].xm_link;
		break;
	case  1:
		l = &t->seed[t->seed_cnt - 1].xp_link;
		break;
	case -2:
		l = &t->seed[t->seed_cnt - 1].ym_link;
		break;
	case  2:
		l = &t->seed[t->seed_cnt - 1].yp_link;
		break;
	case -3:
		l = &t->seed[t->seed_cnt - 1].zm_link;
		break;
	case  3:
		l = &t->seed[t->seed_cnt - 1].zp_link;
		break;
	default:
		OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
			"ERR 4E0E: unknown link direction %d\n", c_dir);
		goto out;
	}
	l->end[0].type = PASSTHRU;
	l->end[0].port = -1;		/* We don't really connect. */
	l->end[0].n_id = sw_guid0;
	l->end[0].sw = NULL;		/* Fix this up later. */
	l->end[0].link = NULL;		/* Fix this up later. */

	l->end[1].type = PASSTHRU;
	l->end[1].port = -1;		/* We don't really connect. */
	l->end[1].n_id = sw_guid1;
	l->end[1].sw = NULL;		/* Fix this up later. */
	l->end[1].link = NULL;		/* Fix this up later. */

	success = true;
out:
	return success;
}