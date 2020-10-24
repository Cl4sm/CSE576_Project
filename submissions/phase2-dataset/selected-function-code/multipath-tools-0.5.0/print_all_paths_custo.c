print_all_paths_custo (vector pathvec, int banner, char *fmt)
{
	int i;
	struct path * pp;
	char line[MAX_LINE_LEN];

	if (!VECTOR_SIZE(pathvec)) {
		if (banner)
			fprintf(stdout, "===== no paths =====\n");
		return;
	}

	if (banner)
		fprintf(stdout, "===== paths list =====\n");

	get_path_layout(pathvec, 1);
	snprint_path_header(line, MAX_LINE_LEN, fmt);
	fprintf(stdout, "%s", line);

	vector_foreach_slot (pathvec, pp, i)
		print_path(pp, fmt);
}
