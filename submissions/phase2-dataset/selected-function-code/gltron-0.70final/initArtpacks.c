  const char *art_path;
  List *artList;
  List *p;
  int i;

  art_path = getDirectory( PATH_ART );
  artList = readDirectoryContents(art_path, NULL);
  if(artList->next == NULL) {
    fprintf(stderr, "[fatal] no art files found...exiting\n");
    exit(1); /* OK: critical, installation corrupt */
  }
  
  i = 1;
  for(p = artList; p->next != NULL; p = p->next) {
    if(strncmp((char*)p->data, "Makefile", 8)) {
      scripting_RunFormat("artpacks[%d] = \"%s\"", i, (char*) p->data);
      i++;
    }
  }
  scripting_Run("setupArtpacks()");
}
