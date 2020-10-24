int modem_init ( TFILE *mf, cap c, char *id, 
		 int calling, int poll, int capsset, int *preverse )
{
  int err=0, t=-TO_RESET ;
  char buf [ CMDBUFSIZE ], model [ CMDBUFSIZE ] = "" ;
  char **p, *q, *modelq [2][4] = { { "+FMFR?", "+FMDL?", 0 }, 
				   { "+FMI?", "+FMM?", "+FMR?", 0 } } ;


  /* diasable command echo and get firmware revision */

  cmd ( mf, "E0", t ) ;

  if ( cmd ( mf, "I3", t ) == OK ) {
    getresp ( "", model, CMDBUFSIZE-1 ) ;
    strcat ( model, " " ) ;
  }

  /* if not already chosen, pick safest class; set it */

  if ( ! err && ! c1 && !c2 && ! c20 ) {
    if ( cmd ( mf, "+FCLASS=?", t ) != OK ) {
      err = msg ("E3 modem does not support fax" ) ;
    } else {
      if ( strinresp ( "2.0" ) ) c20 = 1 ;
      else if ( strinresp ( "2" ) ) ;
      else if ( strinresp ( "1" ) ) c1 = 1 ;
      else err = msg ("E3 can't determine fax modem class support" ) ;
      if ( strstr ( model, "Sportster" ) ) { /* USR Sporsters are buggy */
	c1 = 1 ; 
	c2 = c20 = 0 ;
      }
    }
  }

  ckcmd ( mf, &err, c1 ? "+FCLASS=1" : 
       ( c20 ? "+FCLASS=2.0" : "+FCLASS=2" ), t, OK ) ;

  /* get make & model if using Class 2 or 2.0 */

  if ( ! c1 ) {
    for ( p = modelq [ c20 ] ; ! err && *p ; p++ ) {
      if ( cmd ( mf, *p, t ) == OK ) {
	getresp ( "", model, CMDBUFSIZE-1 ) ;
	strcat ( model, " " ) ;
      }
    }
  
    if ( ! c1 && strstr ( model, "Multi-Tech" ) ) {
      *preverse = 0 ;
      msg ("I Multi-Tech bit order set" ) ;
    }
  }

  if ( ! err ) 
    msg ( "I using %sin class %s", model, c1 ? "1" : c20 ? "2.0" : "2" ) ;

  /* get maximum modem speed if not already set (Class 1 only) */

  if ( ! err && c1 && ! capsset ) {
    int i ;
    char *c1spstr [6] = { "24", "48", "72", "96", "121", "145" } ;
    ckcmd ( mf, &err, calling ? "+FTM=?" : "+FRM=?", t, OK ) ;
    for ( i=0 ; i < 6 && strinresp ( c1spstr[i] ) ; i++ ) ;
    c[1]=i?i-1:0;
  }

  /* issue essential commands and set/get modem capabilities (Class 2 only) */

  if ( ! err && ! c1 ) {

    if ( c20 ) {
      ckcmd ( mf, 0, "+FIP", t, OK ) ;
      ckcmd ( mf, 0, "+FNR=1,1,1,0", t, OK ) ;

      ckcmd ( mf, 0, "+FLO=1", t, OK ) ;
      ckcmd ( mf, 0, "+FBO=0", t, OK ) ;
    }

    ckcmd ( mf, &err, "+FCR=1", t, OK ) ;

    if ( ! capsset ) {
      if ( cmd ( mf, c20 ? "+FIS?" : "+FDIS?", -t ) == OK &&
	   ( q = strinresp ( "," ) ) ) {
	str2cap ( q-1, c ) ;
      } else {
	msg ( "W can't get modem capabilities, set to default" ) ;
	capsset = 1 ;
      }
    }

    if ( capsset ) {
      sprintf ( buf, c20 ? "+FCC=%d,%d,%d,%d,%d,%d,%d,%d" : 
		"+FDCC=%d,%d,%d,%d,%d,%d,%d,%d", 
		c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7] ) ;
      ckcmd ( mf, 0, buf, -t, OK ) ;
    }
    
    sprintf ( buf, c20 ? "+FLI=\"%.*s\"" : "+FLID=\"%.*s\"" , 
	     CMDBUFSIZE-9, id ) ;
    ckcmd ( mf, 0, buf, -t, OK ) ;

    if ( ! err && poll ) {

      ckcmd ( mf, 0, c20 ? "+FSP=1" : "+FSPL=1", -t, OK ) ;

      sprintf ( buf, c20 ? "+FPI=\"%.*s\"" : "+FCIG=\"%.*s\"" , 
	       CMDBUFSIZE-9, id ) ;
      ckcmd ( mf, 0, buf, -t, OK ) ;

    }
  }

  return err ;
}
