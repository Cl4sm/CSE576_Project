ObjectPtr
makeObject(int type, const void *key, int key_size, int public, int fromdisk,
           RequestFunction request, void* request_closure)
{
    ObjectPtr object;
    int h;

    object = findObject(type, key, key_size);
    if(object != NULL) {
        if(public)
            return object;
        else
            privatiseObject(object, 0);
    }

    if(publicObjectCount + privateObjectCount >= objectHighMark) {
        if(!objectExpiryScheduled)
            discardObjects(0, 0);
        if(publicObjectCount + privateObjectCount >= objectHighMark) {
            return NULL;
        }
    }

    if(publicObjectCount >= publicObjectLowMark && 
       !objectExpiryScheduled) {
        TimeEventHandlerPtr event;
        event = scheduleTimeEvent(-1, discardObjectsHandler, 0, NULL);
        if(event)
            objectExpiryScheduled = 1;
        else
            do_log(L_ERROR, "Couldn't schedule object expiry.\n");
    }

    object = malloc(sizeof(ObjectRec));
    if(object == NULL)
        return NULL;

    object->type = type;
    object->request = request;
    object->request_closure = request_closure;
    object->key = malloc(key_size + 1);
    if(object->key == NULL) {
        free(object);
        return NULL;
    }
    memcpy(object->key, key, key_size);
    /* In order to make it more convenient to use keys as strings,
       they are NUL-terminated. */
    object->key[key_size] = '\0';
    object->key_size = key_size;
    object->flags = (public?OBJECT_PUBLIC:0) | OBJECT_INITIAL;
    if(public) {
        h = hash(object->type, object->key, object->key_size, 
                 log2ObjectHashTableSize);
        if(objectHashTable[h]) {
            writeoutToDisk(objectHashTable[h], objectHashTable[h]->size, -1);
            privatiseObject(objectHashTable[h], 0);
            assert(!objectHashTable[h]);
        }
        objectHashTable[h] = object;
        object->next = object_list;
        object->previous = NULL;
        if(object_list)
            object_list->previous = object;
        object_list = object;
        if(!object_list_end)
            object_list_end = object;
    } else {
        object->next = NULL;
        object->previous = NULL;
    }
    object->abort_data = NULL;
    object->code = 0;
    object->message = NULL;
    initCondition(&object->condition);
    object->headers = NULL;
    object->via = NULL;
    object->numchunks = 0;
    object->chunks = NULL;
    object->length = -1;
    object->date = -1;
    object->age = -1;
    object->expires = -1;
    object->last_modified = -1;
    object->atime = -1;
    object->etag = NULL;
    object->cache_control = 0;
    object->max_age = -1;
    object->s_maxage = -1;
    object->size = 0;
    object->requestor = NULL;
    object->disk_entry = NULL;
    if(object->flags & OBJECT_PUBLIC)
        publicObjectCount++;
    else
        privateObjectCount++;
    object->refcount = 1;

    if(public && fromdisk)
        objectGetFromDisk(object);
    return object;
}