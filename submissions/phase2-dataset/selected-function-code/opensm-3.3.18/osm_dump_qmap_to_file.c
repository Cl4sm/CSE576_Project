void osm_dump_qmap_to_file(osm_opensm_t * p_osm, const char *file_name,
			   cl_qmap_t * map,
			   void (*func) (cl_map_item_t *, FILE *, void *),
			   void *cxt)
{
	char path[1024];
	FILE *file;

	snprintf(path, sizeof(path), "%s/%s",
		 p_osm->subn.opt.dump_files_dir, file_name);

	file = fopen(path, "w");
	if (!file) {
		OSM_LOG(&p_osm->log, OSM_LOG_ERROR,
			"cannot create file \'%s\': %s\n",
			path, strerror(errno));
		return;
	}

	dump_qmap(file, map, func, cxt);

	fclose(file);
}