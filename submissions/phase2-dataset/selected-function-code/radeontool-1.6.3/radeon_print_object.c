static void radeon_print_object(uint16_t obj)
{
    uint8_t obj_id, num, obj_type;

    obj_id = (obj & OBJECT_ID_MASK) >> OBJECT_ID_SHIFT;
    num = (obj & ENUM_ID_MASK) >> ENUM_ID_SHIFT;
    obj_type = (obj & OBJECT_TYPE_MASK) >> OBJECT_TYPE_SHIFT;

    if (obj_type == GRAPH_OBJECT_TYPE_CONNECTOR)
	printf("%s, %d, %s (0x%04x)\n",
	       object_type_names[obj_type], num, connector_names[obj_id], obj);
    else if (obj_type == GRAPH_OBJECT_TYPE_ENCODER)
	printf("%s, %d, %s (0x%04x)\n",
	       object_type_names[obj_type], num, object_names[obj_id], obj);
    else if (obj_type == GRAPH_OBJECT_TYPE_ROUTER)
	printf("%s, %d, %s (0x%04x)\n",
	       object_type_names[obj_type], num, router_object_names[obj_id], obj);
    else
	printf("%s, %d, 0x%02x (0x%04x)\n",
	       object_type_names[obj_type], num, obj_id, obj);

}