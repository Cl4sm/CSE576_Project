void initDirectories(void) {
  if(PREF_DIR[0] != '~')
    sprintf(preferences_dir, PREF_DIR);
  else
    sprintf(preferences_dir, "%s%s", getHome(), PREF_DIR + 1);

  if(SNAP_DIR[0] != '~')
    sprintf(snapshots_dir, SNAP_DIR);
  else
    sprintf(snapshots_dir, "%s%s", getHome(), SNAP_DIR + 1);

#ifdef LOCAL_DATA
  #ifdef macintosh
  sprintf(data_dir, ":data");
  sprintf(art_dir, ":art");
  sprintf(scripts_dir, ":scripts");
  sprintf(music_dir, ":music");
  #else
  sprintf(data_dir, "data");
  sprintf(art_dir, "art");
  sprintf(scripts_dir, "scripts");
  sprintf(music_dir, "music");
  #endif

#else
  sprintf(data_dir, "%s%c%s", DATA_DIR, SEPARATOR, "data");
  sprintf(art_dir, "%s%c%s", DATA_DIR, SEPARATOR, "art");
  sprintf(scripts_dir, "%s%c%s", DATA_DIR, SEPARATOR, "scripts");
  sprintf(music_dir, "%s%c%s", DATA_DIR, SEPARATOR, "music");
#endif

	/*
  printf("directories:\n"
	 "\tprefs: %s\n"
	 "\tsnaps: %s\n"
	 "\tdata:  %s\n"
	 "\tart:   %s\n"
	 "\tscripts:   %s\n"
	 "\tmusic: %s\n",
	 preferences_dir, snapshots_dir, 
	 data_dir, art_dir, scripts_dir, 
	 music_dir);
	*/

  makeDirectory(preferences_dir);
  makeDirectory(snapshots_dir);
}
