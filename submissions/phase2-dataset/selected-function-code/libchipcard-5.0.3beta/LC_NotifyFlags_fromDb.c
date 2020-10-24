uint32_t LC_NotifyFlags_fromDb(GWEN_DB_NODE *db, const char *name){
  const char *p;
  uint32_t flags=0;
  int i;

  for (i=0; ; i++) {
    p=GWEN_DB_GetCharValue(db, name, i, 0);
    if (!p)
      break;

    if (strcasecmp(p, "DriverStart")==0)
      flags|=LC_NOTIFY_FLAGS_DRIVER_START;
    else if (strcasecmp(p, "DriverUp")==0)
      flags|=LC_NOTIFY_FLAGS_DRIVER_UP;
    else if (strcasecmp(p, "DriverDown")==0)
      flags|=LC_NOTIFY_FLAGS_DRIVER_DOWN;
    else if (strcasecmp(p, "DriverError")==0)
      flags|=LC_NOTIFY_FLAGS_DRIVER_ERROR;

    else if (strcasecmp(p, "ReaderStart")==0)
      flags|=LC_NOTIFY_FLAGS_READER_START;
    else if (strcasecmp(p, "ReaderUp")==0)
      flags|=LC_NOTIFY_FLAGS_READER_UP;
    else if (strcasecmp(p, "ReaderDown")==0)
      flags|=LC_NOTIFY_FLAGS_READER_DOWN;
    else if (strcasecmp(p, "ReaderError")==0)
      flags|=LC_NOTIFY_FLAGS_READER_ERROR;

    else if (strcasecmp(p, "ServiceStart")==0)
      flags|=LC_NOTIFY_FLAGS_SERVICE_START;
    else if (strcasecmp(p, "ServiceUp")==0)
      flags|=LC_NOTIFY_FLAGS_SERVICE_UP;
    else if (strcasecmp(p, "ServiceDown")==0)
      flags|=LC_NOTIFY_FLAGS_SERVICE_DOWN;
    else if (strcasecmp(p, "ServiceError")==0)
      flags|=LC_NOTIFY_FLAGS_SERVICE_ERROR;

    else if (strcasecmp(p, "CardInserted")==0)
      flags|=LC_NOTIFY_FLAGS_CARD_INSERTED;
    else if (strcasecmp(p, "CardRemoved")==0)
      flags|=LC_NOTIFY_FLAGS_CARD_REMOVED;

    else if (strcasecmp(p, "ClientUp")==0)
      flags|=LC_NOTIFY_FLAGS_CLIENT_UP;
    else if (strcasecmp(p, "ClientDown")==0)
      flags|=LC_NOTIFY_FLAGS_CLIENT_DOWN;
    else if (strcasecmp(p, "ClientStartWait")==0)
      flags|=LC_NOTIFY_FLAGS_CLIENT_STARTWAIT;
    else if (strcasecmp(p, "ClientStopWait")==0)
      flags|=LC_NOTIFY_FLAGS_CLIENT_STOPWAIT;
    else if (strcasecmp(p, "ClientTakeCard")==0)
      flags|=LC_NOTIFY_FLAGS_CLIENT_TAKECARD;
    else if (strcasecmp(p, "ClientGotCard")==0)
      flags|=LC_NOTIFY_FLAGS_CLIENT_GOTCARD;

    else if (strcasecmp(p, "ClientCmdSend")==0)
      flags|=LC_NOTIFY_FLAGS_CLIENT_CMDSEND;
    else if (strcasecmp(p, "ClientCmdRecv")==0)
      flags|=LC_NOTIFY_FLAGS_CLIENT_CMDRECV;

    else if (strcasecmp(p, "SingleShot")==0)
      flags|=LC_NOTIFY_FLAGS_SINGLESHOT;

    else {
      DBG_WARN(0, "Unknown flag \"%s\", ignoring", p);
    }
  } /* for */

  return flags;
}