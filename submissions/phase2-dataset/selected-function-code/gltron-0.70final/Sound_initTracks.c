  const char *music_path;
  List *soundList;
  List *p;
  int i;

  music_path = getDirectory( PATH_MUSIC );
	soundList = readDirectoryContents(music_path, NULL);
  if(soundList->next == NULL) {
    fprintf(stderr, "[sound] no music files found...exiting\n");
    exit(1); // FIXME: handle missing songs somewhere else
  }
    
  i = 1;
  for(p = soundList; p->next != NULL; p = p->next) {
    
    // bugfix: filter track list to readable files (and without directories)
    char *path = getPossiblePath( PATH_MUSIC, (char*)p->data );
  	if( path != NULL && fileExists( path ) ) {
    	scripting_RunFormat("tracks[%d] = \"%s\"", i, (char*) p->data);
        i++;
    	free( path );
    }
  }
  scripting_Run("setupSoundTrack()");
}
