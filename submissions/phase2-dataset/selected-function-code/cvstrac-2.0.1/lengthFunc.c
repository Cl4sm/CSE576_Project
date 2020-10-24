static void lengthFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
  int len;
  switch( sqlite3_value_type(argv[0]) ){
    case SQLITE_BLOB:
    case SQLITE_INTEGER:
    case SQLITE_FLOAT: {
      sqlite3_result_int(context, sqlite3_value_bytes(argv[0]));
      break;
    }
    case SQLITE_TEXT: {
      const char *z = (const char *)sqlite3_value_text(argv[0]);
      mbstate_t st;
      memset(&st, 0, sizeof(st));
      len = (int)mbsrtowcs(NULL, &z, 0, &st);
      if( len < 0 )
        len = 0;
      sqlite3_result_int(context, len);
      break;
    }
    default: {
      sqlite3_result_null(context);
      break;
    }
  }
}