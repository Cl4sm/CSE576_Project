int LC_GeldKarte_BLog_toDb(const LC_GELDKARTE_BLOG *st, GWEN_DB_NODE *db) {
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
  if (st->merchantId)
    if (GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "merchantId", st->merchantId))
      return -1;
  if (GWEN_DB_SetIntValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "hSeq", st->hSeq))
    return -1;
  if (GWEN_DB_SetIntValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "sSeq", st->sSeq))
    return -1;
  if (GWEN_DB_SetIntValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "loaded", st->loaded))
    return -1;
  if (st->time)
    if (GWEN_Time_toDb(st->time, GWEN_DB_GetGroup(db, GWEN_DB_FLAGS_DEFAULT, "time")))
      return -1;
  if (GWEN_DB_SetIntValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "keyId", st->keyId))
    return -1;
  return 0;
}