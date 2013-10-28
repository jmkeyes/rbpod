/* collection.c */

#include "rbpod.h"
#include "collection.h"

VALUE cRbPodCollection;

struct collection { VALUE klass; GList *list; };

inline VALUE rbpod_collection_create(GList *list, VALUE type) {
    struct collection *collection = ALLOC(struct collection);
    collection->list  = list;
    collection->klass = type;
    return Data_Wrap_Struct(cRbPodCollection, NULL, NULL, (void *) collection);
}

static VALUE rbpod_collection_get(VALUE self, VALUE key) {
    struct collection *collection = TYPED_DATA_PTR(self, struct collection);
    GList *current = NULL;

    if (FIXNUM_P(key) == FALSE)
        return Qnil;

    current = g_list_nth(collection->list, FIX2INT(key));

    return Data_Wrap_Struct(collection->klass, NULL, NULL, (void *) current->data);
}

static VALUE rbpod_collection_last(VALUE self) {
    struct collection *collection = TYPED_DATA_PTR(self, struct collection);
    GList *current = g_list_last(collection->list);

    if (current == NULL)
        return Qnil;

    return Data_Wrap_Struct(collection->klass, NULL, NULL, (void *) current->data);
}

static VALUE rbpod_collection_first(VALUE self) {
    struct collection *collection = TYPED_DATA_PTR(self, struct collection);
    GList *current = g_list_first(collection->list);

    if (current == NULL)
        return Qnil;

    return Data_Wrap_Struct(collection->klass, NULL, NULL, (void *) current->data);
}

static VALUE rbpod_collection_length(VALUE self) {
    struct collection *collection = TYPED_DATA_PTR(self, struct collection);
    return INT2NUM(g_list_length(collection->list));
}

static VALUE rbpod_collection_each(VALUE self) {
    struct collection *collection = TYPED_DATA_PTR(self, struct collection);
    GList *current = NULL;
    VALUE item;

    if (rb_block_given_p() == FALSE)
        return rb_funcall(self, rb_intern("enum_for"), 1, ID2SYM(rb_intern("each")));

    /* If we were supplied a block, enumerate the entire list. */
    for (current = collection->list; current != NULL; current = current->next) {
        item = Data_Wrap_Struct(collection->klass, NULL, NULL, (void *) current->data);
        rb_yield(item);
    }

    return Qnil;
}

static VALUE rbpod_collection_initialize(VALUE self) {
    return self;
}

static void rbpod_collection_deallocate(void *handle) {
    struct collection *collection = (struct collection *) handle;

    if (collection->list != NULL)
        g_list_free(collection->list);

    xfree(handle);
    return;
}

static VALUE rbpod_collection_allocate(VALUE self) {
    struct collection *collection = ALLOC(struct collection);
    collection->list = NULL;
    collection->klass = Qnil;
    return Data_Wrap_Struct(cRbPodCollection, NULL, rbpod_collection_deallocate, (void *) collection);
}

void Init_rbpod_collection(void) {
    cRbPodCollection = rb_define_class_under(mRbPod, "Collection", rb_cObject);

    rb_define_alloc_func(cRbPodCollection, rbpod_collection_allocate);

    rb_include_module(cRbPodCollection, rb_mEnumerable);

    rb_define_method(cRbPodCollection, "initialize", rbpod_collection_initialize, 0);

    rb_define_method(cRbPodCollection, "each", rbpod_collection_each, 0);

    rb_define_method(cRbPodCollection, "length", rbpod_collection_length, 0);

    rb_define_alias(cRbPodCollection,  "size", "length");

    rb_define_method(cRbPodCollection, "first", rbpod_collection_first, 0);
    rb_define_method(cRbPodCollection, "last", rbpod_collection_last, 0);

    rb_define_method(cRbPodCollection, "[]", rbpod_collection_get, 1);
}
