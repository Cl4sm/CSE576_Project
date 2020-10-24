static void priority_parse(char **p_last, osm_opensm_t * p_osm, FILE * out)
{
	char *p_cmd;
	int priority;

	p_cmd = next_token(p_last);
	if (!p_cmd)
		fprintf(out, "Current sm-priority is %d\n",
			p_osm->subn.opt.sm_priority);
	else {
		priority = strtol(p_cmd, NULL, 0);
		if (0 > priority || 15 < priority)
			fprintf(out,
				"Invalid sm-priority %d; must be between 0 and 15\n",
				priority);
		else {
			fprintf(out, "Setting sm-priority to %d\n", priority);
			osm_set_sm_priority(&p_osm->sm, (uint8_t)priority);
		}
	}
}