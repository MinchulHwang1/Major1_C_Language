/*
* FILE : m1.cpp
* PROJECT : Major 1
* PROGRAMMER : Minchul Hwang
* FIRST VERSION : 2023-01-31
* Description : This project originates from focused 1 & 2.
				From focused 1, the function has a condition that can be changed.
				From focused 2, the function to get fare has been added.
				The user's input can be received indefinitely, and if a . is entered at any position,
				the input is terminated and the data received up to the previous time is output. (ascending list fare and destination)
				and the program get destination and date.
				if user input correct destination and date, the indication of fare input has come.
				if user input fare, destination list is changed as input fare which is included with destination and date.
				and then the fare list will be changed as ascending list, ans print the list.
				Dynamic allocation is included in the information received and the information stored in the structure.
* The functions in this file are used to struct - getFloat(), insertFare(), insertDest(), struct - fineFlight(), struct - attachNode(), deleteNode(), fillFlightInfo(), printFlightInfo(), struct - deleteAllNode(), and clearNewLine()
*/
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

#define kDestinationString 30
#define kDateSting 30
#define kContinue 1
#define kTable 105
#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS

typedef struct FlightInfo {
	char destination[kDestinationString];
	char date[kDateSting];
	float fare;
	struct FlightInfo* next;
	struct FlightInfo* prev;
} FlightNode;

void printFlightInfo(FlightNode* arrFlight);
FlightNode* deleteAllNode(FlightNode* head);
void clearLine(char* string);
int getFloat(float* pNumber);
void insertFare(char* des, char* dat, float fare, FlightNode** head, FlightNode** tail);
void insertDest(char* des, char* dat, float fare, FlightNode** head, FlightNode** tail);
FlightNode* findFlight(FlightNode*head, char *destination, char *date);
FlightNode* attachNode(FlightNode* head, FlightNode* attach);
void deleteNode(FlightNode* node, FlightNode** head, FlightNode** tail);

int main() {
	char des[kDestinationString] = {};
	char dat[kDateSting] = {};
	float getFare = 0.00;
	float getNewFare = 0.00;
	FlightNode* fareHead = NULL;
	FlightNode* fareTail = NULL;
	FlightNode* destiHead = NULL;
	FlightNode* destiTail = NULL;
	FlightNode* positionCurNode = NULL;			//node for saving current node
	
	while (kContinue) {
		printf("Please Input destination : ");
		fgets(des, sizeof(des), stdin);
		clearLine(des);

		if (strcmp(des, ".") == NULL) {
			break;
		}

		printf("Please Input Flight date : ");
		fgets(dat, sizeof(dat), stdin);
		clearLine(dat);

		if (strcmp(dat, ".") == NULL) {
			break;
		}

		printf("Please Input Flight fare : ");
		getFloat(&getFare);
		
		//send information of list to save in list
		insertFare(des, dat, getFare, &fareHead, &fareTail);
		insertDest(des, dat, getFare, &destiHead, &destiTail);
	}

	printf("\n");
	printf("List - Sorted by fare.\n");
	printFlightInfo(fareHead);

	printf("\n");
	printf("List - Sorted by destination.\n");
	printFlightInfo(destiHead);

	printf("\n");
	printf("Please Input destination : ");
	fgets(des, sizeof(des), stdin);
	clearLine(des);


	printf("Please Input Flight date : ");
	fgets(dat, sizeof(dat), stdin);
	clearLine(dat);
	
	//save the node which user want to find
	positionCurNode = findFlight(destiHead, des, dat);
	
	float beforeFare = 0.00;

	if (positionCurNode != NULL) {
		printf("Please Input Flight fare : ");
		getFloat(&getNewFare);
		beforeFare = positionCurNode->fare;
		positionCurNode->fare = getNewFare;

		printf("\n");
		printf("Before Fare : %0.2f\n", beforeFare);
		printf("\n");
		printf("After Fare : %0.2f\n", getNewFare);

		//attach current node to left destination 
		destiHead = attachNode(destiHead, positionCurNode);

		deleteNode(positionCurNode, &fareHead, &fareTail);

		printf("\n");
		printf("List - New Sorted by fare.\n");
		printFlightInfo(fareHead);
		printf("\n");
		printf("List - New Sorted by destination.\n");
		printFlightInfo(destiHead);
	}
	else {
		printf("\n");
		printf("The list has not been changed.\n");
	}

	//delete all list
	fareHead = deleteAllNode(fareHead);
	destiHead = deleteAllNode(destiHead);
	
	return 0;
}


