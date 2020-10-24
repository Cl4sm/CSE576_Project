static void permodlog_parse(char **p_last, osm_opensm_t * p_osm, FILE * out)
{
	FILE *fp;
	char buf[1024];

	if (p_osm->subn.opt.per_module_logging_file != NULL) {
		fp = fopen(p_osm->subn.opt.per_module_logging_file, "r");
		if (!fp) {
			if (errno == ENOENT)
				return;
			fprintf(out, "fopen(%s) failed: %s\n",
				p_osm->subn.opt.per_module_logging_file,
				strerror(errno));
			return;
		}

		fprintf(out, "Per module logging file: %s\n",
			p_osm->subn.opt.per_module_logging_file);
		while (fgets(buf, sizeof buf, fp) != NULL)
			fprintf(out, "%s", buf);
		fclose(fp);
		fprintf(out, "\n");
	}
}