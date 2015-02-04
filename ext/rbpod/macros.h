/* macros.h */

#ifndef RBPOD_MACROS_H
#define RBPOD_MACROS_H

#define BooleanValue(value) (value) ? Qtrue : Qfalse

#define TYPED_DATA_PTR(self, type) ((type *) DATA_PTR(self))

#define RETURN_CUSTOMIZED_ENUMERATOR(obj, argc, argv, module)                \
    do {                                                                     \
        if (rb_block_given_p() == FALSE)                                     \
            return rb_custom_enumeratorize((obj), (argc), (argv), (module)); \
    } while(0)

#define DEF_ATTR_READER_STRING(c_type, prefix, field) \
    static VALUE rbpod_##prefix##_##field##_get(VALUE self) \
    { \
        c_type *ptr = TYPED_DATA_PTR(self, c_type); \
        return (ptr->field == NULL) ? Qnil : rb_str_new2(ptr->field); \
    }

#define DEF_ATTR_WRITER_STRING(c_type, prefix, field) \
    static VALUE rbpod_##prefix##_##field##_set(VALUE self, VALUE value) \
    { \
        c_type *ptr = TYPED_DATA_PTR(self, c_type); \
        gchar *previous = (gchar *) ptr->field; \
        ptr->field = StringValueCStr(value); \
        if (previous != NULL) { \
            g_free(previous); \
        } \
        return Qnil; \
    }

#define DEF_ATTR_READER_FIXNUM(c_type, prefix, field) \
    static VALUE rbpod_##prefix##_##field##_get(VALUE self) \
    { \
        c_type *ptr = TYPED_DATA_PTR(self, c_type); \
        return INT2NUM(ptr->field); \
    }

#define DEF_ATTR_WRITER_FIXNUM(c_type, prefix, field) \
    static VALUE rbpod_##prefix##_##field##_set(VALUE self, VALUE value) \
    { \
        c_type *ptr = TYPED_DATA_PTR(self, c_type); \
        ptr->field = NUM2INT(value); \
        return Qnil; \
    }

#define DEF_ATTR_READER_BOOLEAN(c_type, prefix, field) \
    static VALUE rbpod_##prefix##_##field##_get(VALUE self) \
    { \
        c_type *ptr = TYPED_DATA_PTR(self, c_type); \
        return (ptr->field == 0) ? Qfalse : Qtrue; \
    }

#define DEF_ATTR_WRITER_BOOLEAN(c_type, prefix, field) \
    static VALUE rbpod_##prefix##_##field##_set(VALUE self, VALUE value) \
    { \
        c_type *ptr = TYPED_DATA_PTR(self, c_type); \
        ptr->field = (RTEST(value)) ? 1 : 0; \
        return Qnil; \
    }

#define DEF_ATTR_READER_DATETIME(c_type, prefix, field) \
    static VALUE rbpod_##prefix##_##field##_get(VALUE self) \
    { \
        c_type *ptr = TYPED_DATA_PTR(self, c_type); \
        VALUE timestamp = INT2NUM(ptr->field); \
        if (ptr->field == 0) { \
            return Qnil; \
        } \
        return rb_funcall(rb_cTime, rb_intern("at"), 1, timestamp); \
    }

#define DEF_ATTR_WRITER_DATETIME(c_type, prefix, field) \
    static VALUE rbpod_##prefix##_##field##_set(VALUE self, VALUE value) \
    { \
        c_type *ptr = TYPED_DATA_PTR(self, c_type); \
        ptr->field = rb_funcall(value, rb_intern("to_i"), 0); \
        return Qnil; \
    }

#define DEF_ATTR_ACCESSOR(c_type, prefix, ruby_type, field) \
    DEF_ATTR_READER_##ruby_type(c_type, prefix, field); \
    DEF_ATTR_WRITER_##ruby_type(c_type, prefix, field)

#define DCL_ATTR_WRITER(ruby_class, prefix, name, field) \
    rb_define_method(ruby_class, #name "=", rbpod_##prefix##_##field##_set, 1)

#define DCL_ATTR_PREDICATE(ruby_class, prefix, name, field) \
    rb_define_method(ruby_class, #name "?", rbpod_##prefix##_##field##_get, 0)

#define DCL_ATTR_READER(ruby_class, prefix, name, field) \
    rb_define_method(ruby_class, #name, rbpod_##prefix##_##field##_get, 0)

#define DCL_ATTR_P(ruby_class, prefix, name, field) \
    DCL_ATTR_PREDICATE(ruby_class, prefix, name, field); \
    DCL_ATTR_WRITER(ruby_class, prefix, name, field)

#define DCL_ATTR(ruby_class, prefix, name, field) \
    DCL_ATTR_READER(ruby_class, prefix, name, field); \
    DCL_ATTR_WRITER(ruby_class, prefix, name, field)

#endif /* RBPOD_MACROS_H */
