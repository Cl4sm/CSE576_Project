int LC_GeldKarte_LLog_ReadDb(LC_GELDKARTE_LLOG *st, GWEN_DB_NODE *db) {
  assert(st);
  assert(db);
  LC_GeldKarte_LLog_SetStatus(st, GWEN_DB_GetIntValue(db, "status", 0, 0));
  LC_GeldKarte_LLog_SetBSeq(st, GWEN_DB_GetIntValue(db, "bSeq", 0, 0));
  LC_GeldKarte_LLog_SetLSeq(st, GWEN_DB_GetIntValue(db, "lSeq", 0, 0));
  LC_GeldKarte_LLog_SetValue(st, GWEN_DB_GetIntValue(db, "value", 0, 0));
  LC_GeldKarte_LLog_SetCenterId(st, GWEN_DB_GetCharValue(db, "centerId", 0, 0));
  LC_GeldKarte_LLog_SetTerminalId(st, GWEN_DB_GetCharValue(db, "terminalId", 0, 0));
  LC_GeldKarte_LLog_SetTraceId(st, GWEN_DB_GetCharValue(db, "traceId", 0, 0));
  LC_GeldKarte_LLog_SetLoaded(st, GWEN_DB_GetIntValue(db, "loaded", 0, 0));
  if (1) { /* for local vars */
    GWEN_DB_NODE *dbT;

    dbT=GWEN_DB_GetGroup(db, GWEN_PATH_FLAGS_NAMEMUSTEXIST, "time");
    if (dbT) {
  if (st->time)
    GWEN_Time_free(st->time);
  st->time=GWEN_Time_fromDb(dbT);
}
  }
  return 0;
}