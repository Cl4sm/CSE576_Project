void LC_Client__SampleXmlFiles(const char *where,
                               GWEN_STRINGLIST *sl) {
  GWEN_BUFFER *buf;
  GWEN_DIRECTORY *d;
  unsigned int dpos;

  buf=GWEN_Buffer_new(0, 256, 0, 1);

  d=GWEN_Directory_new();
  GWEN_Buffer_AppendString(buf, where);
  DBG_DEBUG(LC_LOGDOMAIN, "Sampling files in \"%s\"",
            GWEN_Buffer_GetStart(buf));
  dpos=GWEN_Buffer_GetPos(buf);
  if (!GWEN_Directory_Open(d, GWEN_Buffer_GetStart(buf))) {
    char buffer[256];
    unsigned int i;
    GWEN_STRINGLIST *slDirs;
    GWEN_STRINGLISTENTRY *se;

    slDirs=GWEN_StringList_new();
    while (!GWEN_Directory_Read(d, buffer, sizeof(buffer))){
      if (strcmp(buffer, ".")!=0 &&
          strcmp(buffer, "..")!=0) {
        struct stat st;

        GWEN_Buffer_Crop(buf, 0, dpos);
        GWEN_Buffer_SetPos(buf, dpos);
        GWEN_Buffer_AppendByte(buf, '/');
        GWEN_Buffer_AppendString(buf, buffer);
        DBG_DEBUG(LC_LOGDOMAIN, "Checking file \"%s\"",
                  GWEN_Buffer_GetStart(buf));
  
        if (stat(GWEN_Buffer_GetStart(buf), &st)) {
          DBG_ERROR(LC_LOGDOMAIN, "stat(%s): %s",
                    GWEN_Buffer_GetStart(buf),
                    strerror(errno));
        }
        else {
          if (S_ISDIR(st.st_mode)) {
            /* it is a folder, dive into it later */
            GWEN_StringList_AppendString(slDirs,
                                         GWEN_Buffer_GetStart(buf),
                                         0, 1);
          }
          else {
            i=strlen(buffer);
            if (i>3) {
              if (strcasecmp(buffer+i-4, ".xml")==0) {
                DBG_INFO(LC_LOGDOMAIN, "Adding xml file \"%s\"",
                         GWEN_Buffer_GetStart(buf));
                GWEN_StringList_AppendString(sl,
                                             GWEN_Buffer_GetStart(buf),
                                             0, 1);
              } /* if name ends in ".xml" */
            } /* if name longer than 3 chars */
          } /* if it is not a folder */
        } /* if stat succeeded */
      } /* if not a special file/folder */
    } /* while */
    GWEN_Directory_Close(d);

    /* now read subfolders */
    se=GWEN_StringList_FirstEntry(slDirs);
    while(se) {
      LC_Client__SampleXmlFiles(GWEN_StringListEntry_Data(se), sl);
      se=GWEN_StringListEntry_Next(se);
    }
    GWEN_StringList_free(slDirs);

  } /* if open succeeded */
  else {
    DBG_DEBUG(LC_LOGDOMAIN, "Could not open dir \"%s\"",
	      GWEN_Buffer_GetStart(buf));
  }
  GWEN_Directory_free(d);
  GWEN_Buffer_free(buf);
}