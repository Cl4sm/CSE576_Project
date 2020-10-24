static int
opensm_dump_to_file(osm_opensm_t * p_osm, const char *file_name,
		    void (*dump_func) (osm_opensm_t * p_osm, FILE * file))
{
	char path[1024];
	char path_tmp[1032];
	FILE *file;
	int fd, status = 0;

	snprintf(path, sizeof(path), "%s/%s",
		 p_osm->subn.opt.dump_files_dir, file_name);

	snprintf(path_tmp, sizeof(path_tmp), "%s.tmp", path);

	file = fopen(path_tmp, "w");
	if (!file) {
		OSM_LOG(&p_osm->log, OSM_LOG_ERROR, "ERR 4C01: "
			"cannot open file \'%s\': %s\n",
			path_tmp, strerror(errno));
		return -1;
	}

	chmod(path_tmp, S_IRUSR | S_IWUSR);

	dump_func(p_osm, file);

	if (p_osm->subn.opt.fsync_high_avail_files) {
		if (fflush(file) == 0) {
			fd = fileno(file);
			if (fd != -1) {
				if (fsync(fd) == -1)
					OSM_LOG(&p_osm->log, OSM_LOG_ERROR,
						"ERR 4C08: fsync() failed (%s) for %s\n",
						strerror(errno), path_tmp);
			} else
				OSM_LOG(&p_osm->log, OSM_LOG_ERROR, "ERR 4C09: "
					"fileno() failed for %s\n", path_tmp);
		} else
			OSM_LOG(&p_osm->log, OSM_LOG_ERROR, "ERR 4C0A: "
				"fflush() failed (%s) for %s\n",
				strerror(errno), path_tmp);
	}

	fclose(file);

	status = rename(path_tmp, path);
	if (status) {
		OSM_LOG(&p_osm->log, OSM_LOG_ERROR, "ERR 4C0B: "
			"Failed to rename file:%s (err:%s)\n",
			path_tmp, strerror(errno));
	}

	return status;
}