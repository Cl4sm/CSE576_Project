int LC_DriverInfo_FindFile(GWEN_STRINGLIST *slDirs,
                           GWEN_STRINGLIST *slNames,
                           GWEN_BUFFER *nbuf) {
  GWEN_STRINGLISTENTRY *eDirs;

  eDirs=GWEN_StringList_FirstEntry(slDirs);
  while(eDirs) {
    uint32_t pos;
    GWEN_STRINGLISTENTRY *eNames;

    GWEN_Buffer_Reset(nbuf);
    GWEN_Buffer_AppendString(nbuf, GWEN_StringListEntry_Data(eDirs));
    GWEN_Buffer_AppendByte(nbuf, '/');
    pos=GWEN_Buffer_GetPos(nbuf);

    eNames=GWEN_StringList_FirstEntry(slNames);

    while(eNames) {
      GWEN_DIRECTORY *dDir;

      dDir=GWEN_Directory_new();
      if (!GWEN_Directory_Open(dDir, GWEN_StringListEntry_Data(eDirs))) {
        char nameBuf[256];

        /* search for name in this folder */
        while(!GWEN_Directory_Read(dDir, nameBuf, sizeof(nameBuf))) {
          if (strcmp(nameBuf, ".")!=0 &&
              strcmp(nameBuf, "..")!=0) {
            if (-1!=GWEN_Text_ComparePattern(nameBuf,
                                             GWEN_StringListEntry_Data(eNames),
                                             0)) {
              struct stat st;

              /* found name, add it to the buffer */
              GWEN_Buffer_Crop(nbuf, 0, pos);
              GWEN_Buffer_SetPos(nbuf, pos);
              GWEN_Buffer_AppendString(nbuf, nameBuf);
              if (stat(GWEN_Buffer_GetStart(nbuf), &st)) {
                /* error */
                DBG_WARN(0, "stat(%s): %s",
                         GWEN_Buffer_GetStart(nbuf),
                         strerror(errno));
              }
              else {
                /* check for regular file */
                if (S_ISREG(st.st_mode)) {
                  GWEN_Directory_Close(dDir);
                  GWEN_Directory_free(dDir);
                  DBG_DEBUG(0, "File found: %s", GWEN_Buffer_GetStart(nbuf));
                  return 0;
                }
                else {
                  DBG_INFO(0, "Entry \"%s\" is not a regular file",
                           GWEN_Buffer_GetStart(nbuf));
                }
              }
            } /* if name pattern matches */
          } /* if not a special entry */
        } /* while still entries */
        GWEN_Directory_Close(dDir);
      }
      GWEN_Directory_free(dDir);

      eNames=GWEN_StringListEntry_Next(eNames);
    } /* while eNames */

    eDirs=GWEN_StringListEntry_Next(eDirs);
  } /* while eDirs */

  DBG_DEBUG(0, "File not found in search paths");
  return -1;
}