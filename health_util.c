#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "health.h"

/*
* health_util.c - Helper functions for the Health Monitoring System
*
* Add any optional helper function declarations here, define 
* the functions after the required functions below.
*
* static void myOptionalFunc();   // EXAMPLE 
*
*/
void printPatientData(int ID);
void checkout(int patientID);

/*
* addPatient: check-in a new patient
*   (1) allocate a new Chart for the patient
*   (2) initialize the chart with the passed patientID
*   (3) new patients are inserted at the start of the patient list
*
* (note that the variable patientList is globally accessible)
*/
void addPatient( int patientID ){
  
	Chartptr newChart = malloc(sizeof(Chart));
	newChart -> id = patientID;
	newChart -> buffer = NULL;
	newChart -> next = NULL;
	
	if(patientList == NULL){
		patientList = newChart;
		newChart -> next = patientList;
	} else {
		Chartptr temp = patientList -> next;
		patientList -> next = newChart;
		newChart -> next = temp;
	}
}

/*
* addHealthType: add a new health type buffer for a given patient
*	(1) allocate a new CircularBuffer
*	(2) initialize the buffer
* 	(3) link it to the existing patient's Chart
*/
void addHealthType( int patientID, int newType ){
  
	CBuffptr newBuff = malloc(sizeof(CircularBuffer));
	newBuff -> type = newType;
	newBuff -> start = 0;
	newBuff -> end = 0;
	
	Chartptr patient = getChart(patientID);
	CBuffptr curs = patient -> buffer;
	
	if (curs == NULL) {	
		patient -> buffer = newBuff;
		newBuff -> next = newBuff; 
	} else {
		while(curs -> next != NULL && curs -> next != patient -> buffer) {
			curs = curs -> next;
		}
		curs -> next = newBuff;
		newBuff -> next = patient -> buffer;
	}
}
  
/*
*  getChart: given a patientID, return a pointer to their Chart
*/
Chartptr getChart( int patientID ){
	Chartptr foundChart = patientList;

	while(foundChart != NULL){
		if(foundChart -> id == patientID){ 
			break;
		} else if ( foundChart -> next == patientList) {
			return NULL;
		}
		foundChart = foundChart -> next;
	}
  
	return foundChart;
}

/* 
*  getHealthType: given a patientID & healthType, return a pointer 
*  to the CircularBuffer for that type. If the health type does NOT exist 
*  for that patient, return NULL
*/
CBuffptr getHealthType( int patientID, int healthType ){
	CBuffptr foundType = NULL;
	Chartptr patient = getChart(patientID);

	if (patient -> buffer != NULL){
		CBuffptr curs = patient -> buffer;   
		do {
			if(curs -> type == healthType){
				foundType = curs; 
				break;
			}
			curs = curs -> next;
		}while(curs != patient -> buffer);
	}
	return foundType;
}
 
/*
*  addHealthReading: given a pointer to CircularBuffer, add the passed
*  timestamp and health data type reading to the buffer
*/
void addHealthReading( CBuffptr buffer, char* timestamp, int reading ){
	
 	Element *entry = &(buffer -> reading[buffer -> end]);
	strcpy(entry -> timestamp, timestamp);
	entry -> value = reading; 
	buffer -> end++;

}
  
/*
*  removePatient: check-out an existing patient
*	  (1) delete the patient's Chart & accompanying 
*         health data readings.
*     	  (2) update the list of current patients
*/
void removePatient(int patientID){
	Chartptr patient = getChart(patientID);
 
	if(patient != NULL && patient -> buffer != NULL) {
		CBuffptr curs = patient -> buffer -> next; 

		while(curs != patient -> buffer){

			CBuffptr temp = curs -> next;
			free(curs);
			curs = temp;
		} 
		free(patient -> buffer);
		patient -> buffer = NULL;
	}
}

 
/*
* Optional helper functions defined starting here:
*
* static void myOptionalFunc(){ }  // EXAMPLE
*  
*/

void checkout( int patientID ){
	
	removePatient(patientID);

	Chartptr patient = getChart(patientID);

	if (patient != NULL){
		if ( patient -> next -> id == patient -> id) { 
			patientList = NULL;
			free(patient);
		} else {
			while (patientList -> next != patient){ 
				patientList = patientList -> next;
			} 
			patientList -> next = patient -> next;
			free(patient);
		}
	}
}

void printPatientData(int ID){
	printf("Readings for Patient ID = %d are:\n", ID);
	CBuffptr type;
	int i, k;
	char *titles[] = {"Temperature", "Heart Rate", "Systolic Pressure", "Diastolic Pressure", "Respiration Rate"};
	
	for (k = 1; k <= 5; k++){
		printf("%s:\n", titles[k - 1]);
		type = getHealthType(ID, k);
		if (type == NULL || type -> start == type -> end){
			puts("<none>");	
		} else {
			if ( k == 1 ){ // Temperature requires a double to be printed
				for (i = type -> start; i < type -> end; i++){
					printf("%s: %.1f\n", type -> reading[i].timestamp, type -> reading[i].value / 10.0);
				}	
			} else {
				for (i = type -> start; i < type -> end; i++){
					printf("%s: %d\n", type -> reading[i].timestamp, type -> reading[i].value);
				}

			}
		}

	}
}
