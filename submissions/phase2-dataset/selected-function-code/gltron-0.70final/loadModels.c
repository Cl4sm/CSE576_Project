static void loadModels(void) {
	char *path;
	int i;
	/* load recognizer model */
	path = getPath(PATH_DATA, "recognizer.obj");
	if(path != NULL) {
		recognizer = readMeshFromFile(path, TRI_MESH);
		/* old code did normalize & invert normals & rescale to size = 60 */
	} else {
		fprintf(stderr, "fatal: could not load %s - exiting...\n", path);
		exit(1); /* OK: critical, installation corrupt */
	}
	free(path);
 
	/* load recognizer quad model (for recognizer outlines) */
	path = getPath(PATH_DATA, "recognizer_quad.obj");
	if(path != NULL) {
		recognizer_quad = readMeshFromFile(path, QUAD_MESH);
		/* old code did normalize & invert normals & rescale to size = 60 */
	} else {
		fprintf(stderr, "fatal: could not load %s - exiting...\n", path);
		exit(1); /* OK: critical, installation corrupt */
	}
	free(path);

	/* load lightcycle models */
	for(i = 0; i < LC_LOD; i++) {
		path = getPath(PATH_DATA, lc_lod_names[i]);
		if(path != NULL) {
			lightcycle[i] = readMeshFromFile(path, TRI_MESH);
		} else {
			fprintf(stderr, "fatal: could not load model %s - exiting...\n", lc_lod_names[i]);
			exit(1); /* OK: critical, installation corrupt */
		}
	}
	free(path);
}
