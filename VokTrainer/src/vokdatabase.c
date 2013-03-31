/*
 * vokdatabase.c
 *
 *  Created on: 21.03.2013
 *      Author: marcel
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vokdatabase.h"
//The working directory
DIR *voks;
voktrainerRuntime *runtime;
char *getRealUnitName(char filename[]) {
	return filename;
}
int initDatabase(char* pathtoworkspace) {
	unit *bufferedUnits;
	unit *pointer;
	voks = opendir(pathtoworkspace);
	if (voks == NULL ) {
		return -1;
	}
	struct dirent *file_entry;
	int unitCounter = 0;
	bufferedUnits = malloc(sizeof(unit));
	bufferedUnits->name = malloc(sizeof(unitname));
	pointer = bufferedUnits;
	while ((file_entry = readdir(voks))) {
		char* name = file_entry->d_name;
		if (strcmp(name, ".") != 0 && strcmp(name, "..") != 0) {
			//get the file to open it
			char filename[strlen(pathtoworkspace) + strlen(name)];
			//copy pathtoworkspace to the filename varaible
			strcpy(filename, pathtoworkspace);
			//add the name
			strcat(filename, name);
			//open the file
			FILE *bufferfile = fopen(filename, "rw");
			//Buffer the file
			char linebuffer[30];
			if (fgets(linebuffer, 30, bufferfile) != NULL
					&& (strcmp(linebuffer, "#voktrainer headline\n") == 0
							|| strcmp(linebuffer, "#voktrainer headline \n")
									== 0)) {
				//count the unit
				unitCounter++;
				//set the pointer to the next unit
				pointer->next = malloc(sizeof(unit));
				//write this one to this local tmp variable
				pointer = pointer->next;
				//Add the name
				pointer->name = malloc(sizeof(unitname));
				pointer->name->name = getRealUnitName(name);
				//start buffering variables
				//setup the variables
				pointer->vocs = malloc(sizeof(vocable));
				vocable *voc_pointer = pointer->vocs;
				while (fgets(linebuffer, 30, bufferfile) != NULL ) {
					if (linebuffer[0] != '#') {
						if (voc_pointer == NULL ) {
							voc_pointer->next = malloc(sizeof(vocable));
						}
						//remove \n form the end of this line
						size_t ln = strlen(linebuffer) - 1;
						if (linebuffer[ln] == '\n')
							linebuffer[ln] = '\0';
						//printf("Read \"%s\" \n", linebuffer);
						strcpy(voc_pointer->wordA, strtok(linebuffer, "."));
						strcpy(voc_pointer->wordB, strtok(linebuffer, "."));
						//printf("Found Vocables %s and %s \n",
						//		voc_pointer->wordB, voc_pointer->wordA);
						//setup the next vocable in the queue
						voc_pointer = voc_pointer->next;
					}
				}
			} else {
				printf("No headline found ignoring File \"%s\" %s ! \n",
						filename, linebuffer);
			}
		}
	}
	runtime = malloc(sizeof(voktrainerRuntime));
	runtime->bufferedUnits = *bufferedUnits;
	runtime->unitCounter = unitCounter;
	return 1;
}
unit* getSelectedUnit() {
	return runtime->selected;
}
void setSelectedUnit(unit *unit) {
	runtime->asked = unit->vocs;
	runtime->selected = unit;
}
unitsArray *getUnits() {
	int max = runtime->unitCounter;
	int count = 0;
	unitsArray *result = malloc(sizeof(unitsArray) + (max - 1));
	result->size = max;
	//unitname result[max];
	unit *current = &runtime->bufferedUnits;
	for (count = 0; count < max; count++) {
		result->names[count] = *current->next->name;
		current = current->next;
	}
	return result;
}
unit* getUnitForName(unitname *name) {
	unit *bufferedunits = &runtime->bufferedUnits;
	int res = 1;
	while (res != 0) {
		bufferedunits = bufferedunits->next;
		if (bufferedunits == NULL ) {
			return NULL ;
		}
		res = strcmp(name->name, bufferedunits->name->name);
	}
	return bufferedunits;
}

vocable* nextVocable() {
	vocable *result = runtime->asked;
	if (result == NULL ) {
		return NULL ;
	}
	runtime->asked = runtime->asked->next;
	return result;
}
