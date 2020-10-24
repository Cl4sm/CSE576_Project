int LC_GeldKarte_BLog_ReadDb(LC_GELDKARTE_BLOG *st, GWEN_DB_NODE *db) {
  assert(st);
  assert(db);
  LC_GeldKarte_BLog_SetStatus(st, GWEN_DB_GetIntValue(db, "status", 0, 0));
  LC_GeldKarte_BLog_SetBSeq(st, GWEN_DB_GetIntValue(db, "bSeq", 0, 0));
  LC_GeldKarte_BLog_SetLSeq(st, GWEN_DB_GetIntValue(db, "lSeq", 0, 0));
  LC_GeldKarte_BLog_SetValue(st, GWEN_DB_GetIntValue(db, "value", 0, 0));
  LC_GeldKarte_BLog_SetMerchantId(st, GWEN_DB_GetCharValue(db, "merchantId", 0, 0));
  LC_GeldKarte_BLog_SetHSeq(st, GWEN_DB_GetIntValue(db, "hSeq", 0, 0));
  LC_GeldKarte_BLog_SetSSeq(st, GWEN_DB_GetIntValue(db, "sSeq", 0, 0));
  LC_GeldKarte_BLog_SetLoaded(st, GWEN_DB_GetIntValue(db, "loaded", 0, 0));
  if (1) { /* for local vars */
    GWEN_DB_NODE *dbT;

    dbT=GWEN_DB_GetGroup(db, GWEN_PATH_FLAGS_NAMEMUSTEXIST, "time");
    if (dbT) {
  if (st->time)
    GWEN_Time_free(st->time);
  st->time=GWEN_Time_fromDb(dbT);
}
  }
  LC_GeldKarte_BLog_SetKeyId(st, GWEN_DB_GetIntValue(db, "keyId", 0, 0));
  return 0;
}