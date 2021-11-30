/*
 * MLIB - TEST module
 *
 * Copyright (c) 2017-2021, Patrick Pelissier
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * + Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * + Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <stdio.h>

#include "m-wrap.h"
#include "m-list.h"
#include "m-array.h"
#include "m-deque.h"
#include "m-dict.h"
#include "m-rbtree.h"
#include "m-bptree.h"

#include "coverage.h"

// TODO: Add IT_REF PROPERTIES for associative array / set as:
//  0: IT_REF is a pointer to key (set only)
//  1: IT_REF is a pointer to pair (key, value) (map)
//  2: IT_REF is a pair of pointer to key, pointer to value (map)

// TODO: modify m-wrap to use this property instead of LIMITS(3)
// FIXME: m-concurrent ? Need to export property ?

// TODO: Remove LIMITS(3)

// TODO: Factorize common usage of LIMITS as the default kind of wrapper:
//  STACK (array, list, dequeu, prioqueue)
//  QUEUE (dplist, dequeu), 
//  OSET (rbtree, b+tree), 
//  USET (rbtree, b+tree, dict_set, dict_oa_set), 
//  OMAP (dict, dict_oa), 
//  UMAP (b+tree, dict, dict_oa), 
//  CUSTOM (To be provided by user)
// providing both the size minimal of the types and the classic export oplist

#define EXPORT_LIST (INIT(1), INIT_SET(1), SET(1), CLEAR(1), PUSH(1), POP(1) )
WRAP_FULL_DECL(wrap_list, int, EXPORT_LIST )
LIST_DEF(list_int, int)
WRAP_FULL_DEF_AS(wrap_list, wrap_list_t, wrap_list_it_t, int, EXPORT_LIST, LIST_OPLIST(list_int), WRAP_DEFAULT_SUFFIX_OPL() )
//TODO: suffix parameter shall be optional

#define O2 ARRAY_OPLIST(array_int)
WRAP_FULL_DECL(wrap_array, int, O2 )
WRAP_FULL_DEF(wrap_array, ARRAY, int)

#define O3 M_OPEXTEND(DEQUE_OPLIST(deque_int) , LIMITS( (8,4,0) ) )
WRAP_FULL_DECL(wrap_deque, int, O3 )
WRAP_FULL_DEF(wrap_deque, DEQUE, int)
//TODO: LIMITS shall be factorized so that a user can use it without including a header.
//TODO: LIMITS shall also be a parameter independent of the oplist in the macro

WRAP_FULL_DECL(wrap_dict_set, int, M_OPEXTEND(DICT_SET_OPLIST(dict_int), LIMITS(M_WRAP_LIMITS_DICT_SET) ))
WRAP_FULL_DEF(wrap_dict_set, DICT_SET, int)
//TODO: Key shall not be a size_t for dict set but 'int'. How to do it.

#define O5 M_OPEXTEND(DICT_OPLIST(dict_int2) , LIMITS( (6,4,1) ) )
WRAP_FULL_DECL2(wrap_dict, int, int, O5 )
DICT_DEF2(dict_int2, int, int)
WRAP_FULL_DEF2(wrap_dict, int, int, O5 )

// name, subtype_t, subtype_oplist, inline_oplist, wrapped_oplist
WRAP_PARTIAL_DECL(mylist, list_int_t, LIST_OPLIST(list_int), 
                    (INIT(1) , CLEAR(1) ),
                    (INIT_SET(1), SET(1) )      
                )

// name, subtype_t, subtype_oplist, inline_oplist, wrapped_oplist
WRAP_PARTIAL_DEF(mylist, list_int_t, LIST_OPLIST(list_int), 
                    (INIT(1) , CLEAR(1) ),
                    (INIT_SET(1), SET(1) )      
                )

// TODO: Test with incomplete type?

int main(void)
{

    return 0;
}