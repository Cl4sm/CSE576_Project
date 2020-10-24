static void ticket_change(
  time_t date,        /* date/time of the change */
  int tn,             /* ticket number */
  const char *zUser,  /* user that made the change */
  const char *zField, /* field that changed */
  const char *zOld,   /* old value */
  const char *zNew,   /* new value */
  int isLast          /* non-zero if last ticket change in the history */
){
  struct tm *pTm;
  char zDate[100];
  char zPage[30];

  bprintf(zPage,sizeof(zPage),"%d",tn);

  pTm = localtime(&date);
  strftime(zDate, sizeof(zDate), "%Y-%b-%d %H:%M", pTm);

  @ <li>

  if( strcmp(zField,"description")==0 || strcmp(zField,"remarks")==0 ){
    int len1, len2;
    len1 = strlen(zOld);
    len2 = strlen(zNew);
    if( len1==0 ){
      @ Added <i>%h(zField)</i>:<blockquote>
      output_formatted(&zNew[len1], zPage);
      @ </blockquote>
    }else if( len2>len1+5 && strncmp(zOld,zNew,len1)==0 ){
      @ Appended to <i>%h(zField)</i>:<blockquote>
      output_formatted(&zNew[len1], zPage);
      @ </blockquote>
    }else{
      @ Changed <i>%h(zField)</i>.
      diff_strings(1,zOld,zNew);
    }
  }else if( (!g.okWrite || g.isAnon) && strcmp(zField,"contact")==0 ){
    /* Do not show contact information to unprivileged users */
    @ Change <i>%h(zField)</i>
  }else if( strncmp(zField,"extra",5)==0 ){
    char zLabel[30];
    const char *zAlias;
    bprintf(zLabel,sizeof(zLabel),"%h_name", zField);
    zAlias = db_config(zLabel, zField);
    @ Change <i>%h(zAlias)</i> from "%h(zOld)" to "%h(zNew)"
  }else{
    @ Change <i>%h(zField)</i> from "%h(zOld)" to "%h(zNew)"
  }

  @ by %h(zUser) on %h(zDate)

  if( isLast && ok_to_undo_change(date, zUser) ){
    @ [<a href="tktundo?tn=%d(tn)&u=%t(zUser)&t=%d(date)">Undo
    @ this change</a>]</p>
  }

  @ </li>
}