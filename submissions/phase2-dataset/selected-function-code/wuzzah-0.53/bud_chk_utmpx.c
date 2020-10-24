void bud_chk_utmpx(htable_t *buddy_table, short write_users){
	ht_list_t *budlist_root,*budlist, *utlist_root,*utlist;
	STRUCT_UTMPX *u;
	// below are for quick lookups in the respective ht's
	// (all i need to do is fill in the fields that are used 
	// for the hash and comp functions)
	buddy_t *budptr=NULL, tmpbud;
	bud_utrec_t *recptr=NULL, tmprec;

	budlist_root=ht_iter(buddy_table);
	budlist=budlist_root;
	while(budlist){
		utlist_root=ht_iter(((buddy_t*)budlist->info)->logins);
		utlist=utlist_root;
		while(utlist){
			((bud_utrec_t*)utlist->info)->verified=B_LOGGED_OUT;
			utlist=utlist->lptr;
		}
		ht_free_iter(utlist_root);
		budlist=budlist->lptr;
	}
	ht_free_iter(budlist_root);

	SETUTXENT();
	u=GETUTXENT();
	while(u){
		if(IS_USER_PROCESS(u)){
			tmpbud.name=u->ut_user;
			budptr=ht_find(&tmpbud, buddy_table);
			if(budptr){
				tmprec.u=u;
				recptr=NULL;
				recptr=ht_find(&tmprec, budptr->logins);
				if(recptr){
					recptr->verified=B_UNCHANGED;
				} else {
					recptr=MSTRUCT(bud_utrec_t);
					recptr->u=MSTRUCT(STRUCT_UTMPX);
					memcpy(recptr->u, u,
						sizeof(STRUCT_UTMPX));
					recptr->verified=B_LOGGED_IN;
					ht_insert(recptr, budptr->logins);
				}
			}
		}
		u=GETUTXENT();
	}
	ENDUTXENT();

	budlist_root=ht_iter(buddy_table);
	budlist=budlist_root;
	while(budlist){
		budptr=(buddy_t*)budlist->info;
		utlist_root=ht_iter(budptr->logins);
		utlist=utlist_root;
		while(utlist){
			recptr=(bud_utrec_t*)utlist->info;

			event(budptr, recptr, write_users);

			if(recptr->verified==B_LOGGED_OUT)
				ht_remove(recptr, budptr->logins);

			utlist=utlist->lptr;
		}
		ht_free_iter(utlist_root);
		budlist=budlist->lptr;
	}
	ht_free_iter(budlist_root);

	return;
}
