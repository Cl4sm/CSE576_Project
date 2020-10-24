static void f_aux(sqlite3_context *context, int argc, sqlite3_value **argv){
  int i;
  const char *zParm;
  if( argc!=1 && argc!=2 ) return;

  zParm = (const char*)sqlite3_value_text(argv[0]);
  if( zParm==0 ) return;

  for(i=0; i<g.nAux && g.azAuxName[i]; i++){
    if( strcasecmp(zParm,g.azAuxName[i])==0 ){
      if( g.azAuxVal[i] ){
        sqlite3_result_text(context, g.azAuxVal[i], -1, SQLITE_STATIC);
      }
      return;
    }
  }
  if( g.nAux<MX_AUX ){
    const char *zDef = (argc==2) ? (const char*)sqlite3_value_text(argv[1])
                                 : 0;
    g.azAuxName[g.nAux] = mprintf("%s",zParm);
    g.azAuxParam[g.nAux] = mprintf("%s",zParm);
    for(i=0; g.azAuxParam[g.nAux][i]; i++){
      if(!isalnum(g.azAuxParam[g.nAux][i])) g.azAuxParam[g.nAux][i]='_';
    }
    g.azAuxVal[g.nAux] = mprintf("%s",PD(g.azAuxParam[g.nAux],zDef));
    if( g.azAuxVal[g.nAux] ){
      sqlite3_result_text(context, g.azAuxVal[g.nAux], -1, SQLITE_STATIC);
    }
    g.nAux++;
  }
}