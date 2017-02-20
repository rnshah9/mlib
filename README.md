M\*LIB: a Generic type-safe Container Library in C language
==========================================================

Overview
--------

M\*LIB (M star lib) is a meta library allowing the programmer to use **generic but
type safe container** in pure C language, aka handling generic containers.
The objects within the containers can have proper constructor, destructor
(and other methods):
this will be handled by the library. This allows to construct fully
recursive objects (using container of container of objects).

This is more or less an equivalent to the C++ STL.

M\*LIB should be portable to any systems that support C99 (or C11
for some rare features).
Compatibility with C++ was not aimed (as the library is totally 
useless in C++ - use the STL instead) but is more or less kept.

M\*LIB is only composed of a set of headers.
There is no C file. Just put the header in the search path of your compiler,
and it will work.
So there is no dependency (except some other headers of M\*LIB).

One of M\*LIB's design key is to ensure safety. This is done by multiple means:

* in debug mode, the contracts of the function are checked, ensuring
that the data are not corrupted.
* very few casts are used within the library. Still the library can be used
with the greatest level of warnings by the compiler without
any aliasing warning.
* the genericity is not done directly by macro, but indirectly by making them
define inline functions with the proper prototypes: this allows
the calls to have proper warning checks and proper debugging.

M\*LIB should still be quite-efficient: even if the implementation may not always be state
of the art, there is no overhead in using this library rather than using
direct C low-level access: the compiler is able to **fully** optimize
the library calls
since all the functions are declared as inline and the library gives
the compiler some hints on how to optimize even further.

M\*LIB uses internally the 'malloc', 'realloc' and 'free' functions to handle
the memory pool. This behavior can be overridden.
M\*LIB default policy is to abort the 
program if there is a memory error. However, this behavior
can also be customized.

M\*LIB may use a lot of assertions in its implementation to ensure safety: 
it is highly recommended to properly define NDEBUG for released programs. 
Otherwise you may have a noticeable slow-down (up to a factor x10 in some cases).

M\*LIB is distributed under BSD-2 simplified license.

It is strongly advised not to read the source to know how to use the library
but rather read the examples.

Components
----------

The available containers which doesn't require the user structure to be modified are:

* m-array.h: header for creating array of generic type and of variable size,
* m-list.h: header for creating single-linked list of generic type,
* m-dict.h: header for creating generic dictionary of generic types,
* m-tuple.h: header for creating arbitrary tuple of generic type,
* m-rbtree.h: header for creating binary sorted tree (RED-BLACK),
* m-variant.h: header for creating arbitrary variant of generic type TODO,
* m-set.h: header for creating set TODO,
* m-btree.h: header for creating B-TREE TODO,

The available containers of M\*LIB for thread synchronization are:

* m-buffer.h: header for creating fixed-size queue (or stack) of generic type,
* m-pbuffer.h: header for creating fixed-size priority queue of generic type TODO,
* m-mbuffer.h: header for creating fixed-size merge queue of generic type TODO,
* m-snapshot: header for creating 'snapshot' buffer for passing data between a producer and multiple consumers running at different rates (thread safe). It ensures that the consumer only sees complete data with minimal lag, but the consumer doesn't expect to see every piece of data.
* m-shared.h: header for creating shared pointer of generic type.

The following containers are intrusive (You need to modify your structure):

* m-i-list.h: header for creating dual-linked intrusive list of generic type,
* m-i-shared.h: header for creating intrusive shared pointer of generic type (Thread Safe),


Other headers offering other functionality are:

* m-bitset.h: header for creating bit set,
* m-string.h: header for creating variable-length string,
* m-core.h: header for meta-programming with the C preprocessor.
* m-mutex.h: header for providing a very thin layer accross multiple implementation of mutex/threads.
* m-algo.h: header for providing various generic algorithms to the previous containers.
* m-register.h: header for static polymorphic TODO

Each containers define their iterators.


External Sources
----------------

Many other implementation of generic container libraries exist in C. For example:

