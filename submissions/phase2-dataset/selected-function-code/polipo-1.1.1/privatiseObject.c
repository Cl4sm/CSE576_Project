void
privatiseObject(ObjectPtr object, int linear) 
{
    int i, h;
    if(!(object->flags & OBJECT_PUBLIC)) {
        if(linear)
            object->flags |= OBJECT_LINEAR;
        return;
    }

    if(object->disk_entry)
        destroyDiskEntry(object, 0);
    object->flags &= ~OBJECT_PUBLIC;

    for(i = 0; i < object->numchunks; i++) {
        if(object->chunks[i].locked)
            break;
        if(object->chunks[i].data) {
            object->chunks[i].size = 0;
            dispose_chunk(object->chunks[i].data);
            object->chunks[i].data = NULL;
        }
    }

    h = hash(object->type, object->key, object->key_size, 
             log2ObjectHashTableSize);
    assert(objectHashTable[h] == object);
    objectHashTable[h] = NULL;

    if(object->previous)
        object->previous->next = object->next;
    if(object_list == object)
        object_list = object->next;
    if(object->next)
        object->next->previous = object->previous;
    if(object_list_end == object)
        object_list_end = object->previous;
    object->previous = NULL;
    object->next = NULL;

    publicObjectCount--;
    privateObjectCount++;

    if(object->refcount == 0)
        destroyObject(object);
    else {
        if(linear)
            object->flags |= OBJECT_LINEAR;
    }
}