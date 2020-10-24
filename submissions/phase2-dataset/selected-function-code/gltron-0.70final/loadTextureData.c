  texture *tex = NULL;
  char *path;
  char *artpack;
  
	scripting_GetGlobal("settings", "current_artpack", NULL);
  scripting_GetStringResult(&artpack);
  path = getArtPath(artpack, filename);
  free(artpack);
  if(path != NULL) {
    tex = LOAD_TEX(path);
    free(path);
  }

  if(tex == NULL) {    
    fprintf(stderr, "fatal: failed load %s, exiting...\n", filename);
    exit(1); /* OK: critical, installation corrupt */
  }
  return tex;
}
