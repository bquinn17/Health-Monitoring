#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "health.h"

#define MAXLINE 20 //most possible characters in a line

/*
* Main function for the Health Monitoring System. Primarily responsible for
* processing input of csv lines and printing as required. Data structures are
* maintained using the helper functions in health_util.c
* 
*/

Chartptr patientList = NULL;    /* Define global variable patientList (declared in health.h) */
								/* patientList is globaaly accessible in health_util.c    */

void main(){

	printf("Welcome to the Health Monitoring System\n\n");

/*
*  YOUR CODE GOES HERE:
*  (1) Read an input csv line from stdin 
*  (2) Parse csv line into appropriate fields
*  (3) Take action based on input type:
*  		 - Check-in or check-out a patient with a given ID
*        - Add a new health data type for a given patient
*        - Store health data in patient record or print if requested
*  (4) Continue (1)-(3) until EOF
*/
	char line[MAXLINE+1];
	scanf("%[^\n]", line);
	char splitby[] = ", ";
	while (!feof(stdin)) {
		char *token = strtok(line, splitby);
		int pID = strtol(token, NULL, 10);
		
		token = strtok(NULL, splitby);
		char timeStamp[MAXTIME + 1];
		strcpy(timeStamp, token);

		token = strtok(NULL, splitby);
		int typeID = strtol(token, NULL, 10);

		token = strtok(NULL, splitby);
		int val = strtol(token, NULL, 10);

		if(typeID == 7){
			addPatient(pID);
			puts("--------------------------------------------------");
			printf("%s: Patient ID = %d checking in\n", timeStamp, pID);
			puts("--------------------------------------------------");

		} else if (typeID == 9){
			addHealthType(pID, val);	
		} else if (typeID >= 1 && typeID <= 5) {
			CBuffptr buffer = getHealthType(pID, typeID);
			addHealthReading(buffer, timeStamp, val);
		} else if (typeID == 6) {
			puts("\n--------------------------------------------------");	
			printPatientData(pID);
			puts("--------------------------------------------------");
		} else if (typeID == 8) {
			checkout(pID);
			puts("--------------------------------------------------");
			printf("%s: Patient ID = %d checking out\n", timeStamp, pID);
			puts("--------------------------------------------------");

		} else if (typeID == 0) {
			removePatient(pID);
			puts("--------------------------------------------------");
			printf("%s: Patient ID = %d reset data\n", timeStamp, pID);
			puts("--------------------------------------------------");
		}


		scanf(" %[^\n]", line);
	}

	printf("\nEnd of Input\n");

}
