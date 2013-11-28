/* collection.c */

#include "rbpod.h"

/*
 * call-seq:
 *     [](index) -> Object or nil
 *
 * Given an integer +index+, return the item at that position in the collection.
 */
static VALUE rbpod_collection_get(VALUE self, VALUE key)
{
    GList *current = NULL, *collection = TYPED_DATA_PTR(self, GList);
    VALUE klass = rb_funcall(self, rb_intern("type"), 0);

    if (FIXNUM_P(key) == FALSE) {
        return Qnil;
    }

    current = g_list_nth(collection, FIX2INT(key));

    if (current == NULL) {
        return Qnil;
    }

    return rb_class_new_instance_with_data(0, NULL, klass, current->data);
}

/*
 * call-seq:
 *     last() -> Object
 *
 * Return the last item of the collection.
 */
static VALUE rbpod_collection_last(VALUE self)
{
    VALUE klass = rb_funcall(self, rb_intern("type"), 0);
    GList *collection = TYPED_DATA_PTR(self, GList);
    GList *current = g_list_last(collection);

    if (current == NULL) {
        return Qnil;
    }

    return rb_class_new_instance_with_data(0, NULL, klass, current->data);
}

/*
 * call-seq:
 *     first() -> Object
 *
 * Return the first item of the collection.
 */
static VALUE rbpod_collection_first(VALUE self)
{
    VALUE klass = rb_funcall(self, rb_intern("type"), 0);
    GList *collection = TYPED_DATA_PTR(self, GList);
    GList *current = g_list_first(collection);

    if (current == NULL) {
        return Qnil;
    }

    return rb_class_new_instance_with_data(0, NULL, klass, current->data);
}

/*
 * call-seq:
 *     size() -> Integer
 *
 * Return the total length of all items in the collection.
 */
static VALUE rbpod_collection_size(VALUE self)
{
    GList *collection = TYPED_DATA_PTR(self, GList);
    return INT2NUM(g_list_length(collection));
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
    GList *current = NULL, *collection = TYPED_DATA_PTR(self, GList);
    VALUE klass, item, arguments;

    /* Return an enumerator if a block was not supplied. */
    RETURN_ENUMERATOR(self, 0, 0);

    /* What sort of items are we casting this data to? */
    klass = rb_funcall(self, rb_intern("type"), 0);

    /* Create a shallow copy of the passed arguments. */
    arguments = rb_ary_dup(argv);

    /* Prepend an empty element as a placeholder. */
    rb_ary_unshift(arguments, Qnil);

    /* If we were supplied a block, enumerate the entire list. */
    for (current = collection; current != NULL; current = g_list_next(current)) {
        /* TODO: Find a better workaround than this or Data_Wrap_Struct. */
        item = rb_class_new_instance_with_data(0, NULL, klass, current->data);
        rb_ary_store(arguments, 0, item);
        rb_yield_splat(arguments);
    }

    return self;
}

/*
 * :nodoc:
 */
static VALUE rbpod_collection_type(VALUE self)
{
    /* We should get an error if we try to create an instance of NilClass. */
    return Qnil;
}

/*
 * :nodoc:
 */
static VALUE rbpod_collection_included(VALUE self, VALUE other)
{
    /* Collections should be Enumerable and Comparable. */
    rb_include_module(other, rb_mEnumerable);
    rb_include_module(other, rb_mComparable);

    /* Override Enumerable with our methods. */
    rb_extend_object(other, self);

    return self;
}

void Init_rbpod_collection(void)
{
#if RDOC_CAN_PARSE_DOCUMENTATION
    mRbPod = rb_define_module("RbPod");
#endif
    mRbPodCollection = rb_define_module_under(mRbPod, "Collection");

    rb_define_singleton_method(mRbPodCollection, "included", rbpod_collection_included, 1);

    rb_define_private_method(mRbPodCollection, "type", rbpod_collection_type, 0);

    rb_define_method(mRbPodCollection, "each", rbpod_collection_each, -2);
    rb_define_method(mRbPodCollection, "size", rbpod_collection_size, 0);

    rb_define_method(mRbPodCollection, "first", rbpod_collection_first, 0);
    rb_define_method(mRbPodCollection, "last", rbpod_collection_last, 0);

    rb_define_method(mRbPodCollection, "[]", rbpod_collection_get, 1);
}
