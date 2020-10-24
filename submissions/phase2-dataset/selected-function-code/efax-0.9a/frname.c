char *frname ( int fr )
{
  static struct framenamestruct {  int code ;  char *name ; } 
  framenames [] = {

    {NSC,"NSC - poller features"}, /* these 3 frames must be first */
    {CIG,"CIG - poller ID"}, 
    {DTC,"DTC - poller capabilities"},
    {NSF,"NSF - answering features"},
    {CSI,"CSI - answering ID"},
    {DIS,"DIS - answering capabilities"},
    {NSS,"NSS - caller features"},
    {TSI,"TSI - caller ID"},
    {DCS,"DCS - session format"},

    {CFR,"CFR - channel OK"},
    {FTT,"FTT - channel not OK"},

    {MPS,"MPS - not done"},
    {EOM,"EOM - not done, new format"},
    {EOP,"EOP - done"},

    {PRI_MPS,"PRI-MPS - not done, call operator"},
    {PRI_EOM,"PRI-EOM - not done, new format, call operator"},
    {PRI_EOP,"PRI-EOP - done, call operator"},

    {MCF,"MCF - page OK"},
    {RTP,"RTP - page OK, check channel"},
    {PIP,"PIP - page OK, call operator"},
    {RTN,"RTN - page not OK, check channel"},
    {PIN,"PIN - page not OK, call operator"},

    {CRP,"CRP - repeat command"},
    {DCN,"DCN - disconnect"},
    {0,0} },
  *p ;
  
  for ( p=framenames ; p->code ; p++ )
    if ( fr == p->code || ( fr & 0x7f ) == p->code) break ;
  return p->code ? p->name : "UNKNOWN" ;
}
