static int generate_html(
  void* pUser,     /* Pointer to output state */
  int nArg,        /* Number of columns in this result row */
  char **azArg,    /* Text of data in all columns */
  char **azName    /* Names of the columns */
){
  struct GenerateHTML* pState = (struct GenerateHTML*)pUser;
  int i;
  int tn;            /* Ticket number.  (value of column named '#') */
  int rn;            /* Report number */
  int ncol;          /* Number of columns in the table */
  int multirow;      /* True if multiple table rows per line of data */
  int newrowidx;     /* Index of first column that goes on a separate row */
  int iBg = -1;      /* Index of column that determines background color */
  char *zBg = 0;     /* Use this background color */
  char zPage[30];    /* Text version of the ticket number */

  /* Get the report number
  */
  rn = pState->rn;

  /* Figure out the number of columns, the column that determines background
  ** color, and whether or not this row of data is represented by multiple
  ** rows in the table.
  */
  ncol = 0;
  multirow = 0;
  newrowidx = -1;
  for(i=0; i<nArg; i++){
    if( azName[i][0]=='b' && strcmp(azName[i],"bgcolor")==0 ){
      zBg = azArg ? azArg[i] : 0;
      iBg = i;
      continue;
    }
    if( g.okWrite && azName[i][0]=='#' ){
      ncol++;
    }
    if( !multirow ){
      if( azName[i][0]=='_' ){
        multirow = 1;
        newrowidx = i;
      }else{
        ncol++;
      }
    }
  }

  /* The first time this routine is called, output a table header
  */
  if( pState->nCount==0 ){
    char zExtra[2000];
    int nField = atoi(PD("order_by","0"));
    const char* zDir = PD("order_dir","");
    zDir = !strcmp("ASC",zDir) ? "ASC" : "DESC";
    zExtra[0] = 0;

    if( g.nAux ){
      @ <tr>
      @ <td colspan=%d(ncol)><form action="rptview" method="GET">
      @ <input type="hidden" name="rn" value="%d(rn)">
      for(i=0; i<g.nAux; i++){
        const char *zN = g.azAuxName[i];
        const char *zP = g.azAuxParam[i];
        if( g.azAuxVal[i] && g.azAuxVal[i][0] ){
          appendf(zExtra,0,sizeof(zExtra),
                  "&%t=%t",g.azAuxParam[i],g.azAuxVal[i]);
        }
        if( g.azAuxOpt[i] ){
          @ %h(zN): 
          if( g.anAuxCols[i]==1 ) {
            cgi_v_optionmenu( 0, zP, g.azAuxVal[i], g.azAuxOpt[i] );
          }else if( g.anAuxCols[i]==2 ){
            cgi_v_optionmenu2( 0, zP, g.azAuxVal[i], g.azAuxOpt[i] );
          }
        }else{
          @ %h(zN): <input type="text" name="%h(zP)" value="%h(g.azAuxVal[i])">
        }
      }
      @ <input type="submit" value="Go">
      @ </form></td></tr> 
    }
    @ <tr>
    tn = -1;
    for(i=0; i<nArg; i++){
      char *zName = azName[i];
      if( i==iBg ) continue;
      if( newrowidx>=0 && i>=newrowidx ){
        if( g.okWrite && tn>=0 ){
          @ <th>&nbsp;</th>
          tn = -1;
        }
        if( zName[0]=='_' ) zName++;
        @ </tr><tr><th colspan=%d(ncol)>%h(zName)</th>
      }else{
        if( zName[0]=='#' ){
          tn = i;
        }
        /*
        ** This handles any sorting related stuff. Note that we don't
        ** bother trying to sort on the "wiki format" columns. I don't
        ** think it makes much sense, visually.
        */
        column_header(rn,azName[i],i+1,nField,zDir,zExtra);
      }
    }
    if( g.okWrite && tn>=0 ){
      @ <th>&nbsp;</th>
    }
    @ </tr>
  }
  if( azArg==0 ){
    @ <tr><td colspan="%d(ncol)">
    @ <i>No records match the report criteria</i>
    @ </td></tr>
    return 0;
  }
  ++pState->nCount;

  /* Output the separator above each entry in a table which has multiple lines
  ** per database entry.
  */
  if( newrowidx>=0 ){
    @ <tr><td colspan=%d(ncol)><font size=1>&nbsp;</font></td></tr>
  }

  /* Output the data for this entry from the database
  */
  if( zBg==0 ) zBg = "white";
  @ <tr bgcolor="%h(zBg)">
  tn = 0;
  zPage[0] = 0;
  for(i=0; i<nArg; i++){
    char *zData;
    if( i==iBg ) continue;
    zData = azArg[i];
    if( zData==0 ) zData = "";
    if( newrowidx>=0 && i>=newrowidx ){
      if( tn>0 && g.okWrite ){
        @ <td valign="top"><a href="tktedit?tn=%d(tn),%d(rn)">edit</a></td>
        tn = 0;
      }
      if( zData[0] ){
        @ </tr><tr bgcolor="%h(zBg)"><td colspan=%d(ncol)>
        output_formatted(zData, zPage[0] ? zPage : 0);
      }
    }else if( azName[i][0]=='#' ){
      tn = atoi(zData);
      if( tn>0 ) bprintf(zPage, sizeof(zPage), "%d", tn);
      @ <td valign="top"><a href="tktview?tn=%d(tn),%d(rn)">%h(zData)</a></td>
    }else if( zData[0]==0 ){
      @ <td valign="top">&nbsp;</td>
    }else{
      @ <td valign="top">
      output_report_field(zData,rn);
      @ </td>
    }
  }
  if( tn>0 && g.okWrite ){
    @ <td valign="top"><a href="tktedit?tn=%d(tn),%d(rn)">edit</a></td>
  }
  @ </tr>
  return 0;
}