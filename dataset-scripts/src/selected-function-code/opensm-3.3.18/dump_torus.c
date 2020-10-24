static
void dump_torus(struct torus *t)
{
	unsigned i, j, k;
	unsigned x_sz = t->x_sz;
	unsigned y_sz = t->y_sz;
	unsigned z_sz = t->z_sz;
	char path[1024];
	FILE *file;

	snprintf(path, sizeof(path), "%s/%s", t->osm->subn.opt.dump_files_dir,
		 "opensm-torus.dump");
	file = fopen(path, "w");
	if (!file) {
		OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
			"ERR 4E47: cannot create file \'%s\'\n", path);
		return;
	}

	for (k = 0; k < z_sz; k++)
		for (j = 0; j < y_sz; j++)
			for (i = 0; i < x_sz; i++)
				if (t->sw[i][j][k])
					fprintf(file, "switch %u,%u,%u GUID 0x%04"
						PRIx64 " (%s)\n",
						i, j, k,
						cl_ntoh64(t->sw[i][j][k]->n_id),
						t->sw[i][j][k]->osm_switch->p_node->print_desc);
	fclose(file);
}