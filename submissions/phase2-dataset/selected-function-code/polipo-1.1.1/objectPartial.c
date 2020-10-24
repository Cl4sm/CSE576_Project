ObjectPtr
objectPartial(ObjectPtr object, int length, struct _Atom *headers)
{
    object->headers = headers;

    if(length >= 0) {
        if(object->size > length) {
            abortObject(object, 502,
                        internAtom("Inconsistent Content-Length"));
            notifyObject(object);
            return object;
        }
    }

    if(length >= 0)
        object->length = length;

    object->flags &= ~OBJECT_INITIAL;
    revalidateDiskEntry(object);
    notifyObject(object);
    return object;
}