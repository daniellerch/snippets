/*****************************************************************************
 *  This file is part of the OpenDomo project.
 *  Copyright(C) 2011 OpenDomo Services SL
 *  
 *  Daniel Lerch Hostalot <dlerch@opendomo.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

//HASHTABLE(htint, int, 16);
//HASHTABLE(htfloat, float, 16);
HASHTABLE(htstr, char*, 16);


int main()
{
	/*
	htint_t *hi = htint_create(10, 0);

	char key[10];
	strcpy(key, "bla");

	htint_set(hi, key, 5);
	htint_set(hi, "ble", 6);
	//htint_set(hi, "ble", 7);
	htint_set(hi, "bli", 7);

	printf("nelm: %d\n", hi->nelm);
	printf("%d\n", htint_get(hi, "bla"));
	printf("%d\n", htint_get(hi, "ble"));
	printf("%d\n", htint_get(hi, "bli"));
	printf("%d\n", htint_get(hi, "ooops"));

	htint_node_t *ni = NULL; 
   for(ni=hi->head; ni; ni=ni->next) 
		printf("->%d\n", ni->value);
	

	htint_free(hi);
	*/

	htstr_t *hs = htstr_create(10000000, "");

	htstr_set(hs, "bla", strdup("ai"));
	htstr_set(hs, "ble", strdup("ei"));
	htstr_set(hs, "bli", strdup("oi"));

	free(htstr_get(hs, "bla"));
	htstr_set(hs, "bla", strdup("uu"));

	printf("%s\n", htstr_get(hs, "bla"));
	printf("%s\n", htstr_get(hs, "ble"));
	printf("%s\n", htstr_get(hs, "bli"));
	printf("%s\n", htstr_get(hs, "ooops"));

	puts("allocating ...");
	int i;
	for(i=0; i<10000000; i++)
	{
		char buffer[32];
		snprintf(buffer, sizeof(buffer), "%d xxxxxxxxxxxxxxxxxxxxxxx", i);
		htstr_set(hs, buffer, strdup(buffer));
	}

	/*
	htstr_node_t *ns = NULL;
	int cnt=0;
	puts("freing");
   for(ns=hs->head; ns; ns=ns->next) 
	{
		cnt++;
		//printf("free: %s\n", ns->value);
		free(ns->value);
		ns->value = NULL;
	}
	printf("free %d\n", cnt);
	*/

   //for(ns=hs->head; ns; ns=ns->next) 
	//	printf("->%s\n", ns->value);

	printf("free ...\n");
	htstr_free(hs);
	
	printf("wait\n");
	for(;;);

   return 0;
}




