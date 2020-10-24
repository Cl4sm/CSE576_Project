int LC_DriverInfo_ReadDrivers(const char *dataDir,
                              GWEN_DB_NODE *dbDrivers,
			      int availOnly,
			      int dontSearchDrivers) {
  GWEN_BUFFER *buf;
  GWEN_DIRECTORY *d;
  unsigned int dpos;
  int drivers=0;

  buf=GWEN_Buffer_new(0, 256, 0, 1);

  d=GWEN_Directory_new();
  GWEN_Buffer_AppendString(buf, dataDir);

  dpos=GWEN_Buffer_GetPos(buf);
  if (!GWEN_Directory_Open(d, GWEN_Buffer_GetStart(buf))) {
    char buffer[256];

    while (!GWEN_Directory_Read(d, buffer, sizeof(buffer))){
      struct stat st;
      int flen;

      flen=strlen(buffer);
      GWEN_Buffer_Crop(buf, 0, dpos);
      GWEN_Buffer_SetPos(buf, dpos);
      GWEN_Buffer_AppendByte(buf, '/');
      GWEN_Buffer_AppendString(buf, buffer);
      if (stat(GWEN_Buffer_GetStart(buf), &st)) {
        DBG_ERROR(0, "stat(%s): %s",
                  GWEN_Buffer_GetStart(buf),
                  strerror(errno));
      }
      else {
        if (!S_ISDIR(st.st_mode)) {
          if (flen>3) {
            if (strncasecmp(buffer+flen-4, ".xml", 4)==0) {
              DBG_DEBUG(0, "Reading driver file \"%s\"",
                        GWEN_Buffer_GetStart(buf));
              if (!LC_DriverInfo_ReadDriverFile(GWEN_Buffer_GetStart(buf),
						dbDrivers,
						availOnly,
						dontSearchDrivers))
                drivers++;
            }
          }
        } /* if it is not a folder */
      } /* if stat succeeded */
    } /* while */
  } /* if open succeeded */
  else {
    DBG_INFO(LC_LOGDOMAIN,
	     "Could not open folder [%s]",
	     GWEN_Buffer_GetStart(buf));
  }
  GWEN_Directory_Close(d);
  GWEN_Directory_free(d);
  GWEN_Buffer_free(buf);

  return 0;
}