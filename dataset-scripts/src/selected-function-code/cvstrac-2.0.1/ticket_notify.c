void ticket_notify(int tn, int first_change, int last_change, int atn){
  const char *zNotify;
  char *zCmd;
  int i, j, c;
  int cmdSize;
  int cnt[128];
  const char *azSubst[128];

  static const struct { int key; char *zColumn; } aKeys[] = {
      { 'a',  "assignedto"  },
      /* A - e-mail address of assignedto person */
      { 'c',  "contact"     },
      { 'd',  "description" },
      /* D - description, HTML formatted */
      /* f - First TKTCHNG rowid of change set; zero if new record */
      /* h = attacHment number if change is a new attachment; zero otherwise */
      /* l - Last TKTCHNG rowid of change set; zero if new record */
      /* n - ticket number */
      /* p - project name  */
      { 'r',  "remarks"     },
      /* R - remarks, HTML formatted */
      { 's',  "status"      },
      { 't',  "title"       },
      /* u - current user  */
      { 'w',  "owner"       },
      { 'y',  "type"        },
      { '1',  "extra1"        },
      { '2',  "extra2"        },
      { '3',  "extra3"        },
      { '4',  "extra4"        },
      { '5',  "extra5"        },
  };

  if( !db_exists("SELECT tn FROM ticket WHERE tn=%d",tn) ){
    return;
  }

  zNotify = db_config("notify",0);
  if( zNotify==0 || zNotify[0]==0 ) return;
  memset(cnt, 0, sizeof(cnt));
  memset(azSubst, 0, sizeof(azSubst));
  for(i=0; zNotify[i]; i++){
    if( zNotify[i]=='%' ){
      c = zNotify[i+1] & 0x7f;
      cnt[c&0x7f]++;
    }
  }
  if( cnt['n']>0 ){
    azSubst['n'] = mprintf("%d", tn);
  }
  if( cnt['f']>0 ){
    azSubst['f'] = mprintf("%d", first_change);
  }
  if( cnt['h']>0 ){
    azSubst['h'] = mprintf("%d", atn);
  }
  if( cnt['l']>0 ){
    azSubst['l'] = mprintf("%d", last_change);
  }
  if( cnt['u']>0 ){
    azSubst['u'] = mprintf("%s", g.zUser);
  }
  if( cnt['p']>0 ){
    azSubst['p'] = mprintf("%s", g.zName);
  }
  if( cnt['D']>0 ){
    /* ensure we grab a description */
    cnt['d']++;
  }
  if( cnt['R']>0 ){
    /* ensure we grab remarks */
    cnt['r']++;
  }
  if( cnt['A']>0 ){
    azSubst['A'] = 
      db_short_query("SELECT user.email FROM ticket, user "
                     "WHERE ticket.tn=%d and ticket.assignedto=user.id", tn);
  }
  for(i=0; i<sizeof(aKeys)/sizeof(aKeys[0]); i++){
    c = aKeys[i].key;
    if( cnt[c]>0 ){
      azSubst[c] =
        db_short_query("SELECT %s FROM ticket WHERE tn=%d",
                       aKeys[i].zColumn, tn);
    }
  }
  if( cnt['c']>0 && azSubst['c']==0 && azSubst['c'][0]==0 ){
    azSubst['c'] = 
      db_short_query("SELECT user.email FROM ticket, user "
                     "WHERE ticket.tn=%d and ticket.owner=user.id", tn);
  }
  if( cnt['D'] ){
    azSubst['D'] = format_formatted( azSubst['d'] );
    cnt['d']--;
  }
  if( cnt['R'] ){
    azSubst['R'] = format_formatted( azSubst['r'] );
    cnt['r']--;
  }

  /* Sanitize the strings to be substituted by removing any single-quotes
  ** and backslashes.
  **
  ** That way, the notify command can contains strings like '%d' or '%r'
  ** (surrounded by quotes) and a hostile user cannot insert arbitrary
  ** shell commands.  Also figure out how much space is needed to hold
  ** the string after substitutes occur.
  */
  cmdSize = strlen(zNotify)+1;
  for(i=0; i<sizeof(azSubst)/sizeof(azSubst[0]); i++){
    if( azSubst[i]==0 || cnt[i]<=0 ) continue;
    azSubst[i] = quotable_string(azSubst[i]);
    cmdSize += cnt[i]*strlen(azSubst[i]);
  }

  zCmd = malloc( cmdSize + 1 );
  if( zCmd==0 ) return;
  for(i=j=0; zNotify[i]; i++){
    if( zNotify[i]=='%' && (c = zNotify[i+1]&0x7f)!=0 && azSubst[c]!=0 ){
      int k;
      const char *z = azSubst[c];
      for(k=0; z[k]; k++){ zCmd[j++] = z[k]; }
      i++;
    }else{
      zCmd[j++] = zNotify[i];
    }
  }
  zCmd[j] = 0;
  assert( j<=cmdSize );
  system(zCmd);
  free(zCmd);
}