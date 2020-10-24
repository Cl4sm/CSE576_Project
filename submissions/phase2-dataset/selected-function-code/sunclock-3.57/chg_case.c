void chg_case(int upper)
{
	char *s;
	int i, j, change=0;

	if(!flag[BLK]) return; 	/* exit if no mark */
	if(cur_pos < mk) {		/* ensure cur_pos >= mk */
		s = cur_pos ;		/* swap cur_pos & mk */
		cur_pos = mk ;
		mk = s ;
		goto_ptr(cur_pos);
	}
	i = j = cur_pos+1 - mk;		/* get length */

	/* detect whether there is any change */
	while(j--) 
	  if (mk[j] != (!upper)? tolower(mk[j]) : toupper(mk[j])) {
	     change = 1;
	     break;
	  }
	if (!change) return;
	u_del(mk, i-1);			/* save undo data enmasse */
	/* printf("mk=%x, i=%d.\n",mk,i); */
	while(i--) mk[i] = !upper ? tolower(mk[i]) : toupper(mk[i]);
	u_ins(mk, cur_pos - mk);
        flag[CHG] = 1;
	flag[SHW] = 1;
}
