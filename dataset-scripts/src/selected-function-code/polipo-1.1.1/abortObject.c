void
abortObject(ObjectPtr object, int code, AtomPtr message)
{
    int i;

    assert(code != 0);

    object->flags &= ~(OBJECT_INITIAL | OBJECT_VALIDATING);
    object->flags |= OBJECT_ABORTED;
    object->code = code;
    if(object->message) releaseAtom(object->message);
    object->message = message;
    object->length = 0;
    object->date = object->age;
    object->expires = object->age;
    object->last_modified = -1;
    if(object->etag) free(object->etag);
    object->etag = NULL;
    if(object->headers) releaseAtom(object->headers); 
    object->headers = NULL;
    object->size = 0;
    for(i = 0; i < object->numchunks; i++) {
        if(object->chunks[i].data) {
            if(!object->chunks[i].locked) {
                dispose_chunk(object->chunks[i].data);
                object->chunks[i].data = NULL;
                object->chunks[i].size = 0;
            }
        }
    }
    privatiseObject(object, 0);
}