void LC_NotifyFlags_toDb(GWEN_DB_NODE *db,
                         const char *name,
                         uint32_t fl) {
  assert(db);
  assert(name);
  GWEN_DB_DeleteVar(db, name);
  if (fl & LC_NOTIFY_FLAGS_DRIVER_START)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "DriverStart");
  if (fl & LC_NOTIFY_FLAGS_DRIVER_UP)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "DriverUp");
  if (fl & LC_NOTIFY_FLAGS_DRIVER_DOWN)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "DriverDown");
  if (fl & LC_NOTIFY_FLAGS_DRIVER_ERROR)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "DriverError");

  if (fl & LC_NOTIFY_FLAGS_SERVICE_START)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "ServiceStart");
  if (fl & LC_NOTIFY_FLAGS_SERVICE_UP)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "ServiceUp");
  if (fl & LC_NOTIFY_FLAGS_SERVICE_DOWN)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "ServiceDown");
  if (fl & LC_NOTIFY_FLAGS_SERVICE_ERROR)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "ServiceError");

  if (fl & LC_NOTIFY_FLAGS_CARD_INSERTED)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "CardInserted");
  if (fl & LC_NOTIFY_FLAGS_CARD_REMOVED)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "CardRemoved");

  if (fl & LC_NOTIFY_FLAGS_CLIENT_UP)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "ClientUp");
  if (fl & LC_NOTIFY_FLAGS_CLIENT_DOWN)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "ClientDown");
  if (fl & LC_NOTIFY_FLAGS_CLIENT_STARTWAIT)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "ClientStartWait");
  if (fl & LC_NOTIFY_FLAGS_CLIENT_STOPWAIT)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "ClientStopWait");
  if (fl & LC_NOTIFY_FLAGS_CLIENT_TAKECARD)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "ClientTakeCard");
  if (fl & LC_NOTIFY_FLAGS_CLIENT_GOTCARD)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "ClientGotCard");
  if (fl & LC_NOTIFY_FLAGS_CLIENT_CMDSEND)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "ClientCmdSend");
  if (fl & LC_NOTIFY_FLAGS_CLIENT_CMDRECV)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "ClientCmdRecv");

  if (fl & LC_NOTIFY_FLAGS_SINGLESHOT)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "SingleShot");
}