* [KLIB](https://github.com/attractivechaos/klib)
* [SGLIB](http://sglib.sourceforge.net/)
* [GLIB](https://wiki.gnome.org/Projects/GLib)
* [GDB internal library](https://sourceware.org/git/gitweb.cgi?p=binutils-gdb.git;a=blob;f=gdb/common/vec.h;h=41e41b5b22c9f5ec14711aac35ce4ae6bceab1e7;hb=HEAD)
* [UTHASH](http://troydhanson.github.io/uthash/index.html)
* [LIBNIH](https://github.com/keybuk/libnih)
* [CCL](http://www.cs.virginia.edu/~lcc-win32/ccl/ccl.html)
* [Generic-Container-Lib](https://github.com/Siapran/Generic-Container-Lib)
* [qlibc](http://wolkykim.github.io/qlibc/)
* [cello](http://libcello.org/)
* [Red-black tree implementation](http://www.canonware.com/rb/)
* [BSD tree.h](http://fxr.watson.org/fxr/source/sys/tree.h)
* [GDSL - The Generic Data Structures Library](http://home.gna.org/gdsl/)
* [LIBCONTAINER](http://www.agottem.com/libcontainer)
* [LIBCOLLECTION](https://bitbucket.org/manvscode/libcollections)

Each of theses can be classified into:

* Everything is a pointer to void,
* Macros are used to access structures in a generic way,
* Macros are used to generate C code.

M\*LIB's category is the last one. Some macros are also defined to access structure in a generic way, but they are optional.
M\*LIB main added value compared to other libraries is its op-list support allowing for proper recursive definition of function
(list of array of dictionary are perfectly supported by M\*LIB).


How to use
----------

To use these data structures, you include the desired header,
instantiate the definition of the structure and its associated methods by using a macro _DEF.
Then you use the defined functions. Let's see an example which is rather low-level (all methods are explicit):

    #include <stdio.h>
    #include "m-list.h"
    LIST_DEF(uint, unsigned int)      /* Define struct list_uint_t and its methods */
    int main(void) {
       list_uint_t list ;             /* list_uint_t has been define above */
       list_uint_init(list);          /* All type needs to be initialized */
       list_uint_push_back(list, 42); /* Push 42 in the list */
       list_uint_push_back(list, 17); /* Push 17 in the list */
       list_it_uint_t it;             /* Define an iterator to scan each one */
       for(list_uint_it(it, list)     /* Start iterator on first element */
           ; !list_uint_end_p(it)     /* Until the end is not reached */
           ; list_uint_next(it)) {    /* Set the iterator to the next element*/
              printf("%d\n",          /* Get a reference to the underlying */
                *list_uint_cref(it)); /* data and print it */
       }
       list_uint_clear(list);         /* Clear all the list */
    }

This is equivalent to this C++ program using the STL:

    #include <iostream>
    #include <list>
    typedef std::list<unsigned int> list_uint_t;
    typedef std::list<unsigned int>::iterator list_it_uint_t;
    int main(void) {
       list_uint_t list ;             /* list_uint_t has been define above */
       list.push_back(42);            /* Push 42 in the list */
       list.push_back(17);            /* Push 17 in the list */
       for(list_it_uint_t it = list.begin()  /* Iterator is first element*/
           ; it != list.end()         /* Until the end is not reached */
           ; ++it) {                  /* Set the iterator to the next element*/
           std::cout << *it << '\n';  /* Print the underlying data */
       }
    }

As you can see, this is rather equivalent with the following remarks:

* M\*LIB code is more verbose in the method name,
* M\*LIB needs explicit construction and destruction (as plain old C requests),
* M\*LIB doesn't return a value to the underlying data but a pointer to this value:
  - this was done for performance (it avoids copying all the data within the stack)
  - and for generality reasons (some structure may not allow copying data).

Note: list\_uint\_t is internally defined as an array of structure of size 1.
This has the following advantages:

* you effectively reserve the data whenever you declare a variable,
* you pass automatically the variable per reference for function calls,
* you can not copy the variable by an affectation (you have to use the API instead).

The same program written in C++11 can be a little bit simpler:

	#include <iostream>
	#include <list>
	typedef std::list<unsigned int> list_uint_t;
	int main(void) {
	  list_uint_t list ;             /* list_uint_t has been define above */
	  list.push_back(42);            /* Push 42 in the list */
	  list.push_back(17);            /* Push 17 in the list */
	  for(auto item: list) {
	    std::cout << item << '\n';   /* Print the underlying data */
	  }
	}

You can also condense the M\*LIB code by using the EACH & LET macros if you are not afraid of using syntactic macros:

    #include <stdio.h>
    #include "m-list.h"
    LIST_DEF(uint, unsigned int)        /* Define struct list_uint_t and its methods */
    int main(void) {
       M_LET(list, LIST_OPLIST(uint)) { /* Define & init list as list_uint_t */
         list_uint_push_back(list, 42); /* Push 42 in the list */
         list_uint_push_back(list, 17); /* Push 17 in the list */
         for M_EACH(item, list, LIST_OPLIST(uint)) {
           printf("%d\n", *item);       /* Print the item */
         }
       }                                /* Clear of list will be done now */
    }


You may not use the internal fields of the structure.
Internal fields of the structure are subject to change even for small revision
of the library.

When using M\*LIB, sometimes (often) the list of errors/warnings generated by the compiler can be (very) huge (specially with latest compilers),
even if the error itself is minor. This is more or less the same as the use of template in C++.
You should focus mainly on the first reported error/warning
even if the link between what the compiler said and what the error is is not immediate:
for example, a lack of inclusion of an header, an unkown oplist, a missing argument, a missing method in an oplist, ...

The final goal of the library is to be able to write code like this in pure C while keeping type safety and compile time name resolution:

	let(list, list_uint_t) {
	  push(list, 42);
	  push(list, 17);
	  for each (item, list) {
	    print(item, "\n");
	  }
	}


OPLIST
------

An OPLIST is a fundamental notion of M\*LIB which hasn't be seen in any other library.
In order to know how to operate on a type, M\*LIB needs additional information
as the compiler doesn't know how to handle properly any type (contrary to C++).
This is done by giving an operator list (or oplist in short) to any macro which
needs to handle the type. As such, an oplist as only meaning within a macro
expansion. Fundamentally, this is yet another form of polymorphic.

An oplist is an *unordered* list of operator in the following format:

(OPERATOR1(function1), OPERATOR2(function2), ...)

It is used to perform the association between the operation on a type
and the function which performs this operation.
Without an oplist, M\*LIB has no way to known how to deal with your type
and will deal with it like a classic C type.

A function name can be followed by M_IPTR in the oplist (for example: INIT(init_func M_IPTR) )
to specify that the function accept as its *first* argument a pointer to the type rather than the type itself
(aka the prototype is init_func(type *) instead of init_func(type)).
If you use the '[1]' trick (see below), you won't need this.

An oplist has no real form from a C language point of view. It is just an abstraction
which disappears after the macro expansion step of the preprocessing.

For each object / container, an oplist is needed and the following operators are
usually expected for an object:

* INIT constructor(obj): initialize the object into a valid state.
* INIT\_SET constructor(obj, org): initialize the object into the same state as org.
* SET operator(obj, org): set the initialized object obj into the same state as the initialized object org.
* CLEAR destructor(obj): destroy the initialized object. This method shall never fail.

Theses methods shall only fail due to ***memory errors***.

Not all operators are needed within an oplist to handle a container.
If some operator is missing, the associated default operator of the function is used.
To use C integer or float types, the default constructors are perfectly fine:
you may use M\_DEFAULT\_OPLIST to define the operator list of such types or you
can just omit it.

An iterator doesn't have a constructor nor destructor methods.

Other operators are:

* NEW () -> type pointer: alloc a new object suitable aligned. The returned object is not initialized. INIT operator shall be called.
* DEL (&obj) : free the allocated uninitialized object 'obj' (default is free). The object is not cleared before being free.
* INIT\_MOVE(objd, objc): Initialize 'objd' to 'objc' by stealing as resources as possible from 'objc' and then clear 'objc'. It is equivalent to calling INIT\_SET(objd,objc) then CLEAR(objc) (but usually way faster).
* MOVE(objd, objc): Set 'objd' to 'objc' by stealing as resources as possible from 'objc' and then clear 'objc'. It is equivalent to calling SET(objd,objc) then CLEAR(objc) or CLEAR(objd) and then INIT\_MOVE(objd, objc).
* HASH (obj) --> size_t: return a hash of the object (used for hash table). Default is performing a hash of the memory representation of the object.
* EQUAL(obj1, obj2) --> bool: return true if both objects are equal, false otherwise. Default is using memcmp.
* CMP(obj1, obj2) --> int: return a negative integer if obj1 < obj2, 0 if obj1 = obj2, a positive integer otherwise. Default is C comparison operators.
* ADD(obj1, obj2, obj3) : set obj1 to the sum of obj2 and obj3. Default is '+' C operator.
* SUB(obj1, obj2, obj3) : set obj1 to the difference of obj2 and obj3. Default is '-' C operator.
* MUL(obj1, obj2, obj3) : set obj1 to the product of obj2 and obj3. Default is '*' C operator.
* DIV(obj1, obj2, obj3) : set obj1 to the division of obj2 and obj3. Default is '/' C operator.
* TYPE() --> type: return the type associated to this oplist.
* SUBTYPE() --> type: return the type of the element stored in the container.
* OPLIST() --> oplist: return the oplist of the type of the elements stored in the container.
* IT\_TYPE() --> type: return the type of an iterator object.
* IT\_FIRST(it\_obj, container): set the object iterator it\_obj to the first sub-element of container.
* IT\_LAST(it\_obj, container): set the object iterator it\_obj to the last sub-element of container.
* IT\_SET(it\_obj, it\_obj2): set the object iterator it\_obj to it\_obj2.
* IT\_END\_P(it\_obj)--> bool: return true if it\_obj is the end has been reached.
* IT\_LAST\_P(it\_obj)--> bool: return true if it_obj is the iterator end has been reached or if the iterator points to the last element.
* IT\_EQUAL\_P(it\_obj, it\_obj2) --> bool: return true if both iterators points to the same element.
* IT\_NEXT(it\_obj): move the iterator to the next sub-component.
* IT\_PREVIOUS(it\_obj): move the iterator to the previous sub-component.
* IT\_CREF(it\_obj) --> &obj: return a const pointer to the referenced object.
* IT\_REF(it\_obj) --> &obj: return a pointer to the referenced object.
* OUT\_STR(FILE* f, obj): Output 'obj' as a string into the FILE stream f.
* IN\_STR(obj, FILE* f) --> bool: Set 'obj' to the string object in the FILE stream f. Return true in case of success, false otherwise.
* GET\_STR(string_t str, obj, bool append): Set 'str' to a string representation of the object 'obj'. Append to the string if 'append' is true.
* UPDATE(dest, src): Update 'dest' with 'src'. What it does exactly is node dependent: it can either SET or ADD to the node the new 'src' (default is SET).

More operators are expected.

Example:

        (INIT(mpz_init),SET(mpz_set),INIT_SET(mpz_init_set),CLEAR(mpz_clear))

If there is two operations with the same name in an oplist, the left one has the priority. This allows partial overiding.


Memory Allocation
-----------------

Memory Allocation functions can be set by overriding the following macros before using the _DEF macros:

* M\_MEMORY\_ALLOC (type): return a pointer to a new object of type 'type'.
* M\_MEMORY\_REALLOC (type, ptr, size): return a pointer to an array of 'size' object of type 'type', reusing the old array pointed by 'ptr'. 'ptr' can be NULL, in which case the array will be created.
* M\_MEMORY\_FREE (ptr): free the object (or array of object) pointed by 'ptr'.

M\_MEMORY\_ALLOC and  M\_MEMORY\_REALLOC are supposed to return NULL in case of memory allocation failure.
The defaults are 'malloc', 'realloc' and 'free'.


Out-of-memory error
-------------------

When a memory exhaustion is reached, the global macro "M\_MEMORY\_FULL" is called and the function returns immediately after.
The object remains in a valid (if previously valid) and unchanged state in this case.
By default, the macro prints an error message and abort the program: handling non-trivial memory errors is hard,
testing such cases is mandatory to avoid security holes, testing them is even harder, 
and this behavior is rather conservative.
It can be overloaded to handle other policy for error handling like:

* throwing an error (in which case, the user is responsible to free memory of the allocated objects - destructor can still be called),
* set a global error and handle it when the function returns,
* other policy.

This function takes the size in bytes of the memory which has been tried to be allocated.

If needed, this macro shall be defined ***prior*** to instantiate the structure.



API Documentation
-----------------

###M-LIST

This header is for creating single-linked list.

####LIST\_DEF(name, type, [, oplist])

Define the list 'list\_##name##\_t' which contains the objects of type 'type' and its associated methods as "static inline" functions.
'name' shall be a C identifier which will be used to identify the list. It will be used to create all the types and functions to handle the container.
It shall be done once per type and per compilation unit.
It also define the iterator list\_it\_##name##\_t and its associated methods as "static inline" functions.

The object 'oplist' is expected to have at least the following operators (INIT, INIT_SET, SET and CLEAR), otherwise default operators are used. If there is no given oplist, the default operators are also used. The created methods will use the operators to init, set and clear the contained object.

For this structure, the back is always the first element, and the front is the last element: the list grows from the back.

Example:

	LIST_DEF(mpz, mpz_t,                                               \
		(INIT(mpz_init), INIT_SET(mpz_init_set), SET(mpz_set), CLEAR(mpz_clear)))

	list_mpz_t my_list;

	void f(mpz_t z) {
		list_mpz_push_back (my_list, z);
	}

####LIST\_OPLIST(name [, oplist])

Return the oplist of the list defined by calling LIST\_DEF with name & oplist. 

####Created methods

In the following methods, name stands for the name given to the macro which is used to identify the type.
The following types are automatically defined by the previous macro:

####list\_name\_t

Type of the list of 'type'.

####list\_it\_name\_t

Type of an iterator over this list.

The following methods are automatically and properly created by the previous macro.

#####void list\_name\_init(list\_name\_t list)

Initialize the list 'list' (aka constructor) to an empty list.

#####void list\_name\_init\_set(list\_name\_t list, const list\_name\_t ref)

Initialize the list 'list' (aka constructor) and set it to the value of 'ref'.

#####void list\_name\_set(list\_name\_t list, const list\_name\_t ref)

Set the list 'list' to the value of 'ref'.

#####void list\_name\_init\_move(list\_name\_t list, list\_name\_t ref)

Initialize the list 'list' (aka constructor) by stealing as many resources from 'ref' as possible.
After-wise 'ref' is cleared and can no longer be used.

#####void list\_name\_move(list\_name\_t list, list\_name\_t ref)

Set the list 'list' (aka constructor) by stealing as many resources from 'ref' as possible.
After-wise 'ref' is cleared and can no longer be used.

#####void list\_name\_clear(list\_name\_t list)

Clear the list 'list (aka destructor). The list can't be used anymore, except with a constructor.

#####void list\_name\_clean(list\_name\_t list)

Clean the list (the list becomes empty). The list remains initialized but is empty.

#####const type *list\_name\_back(const list\_name\_t list)

Return a constant pointer to the data stored in the back of the list.

#####void list\_name\_push\_back(list\_name\_t list, type value)

Push a new element within the list 'list' with the value 'value' contained within.

#####type *list\_name\_push\_raw(list\_name\_t list)

Push a new element within the list 'list' without initializing it and returns a pointer to the **non-initialized** data.
The first thing to do after calling this function is to initialize the data using the proper constructor. This allows to use a more specialized
constructor than the generic one.
Return a pointer to the **non-initialized** data.

#####type *list\_name\_push\_new(list\_name\_t list)

Push a new element within the list 'list' and initialize it with the default constructor of the type.
Return a pointer to the initialized object.

#####void list\_name\_pop\_back(type *data, list\_name\_t list)

Pop a element from the list 'list' and set *data to this value.

#####bool list\_name\_empty\_p(const list\_name\_t list)

Return true if the list is empty, false otherwise.

#####void list\_name\_swap(list\_name\_t list1, list\_name\_t list2)

Swap the list 'list1' and 'list2'.

#####void list\_name\_it(list\_it\_name\_t it, list\_name\_t list)

Set the iterator 'it' to the back(=first) element of 'list'.
There is no destructor associated to this initialization.

#####void list\_name\_it\set(list\_it\_name\_t it, const list\_it\_name\_t ref)

Set the iterator 'it' to the iterator 'ref'.
There is no destructor associated to this initialization.

#####bool list\_name\_end\_p(const list\_it\_name\_t it)

Return true if the iterator doesn't reference a valid element anymore.

#####bool list\_name\_last\_p(const list\_it\_name\_t it)

Return true if the iterator references the top(=last) element or if the iterator doesn't reference a valid element anymore.

#####bool list\_name\_it\_equal\_p(const list\_it\_name\_t it1, const list\_it\_name\_t it2)

Return true if the iterator it1 references the same element than it2.

#####void list\_name\_next(list\_it\_name\_t it)

Move the iterator 'it' to the next element of the list, ie. from the back (=first) element to the front(=last) element.

#####type *list\_name\_ref(list\_it\_name\_t it)

Return a pointer to the element pointed by the iterator.
This pointer remains valid until the list is modified by another method.

#####const type *list\_name\_cref(const list\_it\_name\_t it)

Return a constant pointer to the element pointed by the iterator.
This pointer remains valid until the list is modified by another method.

#####type *list\_name\_get(const list\_name\_t list, size\_t i)

Return a pointer to the element i-th of the list (from 0). 
It is assumed than i is within the size of the list.

#####const type *list\_name\_cget(const list\_name\_t list, size\_t i)

Return a constant pointer to the element i-th of the list (from 0). 
It is assumed than i is within the size of the list.

#####size\_t list\_name\_size(const list\_name\_t list)

Return the number elements of the list (aka size). Return 0 if there no element.

#####void list\_name\_insert(list\_name\_t list, const list\_it\_name\_t it, const type x)

Insert 'x' after the position pointed by 'it' (which is an iterator of the list 'list') or if 'it' doesn't point anymore to a valid element of the list, it is added as the back (=first) element of the 'list'

#####void list\_name\_remove(list\_name\_t list, list\_it\_name\_t it)

Remove the element 'it' from the list 'list'.
After wise, 'it' points to the next element of the list.

#####void list\_name\_move\_back(list\_name\_t list1, list\_name\_t list2, list\_it\_name\_t it)

Move the element pointed by 'it' (which is an iterator of 'list2') from the list 'list2' to the back position of 'list1'.
After wise, 'it' points to the next element of 'list2'.

#####void list\_name\_slice(list\_name\_t list1, list\_name\_t list2)

Move all the element of 'list2' into 'list1".
After-wise, 'list2' is emptied.

#####void list\_name\_reverse(list\_name\_t list)

Reverse the order of the list.

#####void list\_name\_get\_str(string\_t str, const list\_name\_t list, bool append)

Generate a string representation of the list 'list' and set 'str' to this representation
(if 'append' is false) or append 'str' with this representation (if 'append' is true).
This method is only defined if the type of the element defines a GET\_STR method itself.

#####void list\_name\_out\_str(FILE *file, const list\_name\_t list)

Generate a string representation of the list 'list' and outputs it into the FILE 'file'.
This method is only defined if the type of the element defines a OUT\_STR method itself.

#####void list\_name\_in\_str(FILE *file, const list\_name\_t list)

Read from the file 'file' a string representation of a list and set 'list' to this representation.
This method is only defined if the type of the element defines a IN\_STR method itself.

#####bool list\_name\_equal\_p(const list\_name\_t list1, const list\_name\_t list2)

Return true if both lists 'list1' and 'list2' are equal.
This method is only defined if the type of the element defines a EQUAL method itself.



###M-ARRAY

####ARRAY\_DEF(name, type [, oplist])

Define the array 'array\_##name##\_t' which contains the objects of type 'type' and its associated methods as "static inline" functions.
An array is a collection of element which are individually indexable.
Compared to C arrays, the created methods handle automatically the size (aka growable array).
'name' shall be a C identifier which will be used to identify the container.

It also define the iterator array\_it\_##name##\_t and its associated methods as "static inline" functions.

The object oplist is expected to have at least the following operators (INIT, INIT_SET, SET and CLEAR), otherwise default operators are used. If there is no given oplist, the default operators are also used. The created methods will use the operators to init, set and clear the contained object.

Example:

	ARRAY_DEF(mpfr_t, mpfr,                                                                  \
	   (INIT(mpfr_init), INIT_SET(mpfr_init_set), SET(mpfr_set), CLEAR(mpfr_clear)))

	array_mpfr_t my_array;

	void f(mpfr_t z) {
		array_mpfr_push_back (my_array, z);
	}


####ARRAY\_OPLIST(name [, oplist])

Return the oplist of the array defined by calling ARRAY\_DEF with name & oplist. 


####Created methods

In the following methods, name stands for the name given to the macro which is used to identify the type.
The following types are automatically defined by the previous macro:

####array\_name\_t

Type of the array of 'type'.

####array\_it\_name\_t

Type of an iterator over this array.

The following methods are automatically and properly created by the previous macros:

#####void array\_name\_init(array\_name\_t array)

Initialize the array 'array' (aka constructor) to an empty array.

#####void array\_name\_init\_set(array\_name\_t array, const array\_name\_t ref)

Initialize the array 'array' (aka constructor) and set it to the value of 'ref'.

#####void array\_name\_set(array\_name\_t array, const array\_name\_t ref)

Set the array 'array' to the value of 'ref'.

#####void array\_name\_init\_move(array\_name\_t array, array\_name\_t ref)

Initialize the array 'array' (aka constructor) by stealing as many resources from 'ref' as possible.
After-wise 'ref' is cleared.

#####void array\_name\_move(array\_name\_t array, array\_name\_t ref)

Set the array 'array' by stealing as many resources from 'ref' as possible.
After-wise 'ref' is cleared.

#####void array\_name\_clear(array\_name\_t array)

Clear the array 'array (aka destructor).

#####void array\_name\_clean(array\_name\_t array)

Clean the array (the array becomes empty but remains initialized).

#####void array\_name\_push\_back(array\_name\_t array, const type value)

Push a new element into the back of the array 'array' with the value 'value' contained within.

#####type *array\_name\_push\_new(array\_name\_t array)

Push a new element into the back of the array 'array' and initialize it with the default constructor.
Return a pointer to this element.

#####type *array\_name\_push\_raw(array\_name\_t array)

Push a new element within the array 'array' without initializing it and return
a pointer to the non-initialized data.
The first thing to do after calling this function is to initialize the data
using the proper constructor. This allows to use a more specialized
constructor than the generic one.

#####void array\_name\_push\_at(array\_name\_t array, size\_t key, const type x)

Push a new element into the position 'key' of the array 'array' with the value 'value' contained within.
'key' shall be a valid position of the array: from 0 to the size of array (included).

#####void array\_name\_pop\_back(type *data, array\_name\_t array)

Pop a element from the back of the array 'array' and set *data to this value.

#####const type *array\_name\_back(const array\_name\_t array)

Return a constant pointer to the last element of the array.

#####bool array\_name\_empty\_p(const array\_name\_t array)

Return true if the array is empty, false otherwise.

#####size\_t array\_name\_size(const array\_name\_t array)

Return the size of the array.

#####size\_t array\_name\_capacity(const array\_name\_t array)

Return the capacity of the array.

#####void array\_name\_swap(array\_name\_t array1, array\_name\_t array2)

Swap the array 'array1' and 'array2'.

#####void array\_name\_set\_at(array\_name\_t array, size\_t i, type value)

Set the element 'i' of array 'array' to 'value'.
'i' shall be within the size of the array.

#####void array\_name\_set\_at2(array\_name\_t array, size\_t i, type value)

Set the element 'i' of array 'array' to 'value', increasing the size
of the array if needed.

#####void array\_name\_resize(array\_name\_t array, size\_t size)

Resize the array 'array' to the size 'size' (initializing or clearing elements).

#####void array\_name\_shrink\_to\_fit(array\_name\_t array)

Resize the array 'array' to its exact needed size.

#####void array\_name\_pop\_at(type *dest, array\_name\_t array, size\_t key)

Set *dest to the value the element 'i' if dest is not NULL,
Remove this element from the array.
'key' shall be within the size of the array.

#####void array\_name\_remove(array\_name\_t array, size\_t key)

Remove the element 'key' from the array.
'key' shall be within the size of the array.

#####void array\_name\_remove\_v(array\_name\_t array, size\_t i, size\_t j)

Remove the element 'i' (included) to the element 'j' (excluded)
from the array.
'i' and 'j' shall be within the size of the array, and i < j.

#####void array\_name\_insert(array\_name\_t array, size\_t i, const type x)

Insert the object 'x' at the position 'key' of the array.
'key' shall be within the size of the array.

#####void array\_name\_insert\_v(array\_name\_t array, size\_t i, size\_t j)

Insert from the element 'i' (included) to the element 'j' (excluded)
new empty elements to the array.
'i' and 'j' shall be within the size of the array, and i < j.

#####type *array\_name\_get(array\_name\_t array, size\_t i)

Return a pointer to the element 'i' of the array.
'i' shall be within the size of the array.

#####const type *array\_name\_cget(const array\_name\_t it, size\_t i)

Return a constant pointer to the element 'i' of the array.
'i' shall be within the size of the array.

#####void array\_name\_it(array\_it\_name\_t it, array\_name\_t array)

Set the iterator 'it' to the first element of 'array'.

#####void array\_name\_it\_set(array\_it\_name\_t it1, array\_it\_name\_t it2)

Set the iterator 'it1' to 'it2'.

#####bool array\_name\_end\_p(array\_it\_name\_t it)

Return true if the iterator doesn't reference a valid element anymore.

#####bool array\_name\_last\_p(array\_it\_name\_t it)

Return true if the iterator references the last element of the array, or doesn't reference a valid element.

#####bool array\_name\_it\_equal\_p(const array\_it\_name\_t it1, const array\_it\_name\_t it2)

Return true if both iterators point to the same element.

#####void array\_name\_next(array\_it\_name\_t it)

Move the iterator 'it' to the next element of the array.

#####void array\_name\_previous(array\_it\_name\_t it)

Move the iterator 'it' to the previous element of the array.

#####type *array\_name\_ref(array\_it\_name\_t it)

Return a pointer to the element pointed by the iterator.
This pointer remains valid until the array is modified by another method.

#####const type *array\_name\_cref(const array\_it\_name\_t it)

Return a constant pointer to the element pointed by the iterator.
This pointer remains valid until the array is modified by another method.

#####void array\_name\_get\_str(string\_t str, const array\_name\_t array, bool append)

Generate a string representation of the array 'array' and set 'str' to this representation
(if 'append' is false) or append 'str' with this representation (if 'append' is true).
This method is only defined if the type of the element defines a GET\_STR method itself.

#####void array\_name\_out\_str(FILE *file, const array\_name\_t array)

Generate a string representation of the array 'array' and outputs it into the FILE 'file'.
This method is only defined if the type of the element defines a OUT\_STR method itself.

#####void array\_name\_in\_str(FILE *file, const array\_name\_t array)

Read from the file 'file' a string representation of a array and set 'array' to this representation.
This method is only defined if the type of the element defines a IN\_STR method itself.

#####bool array\_name\_equal\_p(const array\_name\_t array1, const array\_name\_t array2)

Return true if both arrays 'array1' and 'array2' are equal.
This method is only defined if the type of the element defines a EQUAL method itself.




###M-DICT

####DICT\_DEF2(name, key\_type, key\_oplist, value\_type, value\_oplist)

Define the dictionary 'dict\_##name##\_t' and its associated methods as "static inline" functions.
A dictionary (or associative array, map, symbol table) is an abstract data type
composed of a collection of (key, value) pairs,
such that each possible key appears at most once in the collection.
'name' shall be a C identifier which will be used to identify the container.

It shall be done once per type and per compilation unit.
It also define the iterator dict\_it\_##name##\_t and its associated methods as "static inline" functions.

The object oplists are expected to have the following operators (INIT, INIT\_SET, SET and CLEAR), otherwise default operators are used.
If there is no given oplist, the default operators are also used. The created methods will use the operators to init, set and clear the contained object.
The key_oplist shall also define the additional operators (HASH and EQUAL).

Interface is subjected to minor change.

Example:

	DICT_DEF2(str, string_t, STRING_OPLIST, string_t, STRING_OPLIST)
	dict_str_t my_dict;
	void f(string_t key, string_t value) {
		dict_str_set_at (my_dict, key, value);
	}


####DICT\_OPLIST(name, key\_oplist, value\_oplist)

Return the oplist of the dictionary defined by calling DICT\_DEF2 with name & key\_oplist & value\_oplist. 

####Created methods

In the following methods, name stands for the name given to the macro which is used to identify the type.
The following types are automatically defined by the previous macro:

#####dict\_name\_t

Type of the dictionary of 'key_type' --> 'value_type'.

#####dict\_it\_name\_t

Type of an iterator over this dictionary.

The following methods are automatically and properly created by the previous macro:

#####void dict\_name\_init(dict\_name\_t dict)

Initialize the dictionary 'dict' to be empty.

#####void dict\_name\_clear(dict\_name\_t dict)

Clear the dictionary 'dict'.

#####void dict\_name\_init\_set(dict\_name\_t dict, const dict\_name\_t ref)

Initialize the dictionary 'dict' to be the same as 'ref'.

#####void dict\_name\_set(dict\_name\_t dict, const dict\_name\_t ref)

Set the dictionary 'dict' to be the same as 'ref'.

#####void dict\_name\_init\_move(dict\_name\_t dict, dict\_name\_t ref)

Initialize the dictionary 'dict' by stealing as resource as possible
from 'ref' and clear 'ref'.

#####void dict\_name\_move(dict\_name\_t dict, dict\_name\_t ref)

Set the dictionary 'dict'  by stealing as resource as possible
from 'ref' and clear 'ref'.

#####void dict\_name\_clean(dict\_name\_t dict)

Clean the dictionary 'dict'. 'dict' remains initialized.

#####size\_t dict\_name\_size(const dict\_name\_t dict)

Return the number of elements of the dictionary.

#####value\_type dict\_name\_get(const dict\_name\_t dict, const key\_type key)

Return a pointer to the value associated to the key 'key' in dictionary
'dict' or NULL if the key is not found.

#####void dict\_name\_set\_at(dict\_name\_t dict, const key\_type key, const value\_type value)

Set the value referenced by key 'key' in the dictionary 'dict' to 'value'.

#####void dict\_name\_remove(dict\_name\_t dict, const key\_type key)

Remove the element referenced by key 'key' in the dictionary 'dict'.
Do nothing if 'key' is no present in the dictionary.

#####void dict\_name\_it(dict\_it\_name\_t it, dict\_name\_t dict)

Set the iterator 'it' to the first element of 'dict'.

#####void dict\_name\_it\_set(dict\_it\_name\_t it, const dict\_it\_name\_t ref)

Set the iterator 'it' to the same element than 'ref'.

#####bool dict\_name\_end\_p(const dict\_it\_name\_t it)

Return true if 'it' references no longer a valid element.

#####bool dict\_name\_last\_p(const dict\_it\_name\_t it)

Return true if 'it' references the last element if is no longer valid.

#####void dict\_name\_next(dict\_it\_name\_t it)

Update the iterator 'it' to the next element.

#####dict\_pair\_name\_t *dict\_name\_ref(dict\_it\_name\_t it)

Return a pointer to the pair composed by the key and its value.
'key' element can not be modified.
This pointer remains valid until the dictionary is modified by another method.

#####const dict\_pair\_name\_t *dict\_name\_ref(dict\_it\_name\_t it)

Return a constant pointer to the pair composed by the key and its value.
This pointer remains valid until the dictionary is modified by another method.

#####void dict\_name\_get\_str(string\_t str, const dict\_name\_t dict, bool append)

Generate a string representation of the dict 'dict' and set 'str' to this representation
(if 'append' is false) or append 'str' with this representation (if 'append' is true).
This method is only defined if the type of the element defines a GET\_STR method itself.

######void dict\_name\_out\_str(FILE *file, const dict\_name\_t dict)

Generate a string representation of the dict 'dict' and outputs it into the FILE 'file'.
This method is only defined if the type of the element defines a OUT\_STR method itself.

######void dict\_name\_in\_str(FILE *file, const dict\_name\_t dict)

Read from the file 'file' a string representation of a dict and set 'dict' to this representation.
This method is only defined if the type of the element defines a IN\_STR method itself.

######bool dict\_name\_equal\_p(const dict\_name\_t dict1, const dict\_name\_t dict2)

Return true if both dicts 'dict1' and 'dict2' are equal.
This method is only defined if the type of the element defines a EQUAL method itself.



###M-TUPLE

####TUPLE\_DEF2(name, (element1, type1, oplist1) [, ...])

Define the tuple 'tuple\_##name##\_t' and its associated methods as "static inline" functions.
A tuple is a finite ordered list of elements of different types. 
Each parameter of the macro is expected to be an element of the tuple.
Each element is defined by three parameters within parenthesis: 
the element name, the element type and the element oplist.
'name' and 'element' shall be a C identifier which will be used to identify the container.

This is more or less like a C structure. The main added value compared to using a struct
is that it generates also all the basic methods to handle it.
In fact, it generates a C struct with the given type and element.

It shall be done once per type and per compilation unit.

The object oplists are expected to have the following operators (INIT, INIT_SET, SET and CLEAR), otherwise default operators are used. If there is no given oplist, the default operators are also used. The created methods will use the operators to init, set and clear the contained object.

The interface is subjected to change.

Example:

	TUPLE_DEF2(pair, (key, string_t, STRING_OPLIST),
			 (value, mpz_t, (INIT(mpz_init), INIT_SET(mpz_init_set), SET(mpz_set), CLEAR(mpz_clear) )))
	void f(void) {
		pair_t p1;
		pair_init (p1);
		string_set_str(p1->key, "HELLO");
		mpz_set_str(p1->value, "1742", 10);
		pair_clear(p1);
	}

####TUPLE\_OPLIST(name, oplist1[, ...] )

Return the oplist of the tuple defined by calling TUPLE\_DEF2 with the given name & the oplists.

####Created methods

In the following methods, name stands for the name given to the macro which is used to identify the type.
The following types are automatically defined by the previous macro:

####name\_t

Type of the defined tuple.

The following methods are automatically and properly created by the previous macros:

#####void name\_init(name\_t tuple)

Initialize the tuple 'tuple' (aka constructor) to an empty tuple.

#####void name\_init\_set(name\_t tuple, const name\_t ref)

Initialize the tuple 'tuple' (aka constructor) and set it to the value of 'ref'.

#####void name\_init\_set2(name\_t tuple, const type1 element1[, ...])

Initialize the tuple 'tuple' (aka constructor) and set it to the value of the tuple ('element1'[, ...]).

#####void name\_set(name\_t tuple, const name\_t ref)

Set the tuple 'tuple' to the value of 'ref'.

#####void name\_init\_move(name\_t tuple, name\_t ref)

Initialize the tuple 'tuple' (aka constructor) by stealing as many resources from 'ref' as possible.
After-wise 'ref' is cleared.
This method is created only if all oplists of the tuple define INIT\_MOVE method.

#####void name\_move(name\_t tuple, name\_t ref)

Set the tuple 'tuple' by stealing as many resources from 'ref' as possible.
After-wise 'ref' is cleared.
This method is created only if all oplists of the tuple define MOVE method.

#####void name\_clear(name\_t tuple)

Clear the tuple 'tuple (aka destructor).

#####const type1 *name\_get\_element1(const name\_t tuple)

Return a constant pointer to the element 'element1' of the tuple.
There is as many methods as there are elements.

#####void name\_set\_element1(name\_t tuple, type1 element1)

Set the element of the tuple to 'element1'
There is as many methods as there are elements.

#####int name\_set\_cmp(const name\_t tuple1, const name\_t tuple2)

Compare 'tuple1' to 'tuple2' using lexicographic order.
This method is created only if all oplists of the tuple define CMP method.

#####int name\_set\_equal\_p(const name\_t tuple1, const name\_t tuple2)

Return true if 'tuple1' and 'tuple2' are identical.
This method is created only if all oplists of the tuple define EQUAL method.

#####void name\_get\_str(string\_t str, const name\_t tuple, bool append)

Generate a string representation of the tuple 'tuple' and set 'str' to this representation
(if 'append' is false) or append 'str' with this representation (if 'append' is true).
This method is only defined if all oplists define a GET\_STR method.

#####void name\_out\_str(FILE *file, const name\_t tuple)

Generate a string representation of the tuple 'tuple' and outputs it into the FILE 'file'.
This method is only defined if all oplists define a OUT\_STR method.

#####void name\_in\_str(FILE *file, const name\_t tuple)

Read from the file 'file' a string representation of a tuple and set 'tuple' to this representation.
This method is only defined if all oplists define a IN\_STR method.


###M-RBTREE

####RBTREE\_DEF(name, type[, oplist])

Define the binary ordered tree 'rbtree\_##name##\_t' and its associated methods as "static inline" functions.
A binary tree is a tree data structure in which each node has at most two children, which are referred to as the left child and the right child.
All elements of the tree are totally ordered.
It shall not be confused with a B-TREE.
'name' shall be a C identifier which will be used to identify the container.

The CMP operator is used to perform the total ordering of the elements.

It shall be done once per type and per compilation unit.
It also define the iterator rbtree\_it\_##name##\_t and its associated methods as "static inline" functions.

The object oplist is expected to have the following operators (INIT, INIT_SET, SET, CMP and CLEAR), otherwise default operators are used. If there is no given oplist, the default operators are also used. The created methods will use the operators to init, set and clear the contained object.

Example:

	RBTREE_DEF(uint, unsigned int)
	void f(unsigned int num) {
		rbtree_uint_t tree;
		rbtree_uint_init(tree);
		for(unsigned int i = 0; i < num; i++)
			rbtree_uint_push(tree, i);
		rbtree_uint_clear(tree);                              
	}


####Created methods

The following methods are automatically and properly created by the previous macros. In the following methods, name stands for the name given to the macro which is used to identify the type.

TODO:


###M-BUFFER

####BUFFER\_DEF(name, type, size, policy[, oplist])

Define the buffer 'buffer\_##name##\_t' and its associated methods as "static inline" functions.
A buffer is a fixed size queue or stack.
'name' shall be a C identifier which will be used to identify the container.

The size parameter defined the fixed size of the queue. It can be 0, in which case, the fixed size will be defined at initialization time.

Multiple additional policy can be applied to the buffer by performing a logical or of the following properties:

* BUFFER\_QUEUE : define a FIFO queue (default),
* BUFFER\_STACK : define a stack (exclusive with BUFFER\_QUEUE),
* BUFFER\_BLOCKING : define blocking calls for push/pop (default),
* BUFFER\_UNBLOCKING : define unblocking calls for push/pop,
* BUFFER\_THREAD\_SAFE : define thread safe functions (default),
* BUFFER\_THREAD\_UNSAFE : define thread unsafe functions,
* BUFFER\_PUSH\_INIT\_POP\_MOVE : change the behavior of PUSH as pushing a new object, and POP as moving this new object into the new emplacement (this is mostly used for performance reasons or to handle properly a shared_ptr semantic). In practice, it works as if POP performs the initialization of the object, 
* BUFFER\_PUSH\_OVERWRITE : PUSH will always overwrite the first entry (this is mostly used to reduce latency).

This container is designed to be used for easy synchronization inter-threads (and the variable shall be a global one).

It shall be done once per type and per compilation unit.
It also define the iterator buffer\_it\_##name##\_t and its associated methods as "static inline" functions.

The object oplist is expected to have the following operators (INIT, INIT\_SET, SET, INIT\_MOVE and CLEAR), otherwise default operators are used. If there is no given oplist, the default operators are also used. The created methods will use the operators to init, set and clear the contained object.

Example:

	BUFFER_DEF(uint, unsigned int, 10, BUFFER_QUEUE|BUFFER_BLOCKING)

	buffer_uint_t g_buff;

	void f(unsigned int i) {
		buffer_uint_init(g_buff, 10);
		buffer_uint_push(g_buff, i);
		buffer_uint_pop(&i, g_buff);
		buffer_uint_clear(g_buff);
	}


####Created methods

The following methods are automatically and properly created by the previous macros. In the following methods, name stands for the name given to the macro which is used to identify the type.

TODO:


###M-SNAPSHOT

This header is for created snapshots.

####SNAPSHOT\_DEF(name, type[, oplist])

Define the snapshot 'snapshot\_##name##\_t' and its associated methods as "static inline" functions.
A snapshot is a mechanism to allows a consumer thread and a produce thread, **working at different speeds**, to exchange messages in a reliable and thread safe way.
In practice, it is implemented using a triple buffer.

This container is designed to be used for easy synchronization inter-threads (and the variable shall be a global one).

It shall be done once per type and per compilation unit. No all functions are thread safe.

The object oplist is expected to have the following operators (INIT, INIT\_SET, SET and CLEAR), otherwise default operators are used. If there is no given oplist, the default operators are also used. The created methods will use the operators to init, set and clear the contained object. It can have the optional methods INIT\_MOVE and MOVE.

Example:

	SNAPSHOT_DEF(uint, unsigned int)
	void f(unsigned int * i) {
		snapshot_uint_t t;
		snapshot_uint_init(t);
		i = snapshot_uint_take(t);
		*i = 1;
		i = snapshot_uint_look(t);
		snapshot_uint_clear(t);
	}


####Created methods

The following methods are automatically and properly created by the previous macros. In the following methods, name stands for the name given to the macro which is used to identify the type.

TODO:


###M-SHARED

This header is for creating shared pointer.

####SHARED\_PTR\_DEF(name, type[, oplist])

Define the shared pointer 'shared\_##name##\_t' and its associated methods as "static inline" functions.
A shared pointer is a mechanism to keep tracks of all users of an object
and performs an automatic destruction of the object whenever all users release
their need on this object.

The destruction of the object is thread safe.

The object oplist is expected to have the following operators (CLEAR and DEL), otherwise default operators are used. If there is no given oplist, the default operators are also used. The created methods will use the operators to init, set and clear the contained object.

There are designed to work with buffers with policy BUFFER\_PUSH\_INIT\_POP\_MOVE
to send a shared pointer across multiple threads.

Example:

	SHARED_PTR_DEF(mpz, mpz_t, (CLEAR(mpz_clear)))
	void f(void) {
		shared_mpz_t p;
		mpz_t z;
		mpz_init(z);
		shared_mpz_init2 (p, z);
		buffer_uint_push(g_buff1, p);
		buffer_uint_push(g_buff2, p);
		shared_mpz_clear(p);
	}


####Created methods

The following methods are automatically and properly created by the previous macros. In the following methods, name stands for the name given to the macro which is used to identify the type.

TODO:


###M-I-SHARED

This header is for creating intrusive shared pointer.

####ISHARED\_PTR\_DEF(name, type[, oplist])

Extend an object by adding the necessary interface to handle it as a shared pointer and define the associated methods to handle it as "static inline" functions.
A shared pointer is a mechanism to keep tracks of all users of an object
and performs an automatic destruction of the object whenever all users release
their need on this object.

The destruction of the object is thread safe.

The object oplist is expected to have the following operators (CLEAR and DEL), otherwise default operators are used. If there is no given oplist, the default operators are also used. The created methods will use the operators to init, set and clear the contained object.

There are designed to work with buffers with policy BUFFER\_PUSH\_INIT\_POP\_MOVE
to send a shared pointer across multiple threads.

Example:

        typedef struct mystruct_s {
                ISHARED_PTR_INTERFACE(mystruct, mystruct_s);
                char *message;
        } mystruct_t;
        static inline mystruct_clear(mystruct_t *p) { free(p->message); }
        ISHARED_PTR_DEF(mystruct, mystruct_t, (CLEAR(mystruct_clear M_IPTR)))

        void f(void) {
                mystruct_t *p = malloc(mystruct_t);
                p->message = strdup ("Hello");
                ishared_mystruct_init(p);
                buffer_mystruct_push(g_buff1, p);
                buffer_mystruct_push(g_buff2, p);
                ishared_mystruct_clear(p);
        }


####Created methods

The following methods are automatically and properly created by the previous macros. In the following methods, name stands for the name given to the macro which is used to identify the type.

TODO:


###M-I-LIST

This header is for creating intrusive dual-chained list.

####ILIST\_DEF(name, type[, oplist])

Extend an object by adding the necessary interface to handle it within a dual linked list,
define the dual linked list 
and define the associated methods to handle it as "static inline" functions.
'name' shall be a C identifier which will be used to identify the list. It will be used to create all the types and functions to handle the container.
It shall be done once per type and per compilation unit.

It is expected to only have one list of a kind in the entire program (TBC if really needed).
An intrusive list allows to move from an object to the next object without needing to go through the entire list,
or to remove an object from a list in O(1).
It may, or may not, be better than standard list. It depends on the context.

The object oplist is expected to have the following operators (INIT, INIT\_SET, SET and CLEAR), otherwise default operators are used. If there is no given oplist, the default operators are also used. The created methods will use the operators to init, set and clear the contained object.


Example:

	typedef struct test_s {
	  int n;
	  ILIST_INTERFACE (tname, struct test_s);
	} test_t;

	ILIST_DEF(tname, test_t)

	void f(void) {
		test_t x1, x2, x3;
		ilist_tname_t list;

		x1.n = 1;
		x2.n = 2;
		x3.n = 3;

		ilist_tname_init(list);
		ilist_tname_push_back (list, &x3);
		ilist_tname_push_front (list, &x1);
		ilist_tname_push_after (&x1, &x2);

		int n = 1;
		for M_EACH(item, list, ILIST_OPLIST(tname)) {
			assert (n == item->n);
			n++;
		}
		ilist_tname_clear(list);
	}


####Created methods

The following methods are automatically and properly created by the previous macros. In the following methods, name stands for the name given to the macro which is used to identify the type.

TODO:

###M-BITSET

This header is for using bitset.

A bitset can be seen as a specialized version of an array of bool, where each item takes only 1 bit.
It allows for compact representation of such array.

Example:

	void f(void) {
		bitset_t set;
		bitset_init(set);
		for(int i = 0; i < 100; i ++)
			bitset_push_back(set, i%2);
		bitset_clear(set);
	}


#### methods

The methods are mostly the same than for an array. The following methods are available:

TODO:

###M-STRING

This header is for using string. The size of the string is automatically updated in function of the needs.


Example:

	void f(void) {
		string_t s1;
		string_init (s1);
		string_set_str (s1, "Hello, world!");
		string_clear(s1);
	}

#### methods

The following methods are available:

TODO:

###M-CORE

This header is the internal core of M\*LIB, providing a lot of functionality 
by extending a lot the preprocessing capability.
Working with these macros is not easy and the developper needs to know
how the macro preprocessing works.
It also adds the needed macro for handling the oplist.
As a consequence, it is needed by all other header files.

Example:

	M_MAP(f, 1, 2, 3, 4)
	M_REDUCE(f, g, 1, 2, 3, 4)
	M_SEQ(1, 20, f)
	
#### methods

The following methods are available:

TODO:

###M-MUTEX

This header is for providing very thin layer around OS implementation of threads, conditional variables and mutexs.
It has backends for WIN32, POSIX thread or C11 thread.

It was needed due to the low adoption rate of C11 equivalent layer.

Example:

	M_THREAD_T idx_p;
	M_THREAD_T idx_c;

	M_THREAD_CREATE (idx_p, conso_function, NULL);
	M_THREAD_CREATE (idx_c, prod_function, NULL);
	M_THREAD_JOIN(idx_p;
	M_THREAD_JOIN(idx_c);

#### methods

The following methods are available:

TODO:


###M-ALGO

This header is for generating generic algorithm to containers.
It is a WIP.

####ALGO\_DEF(name, container_oplist)

Define the available algorithms for the container which oplist is container_oplist.
The defined algorithms depend on the availability of the methods of the containers
(For example, if there no CMP operation, there is no MIN operation defined).

Example:

	ARRAY_DEF(int, int)
	ALGO_DEF(algo_array, ARRAY_OPLIST(int))
	void f(void) {
		array_int_t l;
		array_int_init(l);
		for(int i = 0; i < 100; i++)
			array_int_push_back (l, i);
		array_int_push_back (l, 17);
		assert( algo_array_contains(l, 62) == true);
		assert( algo_array_contains(l, -1) == false);
		assert( algo_array_count(l, 17) == 2);
		array_int_clear(l);
	}

####Created methods

The following methods are automatically and properly created by the previous macros. In the following methods, name stands for the name given to the macro which is used to identify the type.

TODO:
