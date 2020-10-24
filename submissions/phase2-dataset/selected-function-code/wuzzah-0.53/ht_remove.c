void ht_remove(void *obj, htable_t *ht){
	int hc=(ht->hashcode(obj)%ht->num_buckets);
	ht_list_t *chain=ht->table[hc];
	ht_list_t *oldchain=chain;
	while(chain){
		if(ht->compare(obj, chain->info)==0){
			if(oldchain==chain){	// i.e first list entry
				ht->table[hc]=chain->lptr;
				free(chain);
			} else {
				oldchain->lptr=chain->lptr;
				free(chain);
			}
			ht->size--;
			break;
		} 
		oldchain=chain;	
		chain=chain->lptr; 
	}	
}
