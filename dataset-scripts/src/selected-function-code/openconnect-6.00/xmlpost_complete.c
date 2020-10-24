static int xmlpost_complete(xmlDocPtr doc, char *body, int bodylen)
{
	xmlChar *mem = NULL;
	int len, ret = 0;

	if (!body) {
		xmlFree(doc);
		return 0;
	}

	xmlDocDumpMemoryEnc(doc, &mem, &len, "UTF-8");
	if (!mem) {
		xmlFreeDoc(doc);
		return -ENOMEM;
	}

	if (len > bodylen)
		ret = -E2BIG;
	else {
		memcpy(body, mem, len);
		body[len] = 0;
	}

	xmlFreeDoc(doc);
	xmlFree(mem);

	return ret;
}