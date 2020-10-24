    mnt_node *mm,*prev,*nxt;
    mm=mounted_list;
    prev=NULL;
    while(mm) {
	nxt=mm->next;
	if (strcmp(mm->ms.mnt_fsname,devname)==0) {
	    if (!prev)
		mounted_list=mm->next;
	    else
		prev->next=mm->next;
	    free_mtab_node(mm);
	}
	else {
	    prev=mm;
	}
	mm=nxt;
    }
    mounted_list_end=prev;
    mtab_changed=1;
}
