/*
   Copyright (c) 2012 Daniel Lerch Hostalot <dlerch@gmail.com>

   Permission is hereby granted, free of charge, to any person obtaining a 
   copy of this software and associated documentation files (the "Software"), 
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense, 
   and/or sell copies of the Software, and to permit persons to whom the 
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in 
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
   DEALINGS IN THE SOFTWARE.
*/



#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>


#define HASHTABLE_HASH(key, ht_size, hash) \
{ \
   unsigned int __h=0; \
   unsigned char *__p; \
\
   for(__p=(unsigned char*)key; *__p!='\0'; __p++) \
      __h = 31*__h + *__p; \
\
   hash=__h%ht_size; \
}


#define HASHTABLE(name,type,key_len) \
\
typedef struct name##_node_t \
{ \
	char key[key_len]; \
	type value; \
	struct name##_node_t *next_collission; \
} \
name##_node_t; \
\
typedef struct name##_t \
{ \
	size_t table_size; \
	size_t nelm; \
	name##_node_t **ht; \
	type defval; \
} \
name##_t; \
\
name##_t *name##_create(size_t table_size, type defval)\
{ \
	name##_t *s = malloc(sizeof(name##_t)); \
	assert(s!=NULL); \
	s->table_size = table_size; \
	s->nelm = 0; \
	s->ht = calloc(sizeof(name##_node_t), table_size); \
	assert(s->ht !=NULL);\
	s->defval = defval; \
	return s; \
} \
\
void name##_set(name##_t *ht, char *key, type value) \
{ \
	name##_node_t *n = NULL; \
	int found = 0; \
	unsigned int h = 0;\
	HASHTABLE_HASH(key, ht->table_size, h) \
	for(n=ht->ht[h]; n!=NULL; n=n->next_collission) \
	{ \
		if(strcmp(key, n->key)==0) \
		{ \
			n->value = value; \
			found = 1; \
		} \
	} \
	if(!found) \
	{ \
		n = malloc(sizeof(name##_node_t)); \
		assert(n!=NULL); \
		strncpy(n->key, key, sizeof(n->key)); \
		n->key[sizeof(n->key)-1]=0; \
		n->value = value; \
		ht->nelm++; \
		n->next_collission = ht->ht[h]; \
		ht->ht[h] = n; \
	} \
} \
\
type name##_get(name##_t *ht, char *key) \
{ \
	name##_node_t *n = NULL; \
	unsigned int h = 0;\
	HASHTABLE_HASH(key, ht->table_size, h) \
	for(n=ht->ht[h]; n!=NULL; n=n->next_collission) \
	{ \
		if(strcmp(key, n->key)==0) \
			return n->value; \
	} \
	return ht->defval; \
} \
\
void name##_free(name##_t *ht) \
{ int c=0;\
	if(!ht) \
		return; \
	\
	int i; \
	for(i=0; i<ht->table_size; i++) \
	{ \
		name##_node_t *prev = NULL; \
	   name##_node_t *n = NULL; \
		for(n=ht->ht[i]; n!=NULL; n=n->next_collission) \
		{ \
			if(prev) \
			{ \
				free(prev);\
				prev=NULL;\
			} \
			prev = n; \
		} \
		if(prev) \
		{ \
			free(prev);\
			prev=NULL;\
		} \
	} \
	if(ht->ht) \
	{ \
		free(ht->ht); \
	} \
	if(ht) \
	{ \
		free(ht); \
	} \
} 


#endif





