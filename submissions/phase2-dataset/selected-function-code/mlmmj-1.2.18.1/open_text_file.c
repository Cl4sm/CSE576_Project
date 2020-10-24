text *open_text_file(const char *listdir, const char *filename)
{
	char *tmp;
	text *txt;

	txt = mymalloc(sizeof(text));
	txt->src = mymalloc(sizeof(source));
	txt->src->prev = NULL;
	txt->src->upcoming = NULL;
	txt->src->processedlen = 0;
	txt->src->processedwidth = 0;
	txt->src->prefix = NULL;
	txt->src->prefixlen = 0;
	txt->src->prefixwidth = 0;
	txt->src->suffix = NULL;
	txt->src->transparent = 0;
	txt->src->limit = -1;
	txt->src->fmt = NULL;
	txt->action = NULL;
	txt->reason = NULL;
	txt->type = NULL;
	txt->substs = NULL;
	txt->mailname = NULL;
	txt->fmts = NULL;
	txt->wrapindent = 0;
	txt->wrapwidth = 0;
	txt->wrapmode = WRAP_WORD;
	txt->widthreckoning = WIDTH_THIN;
	txt->zerowidth = NULL;
	txt->cond = NULL;
	txt->skip = NULL;

	tmp = concatstr(3, listdir, "/text/", filename);
	txt->src->fd = open(tmp, O_RDONLY);
	myfree(tmp);
	if (txt->src->fd >= 0) return txt;

	tmp = concatstr(2, DEFAULTTEXTDIR "/default/", filename);
	txt->src->fd = open(tmp, O_RDONLY);
	myfree(tmp);
	if (txt->src->fd >= 0) return txt;

	tmp = concatstr(2, DEFAULTTEXTDIR "/en/", filename);
	txt->src->fd = open(tmp, O_RDONLY);
	myfree(tmp);
	if (txt->src->fd >= 0) return txt;

	return NULL;
}