/*
* Function : deleteNode()
* Description : This function is a function that finds the node with the destination and date when 
				the user inputs the destination and date that the user wants to change the fare for.
				When the corresponding node is found, it is sent to the insertFare function to modify the list.
* Parameters : FlightNode *node - the current node which has the information of destination and date.
*			   FlightNode **head - a pointer struct having pointer of head of node.
*			   FlightNode **tail- a pointer struct having pointer of tail of node.
* Returns : NULL
*/
void deleteNode(FlightNode* node, FlightNode** head, FlightNode** tail) {
	if (node == NULL) {
		return;
	}
	
	FlightNode* currentNode = *head;
	FlightNode* ptr = NULL;

	char des[kDestinationString] = {};
	char dat[kDateSting] = {};
	float fare = 0.0;

	strncpy(des, node->destination, sizeof(node->destination));
	strncpy(dat, node->date, sizeof(node->date));
	fare = node->fare;
	
	while (currentNode != NULL) {
		if (strcmp(currentNode->destination, node->destination) == 0) {
			if (strcmp(currentNode->date, node->date) == 0) {
				//if current node is in a haed node.
				if (currentNode == *head) {
					ptr = *head;
					(*head) = (*head)->next;

					free(ptr);
					insertFare(des, dat, fare, head, tail);
					return;
				}
				//if current node is in a tail node.
				else if (currentNode->next == NULL) {
					ptr = *tail;
					*tail = (*tail)->prev;
					(*tail)->next = NULL;
					free(ptr);
					insertFare(des, dat, fare, head, tail);
					return;
				}
				else {
					FlightNode* preBlock = currentNode->prev;
					FlightNode* aftBlock = currentNode->next;
					aftBlock->prev = preBlock;
					preBlock->next = aftBlock;
					free(currentNode);
					insertFare(des, dat, fare, head, tail);
					return;
				}
			}
		}
		currentNode = currentNode->next;
	}
}


/*
* Function : struct - attachNode();
* Description : When this function finds the node of the destination and date entered by the user, 
				it is a function that attaches the following lists.
* Parameters : FlightNode *head - a head node which has the list.
*			   FlightNode *attach - a node to attach to list..
* Returns : head
*/
FlightNode* attachNode(FlightNode* head, FlightNode* attach) {
	FlightNode* newHead = head;
	FlightNode* ptr = attach;
	while (newHead->next != NULL) {
		if (strcmp(newHead->destination, attach->destination) == NULL) {
			if (strcmp(newHead->date, attach->date) == NULL) {		
				newHead = ptr;
			}
		}
		newHead = newHead->next;
	}
	return head;
}


/*
* Function : struct - findFlight();
* Description : This function serves to find the node of the destination and date entered by the user.
* Parameters : FlightNode *head - a head node which has the list.
*			   char* destination - a string which called destination which user want to change the fare.
*			   char* date - a string which called datewhich user want to change the fare.
* Returns : currentNode - returns the node value when a node with both the destination and date entered by the user is found
*			NULL - returns the NULL when a node with both the destination and date entered by the user is not found
*/
FlightNode* findFlight(FlightNode* head, char* destination, char* date) {
	FlightNode* currentNode = head;
	while (currentNode != NULL) {
		if (strcmp(currentNode->destination, destination) == 0) {
			if (strcmp(currentNode->date, date) == 0) {
				return currentNode;
			}
		}
		currentNode = currentNode->next;
	}

	if (currentNode == NULL) {
		return NULL;
	}
}


/*
* Function : insertFare()
* Description : Each time the user enters a new destination, date, and fare, the list is created in ascending order of fare.
* Parameters : char* des - a destination string which is input by user
			   char* dat - a date string which is input by user
			   float fare - a float number fare which is input by user
			   FlightNode** head - A pointer to the corresponding head node to which the input value is to be put.
			   FlightNode** tail - A pointer to the corresponding tail node to which the input value is to be put.
* Returns : None
*/
void insertFare(char* des, char* dat, float fare, FlightNode** head, FlightNode** tail) {
	FlightNode* newBlock = NULL;
	FlightNode* prevBlock = NULL;
	FlightNode* laterBlock = NULL;

	newBlock = (FlightNode*)malloc(sizeof(FlightNode));
	if (newBlock == NULL){
		printf("Error! No more memory!\n");
		return ;
	}	

	strncpy(newBlock->destination, des, strlen(des)+1);
	strncpy(newBlock->date, dat, strlen(dat)+1);
	newBlock->fare = fare;
	newBlock->prev = newBlock->next = NULL;

	//if head node is empty.
	if (*head == NULL){
		*head = *tail = newBlock;
		return ;
	}
	else if ((*head)->fare > fare) {
		newBlock->next = *head;
		(*head)->prev = newBlock;
		*head = newBlock;
	}
	else{
		prevBlock = *head;		
		laterBlock = (*head)->next;	

		while (laterBlock != NULL){
			if (laterBlock->fare > fare){
				break;
			}
			prevBlock = laterBlock;
			laterBlock = laterBlock->next;
		}	

		newBlock->prev = prevBlock;
		newBlock->next = laterBlock;
		prevBlock->next = newBlock;
	
		if (laterBlock == NULL){
			*tail = newBlock;
		}
		else{
			laterBlock->prev = newBlock;
		}
	}
}



