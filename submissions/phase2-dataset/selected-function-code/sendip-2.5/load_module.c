	sendip_module *newmod = malloc(sizeof(sendip_module));
	sendip_module *cur;
	int (*n_opts)(void);
	sendip_option * (*get_opts)(void);
	char (*get_optchar)(void);

	for(cur=first;cur!=NULL;cur=cur->next) {
		if(!strcmp(modname,cur->name)) {
			memcpy(newmod,cur,sizeof(sendip_module));
			newmod->num_opts=0;
			goto out;
		}
	}
	newmod->name=malloc(strlen(modname)+strlen(SENDIP_LIBS)+strlen(".so")+2);
	strcpy(newmod->name,modname);
	if(NULL==(newmod->handle=dlopen(newmod->name,RTLD_NOW))) {
		char *error0=strdup(dlerror());
		sprintf(newmod->name,"./%s.so",modname);
		if(NULL==(newmod->handle=dlopen(newmod->name,RTLD_NOW))) {
			char *error1=strdup(dlerror());
			sprintf(newmod->name,"%s/%s.so",SENDIP_LIBS,modname);
			if(NULL==(newmod->handle=dlopen(newmod->name,RTLD_NOW))) {
				char *error2=strdup(dlerror());
				sprintf(newmod->name,"%s/%s",SENDIP_LIBS,modname);
				if(NULL==(newmod->handle=dlopen(newmod->name,RTLD_NOW))) {
					char *error3=strdup(dlerror());
					fprintf(stderr,"Couldn't open module %s, tried:\n",modname);
					fprintf(stderr,"  %s\n  %s\n  %s\n  %s\n", error0, error1,
							  error2, error3);
					free(newmod);
					free(error3);
					return FALSE;
				}
				free(error2);
			}
			free(error1);
		}
		free(error0);
	}
	strcpy(newmod->name,modname);
	if(NULL==(newmod->initialize=dlsym(newmod->handle,"initialize"))) {
		fprintf(stderr,"%s doesn't have an initialize function: %s\n",modname,
				  dlerror());
		dlclose(newmod->handle);
		free(newmod);
		return FALSE;
	}
	if(NULL==(newmod->do_opt=dlsym(newmod->handle,"do_opt"))) {
		fprintf(stderr,"%s doesn't contain a do_opt function: %s\n",modname,
				  dlerror());
		dlclose(newmod->handle);
		free(newmod);
		return FALSE;
	}
	newmod->set_addr=dlsym(newmod->handle,"set_addr"); // don't care if fails
	if(NULL==(newmod->finalize=dlsym(newmod->handle,"finalize"))) {
		fprintf(stderr,"%s\n",dlerror());
		dlclose(newmod->handle);
		free(newmod);
		return FALSE;
	}
	if(NULL==(n_opts=dlsym(newmod->handle,"num_opts"))) {
		fprintf(stderr,"%s\n",dlerror());
		dlclose(newmod->handle);
		free(newmod);
		return FALSE;
	}
	if(NULL==(get_opts=dlsym(newmod->handle,"get_opts"))) {
		fprintf(stderr,"%s\n",dlerror());
		dlclose(newmod->handle);
		free(newmod);
		return FALSE;
	}
	if(NULL==(get_optchar=dlsym(newmod->handle,"get_optchar"))) {
		fprintf(stderr,"%s\n",dlerror());
		dlclose(newmod->handle);
		free(newmod);
		return FALSE;
	}
	newmod->num_opts = n_opts();
	newmod->optchar=get_optchar();
	/* TODO: check uniqueness */
	newmod->opts = get_opts();

	num_opts+=newmod->num_opts;

out:
	newmod->pack=NULL;
	newmod->prev=last;
	newmod->next=NULL;
	last = newmod;
	if(last->prev) last->prev->next = last;
	if(!first) first=last;

	return TRUE;
}
