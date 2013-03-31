/*
 * vokdatabase.h
 *
 *  Created on: 21.03.2013
 *      Author: marcel
 */

#ifndef VOKDATABASE_H_
#define VOKDATABASE_H_
#include <dirent.h>
#define MAX_VOC_LENGHT 100
/*
 * The Struct to save a Vocable
 */
typedef struct vocable {
	char wordA[MAX_VOC_LENGHT];
	char wordB[MAX_VOC_LENGHT];
	struct vocable *next;
} vocable;

/*
 * Define a special struct only for the Name .... TODO more attributes
 */
typedef struct unitname {
	char *name;
} unitname;

/*
 * The Strict to save a unit
 */
typedef struct unit {
	struct unitname *name;
	struct vocable *vocs;
	struct unit *next;
} unit;

/**
 * A Struct that you can return Arrays
 */
typedef struct unitsArray {
	int size;
	struct unitname names[1];
} unitsArray;

/*
 * The struct for multithreading support that more than on Programm can run at the same Time
 */
typedef struct voktrainerRuntime {
	struct unit bufferedUnits;
	int unitCounter;
	struct unit *selected;
	struct vocable *asked;
} voktrainerRuntime;

//the two global variables
extern DIR *vocs;
extern voktrainerRuntime *runtime;

/*
 * Make the Prototypes of the Funktions for the Database
 */
/*
 * This will read all the found units/vocabels from the workingdirectory.
 */
int initDatabase(char*);
/*
 * Will return an structure which makes an array with from the type unitnames available
 */
unitsArray *getUnits();
/*
 * Will return the pointer on an Unit Element which is selected at the Moment
 */
unit* getSelectedUnit();
/*
 * Will set an Unit as selected. This means if the method nextVocable is called the vocables from this unit will be read.
 */
void setSelectedUnit(unit*);
/*
 *Will return an unit Structure which belongs to this name, or NULL if no Unit is found with this name.
 */
unit* getUnitForName(unitname*);
/*
 *Will return an vocable Structure from the Selected Unit
 */
vocable* nextVocable();

#endif /* VOKDATABASE_H_ */
