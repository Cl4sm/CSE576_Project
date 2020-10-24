void close_text(text *txt)
{
	substitution *subst;
	formatted *fmt;
	conditional *cond;
	while (txt->src != NULL) {
		close_source(txt);
	}
	while (txt->substs != NULL) {
		subst = txt->substs;
		myfree(subst->token);
		myfree(subst->subst);
		txt->substs = txt->substs->next;
		myfree(subst);
	}
	if (txt->mailname != NULL) myfree(txt->mailname);
	while (txt->fmts != NULL) {
		fmt = txt->fmts;
		myfree(fmt->token);
		txt->fmts = txt->fmts->next;
		myfree(fmt);
	}
	while (txt->cond != NULL) {
		cond = txt->cond;
		txt->cond = txt->cond->outer;
		myfree(cond);
	}
	myfree(txt);
}