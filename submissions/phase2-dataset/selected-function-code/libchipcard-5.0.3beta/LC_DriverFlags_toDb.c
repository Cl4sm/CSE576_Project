int LC_DriverFlags_toDb(GWEN_DB_NODE *db, const char *name,
                        uint32_t flags) {
  GWEN_DB_DeleteVar(db, name);
  if (flags & LC_DRIVER_FLAGS_AUTO)
    if (GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT, name, "auto"))
      return -1;
  if (flags & LC_DRIVER_FLAGS_REMOTE)
    if (GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT, name, "remote"))
      return -1;
  if (flags & LC_DRIVER_FLAGS_CONFIG)
    if (GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT, name, "config"))
      return -1;
  if (flags & LC_DRIVER_FLAGS_HAS_VERIFY_FN)
    if (GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT, name,
                             "has_verify_fn"))
      return -1;
  if (flags & LC_DRIVER_FLAGS_HAS_MODIFY_FN)
    if (GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT, name,
                             "has_modify_fn"))
      return -1;

  return 0;
}