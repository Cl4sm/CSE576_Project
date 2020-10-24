int LC_GeldKarte_LLog_toDb(const LC_GELDKARTE_LLOG *st, GWEN_DB_NODE *db) {
  assert(st);
  assert(db);
  if (GWEN_DB_SetIntValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "status", st->status))
    return -1;
  if (GWEN_DB_SetIntValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "bSeq", st->bSeq))
    return -1;
  if (GWEN_DB_SetIntValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "lSeq", st->lSeq))
    return -1;
  if (GWEN_DB_SetIntValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "value", st->value))
    return -1;
  if (st->centerId)
    if (GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "centerId", st->centerId))
      return -1;
  if (st->terminalId)
    if (GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "terminalId", st->terminalId))
      return -1;
  if (st->traceId)
    if (GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "traceId", st->traceId))
      return -1;
  if (GWEN_DB_SetIntValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "loaded", st->loaded))
    return -1;
  if (st->time)
    if (GWEN_Time_toDb(st->time, GWEN_DB_GetGroup(db, GWEN_DB_FLAGS_DEFAULT, "time")))
      return -1;
  return 0;
}