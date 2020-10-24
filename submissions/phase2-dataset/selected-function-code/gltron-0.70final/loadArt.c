void loadArt(void) {
  char *path;
  char *artpack;

	runScript(PATH_SCRIPTS, "artpack.lua"); // load default art settings

	scripting_GetGlobal("settings", "current_artpack", NULL);
  scripting_GetStringResult(&artpack);
  fprintf(stderr, "[status] loading artpack '%s'\n", artpack);
	
  path = getArtPath(artpack, "artpack.lua");
  free(artpack);

  if(path != NULL) {
    scripting_RunFile(path);
    free(path);
  }

  initTexture(gScreen);
  initFonts();
}
