/* collection.c */

#include "rbpod.h"
#include "collection.h"

struct collection {
    VALUE klass;
    GList *list;
};

inline VALUE rbpod_collection_create(GList *list, VALUE type)
{
    struct collection *collection = ALLOC(struct collection);
    collection->list  = list;
    collection->klass = type;
    return Data_Wrap_Struct(cRbPodCollection, NULL, NULL, (void *) collection);
}

/*
 * call-seq:
 *     [](index) -> Object or nil
 *
 * Return the item located at the given position in the list.
 */
static VALUE rbpod_collection_get(VALUE self, VALUE key)
{
    struct collection *collection = TYPED_DATA_PTR(self, struct collection);
    GList *current = NULL;

    if (FIXNUM_P(key) == FALSE) {
        return Qnil;
    }

    current = g_list_nth(collection->list, FIX2INT(key));

    return Data_Wrap_Struct(collection->klass, NULL, NULL, (void *) current->data);
}

/*
 * call-seq:
 *     last() -> Object
 *
 * Return the last item of the collection.
 */
static VALUE rbpod_collection_last(VALUE self)
{
    struct collection *collection = TYPED_DATA_PTR(self, struct collection);
    GList *current = g_list_last(collection->list);

    if (current == NULL) {
        return Qnil;
    }

    return Data_Wrap_Struct(collection->klass, NULL, NULL, (void *) current->data);
}

/*
 * call-seq:
 *     first() -> Object
 *
 * Return the first item of the collection.
 */
static VALUE rbpod_collection_first(VALUE self)
{
    struct collection *collection = TYPED_DATA_PTR(self, struct collection);
    GList *current = g_list_first(collection->list);

    if (current == NULL) {
        return Qnil;
    }

    return Data_Wrap_Struct(collection->klass, NULL, NULL, (void *) current->data);
}

/*
 * call-seq:
 *     length() -> Integer
 *
 * Return the total length of all items in the collection.
 */
static VALUE rbpod_collection_length(VALUE self)
{
    struct collection *collection = TYPED_DATA_PTR(self, struct collection);
    return INT2NUM(g_list_length(collection->list));
}

/*
 * call-seq:
 *     each(*args) -> Enumerator
 *     each(*args) { |item| block } -> Enumerator
 *
 * Iterate over the collection, passing each item to a given block.
 * If no block was supplied, return an enumerator for the collection.
 *
 */
static VALUE rbpod_collection_each(VALUE self, VALUE argv)
{
    struct collection *collection = TYPED_DATA_PTR(self, struct collection);
    VALUE item = Qnil, arguments = rb_ary_dup(argv);
    GList *current = NULL;

    /* Return an enumerator if a block was not supplied. */
    RETURN_ENUMERATOR(self, 0, 0);

    /* Prepend an empty element as a placeholder. */
    rb_ary_unshift(arguments, Qnil);

    /* If we were supplied a block, enumerate the entire list. */
    for (current = collection->list; current != NULL; current = current->next) {
        item = Data_Wrap_Struct(collection->klass, NULL, NULL, (void *) current->data);
        rb_ary_store(arguments, 0, item);
        rb_yield_splat(arguments);
    }

    return self;
}

static void rbpod_collection_deallocate(void *handle)
{
    struct collection *collection = (struct collection *) handle;

    if (collection->list != NULL) {
        g_list_free(collection->list);
    }

    xfree(handle);
    return;
}

static VALUE rbpod_collection_allocate(VALUE self)
{
    struct collection *collection = ALLOC(struct collection);
    collection->list = NULL;
    collection->klass = Qnil;
    return Data_Wrap_Struct(cRbPodCollection, NULL, rbpod_collection_deallocate, (void *) collection);
}

void Init_rbpod_collection(void)
{
#if RDOC_CAN_PARSE_DOCUMENTATION
    mRbPod = rb_define_module("RbPod");
#endif
    cRbPodCollection = rb_define_class_under(mRbPod, "Collection", rb_cObject);

    rb_define_alloc_func(cRbPodCollection, rbpod_collection_allocate);

    rb_include_module(cRbPodCollection, rb_mEnumerable);
    rb_include_module(cRbPodCollection, rb_mComparable);

    rb_define_method(cRbPodCollection, "each", rbpod_collection_each, -2);

    rb_define_method(cRbPodCollection, "length", rbpod_collection_length, 0);

    rb_define_alias(cRbPodCollection,  "size", "length");
    rb_define_alias(cRbPodCollection,  "count", "length");

    rb_define_method(cRbPodCollection, "first", rbpod_collection_first, 0);
    rb_define_method(cRbPodCollection, "last", rbpod_collection_last, 0);

    rb_define_method(cRbPodCollection, "[]", rbpod_collection_get, 1);
}
