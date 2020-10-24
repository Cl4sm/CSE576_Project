{
  /* load some more defaults from config file */
	runScript(PATH_SCRIPTS, "config.lua");
	runScript(PATH_SCRIPTS, "artpack.lua");
	
  /* go for .gltronrc (or whatever is defined in RC_NAME) */
  {
    char *path;
    path = getPossiblePath(PATH_PREFERENCES, RC_NAME);
    if (path != NULL) {
      if (fileExists(path)) {
        printf("[status] loading settings from %s\n", path);
	      scripting_RunFile(path);
      } else {
	      printf("[error] cannot load %s from %s\n", RC_NAME, path);
      }
      free(path);
    }
    else {
      printf("[fatal] can't get valid pref path for %s\n", RC_NAME);
      exit(1); // something is seriously wrong
    }
  }
	
	if(!isSetting("version") || getSettingf("version") < 0.70f) {
		/* load some more defaults from config file */
		runScript(PATH_SCRIPTS, "config.lua");
		runScript(PATH_SCRIPTS, "artpack.lua");
		printf("[warning] old config file found, overriding using defaults\n");
	}
	// check if config is valid
	scripting_GetGlobal("save_completed", NULL);
	if(scripting_IsNilResult()) {
		runScript(PATH_SCRIPTS, "config.lua");
		runScript(PATH_SCRIPTS, "artpack.lua");
		printf("[warning] defunct config file found, overriding using defaults\n");
	}
		
	setSettingf("version", 0.70f);

  /* parse any comandline switches overrinding the loaded settings */
  parse_args(argc, argv);

  /* sanity check some settings */
  checkSettings();
	
  /* intialize the settings cache, remember to do that everytime you
     change something */
  updateSettingsCache();
}