/*
* Function : insertDest()
* Description : Each time the user enters a new destination, date, and fare, the list is created in ascending order of destination.
* Parameters : char* des - a destination string which is input by user
			   char* dat - a date string which is input by user
			   float fare - a float number fare which is input by user
			   FlightNode** head - A pointer to the corresponding head node to which the input value is to be put.
			   FlightNode** tail - A pointer to the corresponding tail node to which the input value is to be put.
* Returns : None
*/
void insertDest(char* des, char* dat, float fare, FlightNode** head, FlightNode** tail) {
	FlightNode* newBlock = NULL;
	FlightNode* prevBlock = NULL;
	FlightNode* laterBlock = NULL;

	newBlock = (FlightNode*)malloc(sizeof(FlightNode));
	if (newBlock == NULL){
		printf("Error! No more memory!\n");
		return ;
	}	

	strncpy(newBlock->destination, des, strlen(des)+1);
	strncpy(newBlock->date, dat, strlen(dat)+1);
	newBlock->fare = fare;
	newBlock->prev = newBlock->next = NULL;

	if (*head == NULL){
		*head = *tail = newBlock;
		return ;
	}
	else if (strcmp((* head)->destination, des) > 0) {
		newBlock->next = *head;
		(*head)->prev = newBlock;
		*head = newBlock;
	}
	else{
		prevBlock = *head;		
		laterBlock = (*head)->next;	 

		while (laterBlock != NULL){
			if (strcmp(laterBlock->destination, des) > 0){
				break;
			}
			prevBlock = laterBlock;
			laterBlock = laterBlock->next;
		}	

		newBlock->prev = prevBlock;
		newBlock->next = laterBlock;
		prevBlock->next = newBlock;

		if (laterBlock == NULL)	{
			*tail = newBlock;
		}
		else{
			laterBlock->prev = newBlock;
		}
	}	
}


/*
* Function : printFlightInfo()
* Description : This function outputs the data stored in the structure.
* Parameters : FlightNode *arrFlight : A structure that stores data values ​​entered by the user.
* Returns : None
*/
void printFlightInfo(FlightNode* arrFlight) {
	printf("%-35s%-35s%-35s\n", "Destination", "Date", "Fare");
	for (int i = 0; i < kTable; i++)
		printf("%c", '-');

	printf("\n");
	while (arrFlight != NULL) {
		printf("%-35s", arrFlight->destination);
		printf("%-35s", arrFlight->date);
		printf("$%-35.2f\n", arrFlight->fare);
		arrFlight = arrFlight->next;
	}
}


/*
* Function : clearNewLine()
* Description : If the received data is blank or contains \n, it is removed.
*				In focused1, I cut \n right away from the accepted data, but in this assignment, it was impossible, so I got it from practice.
* Parameters : char* str - the pointer has string from user
* Returns : None
*/
void clearLine(char* string) {
	char* searchNewLine = strchr(string, '\n');
	if (searchNewLine != NULL){
		*searchNewLine = '\0';
	}
}


/*
* Function : struct - deleteAllNode()
* Description : This function deletes all created node structures.
* Parameters : node* head - Address of the first node in the created node structure
* Returns : NULL
*/
FlightNode* deleteAllNode(FlightNode* head) {
	FlightNode* curNode = NULL;
	FlightNode* ptr = head;

	while (ptr != NULL) {
		curNode = ptr;
		ptr = ptr->next;
		free(curNode);
	}
	return NULL;
}


/*
* Function : getFloat
* Description : This function get float number from user
* Parameters : flost *pNumber - a number try to get number from user.
* Returns : number - a number which is gotten from user.
*/
int getFloat(float* pNumber){
	char record[121] = { 0 }; 
	int number = 0;
	
	fgets(record, 121, stdin);

	if (sscanf(record, "%f", pNumber) != 1){
		number = 0;
	}

	return number;
}
