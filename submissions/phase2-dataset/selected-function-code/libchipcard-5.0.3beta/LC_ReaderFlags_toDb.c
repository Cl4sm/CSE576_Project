void LC_ReaderFlags_toDb(GWEN_DB_NODE *db,
                         const char *name,
                         uint32_t fl) {
  assert(db);
  assert(name);
  GWEN_DB_DeleteVar(db, name);
  if (fl & LC_READER_FLAGS_KEYPAD)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "keypad");
  if (fl & LC_READER_FLAGS_DISPLAY)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "display");
  if (fl & LC_READER_FLAGS_NOINFO)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "noinfo");
  if (fl & LC_READER_FLAGS_REMOTE)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "remote");
  if (fl & LC_READER_FLAGS_AUTO)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "auto");
  if (fl & LC_READER_FLAGS_DRIVER_HAS_VERIFY)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "driverHasVerify");
  if (fl & LC_READER_FLAGS_KEEP_RUNNING)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "keepRunning");
  if (fl & LC_READER_FLAGS_LOW_WRITE_BOUNDARY)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
			 name, "lowWriteBoundary");
  if (fl & LC_READER_FLAGS_NO_MEMORY_SW)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
			 name, "noMemorySw");
}