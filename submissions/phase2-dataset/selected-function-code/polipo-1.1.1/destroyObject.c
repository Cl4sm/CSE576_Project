void
destroyObject(ObjectPtr object)
{
    int i;

    assert(object->refcount == 0 && !object->requestor);
    assert(!object->condition.handlers && 
           (object->flags & OBJECT_INPROGRESS) == 0);

    if(object->disk_entry)
        destroyDiskEntry(object, 0);

    if(object->flags & OBJECT_PUBLIC) {
        privatiseObject(object, 0);
    } else {
        object->type = -1;
        if(object->message) releaseAtom(object->message);
        if(object->key) free(object->key);
        if(object->headers) releaseAtom(object->headers);
        if(object->etag) free(object->etag);
        if(object->via) releaseAtom(object->via);
        for(i = 0; i < object->numchunks; i++) {
            assert(!object->chunks[i].locked);
            if(object->chunks[i].data)
                dispose_chunk(object->chunks[i].data);
            object->chunks[i].data = NULL;
            object->chunks[i].size = 0;
        }
        if(object->chunks) free(object->chunks);
        privateObjectCount--;
        free(object);
    }
}