#pragma once
#include <iostream>
//using namespace std;
#define BUFFER_COUNT_SIZE 10
class Message
{
	/**/
	private:
	int length; // dl�ka char* message
	int lengthFromToStr; // dl�ka %d|%d|
	int from; // toto bude ma� na starosti tie� server 
	int to; // povie ktor�mu klientovi to chce posla�
	//std::string idOfCanal; // id spr�vy ktorej sa to t�kalo bude ma� na starosti server
	char* message; // spr�va ktor� chce odosla�

	public:

		/* message from user, to user, char* string to copy, length of allocated string*/
	Message(int paFrom, int paTo,const char* pTextMatrix,int paLength) {
		this->from = paFrom;
		this->to = paTo;
		
		char fromAndTo[BUFFER_COUNT_SIZE]  = { 0 };
		sprintf_s(fromAndTo, BUFFER_COUNT_SIZE, "%d|%d|", this->from, this->to);
		this->lengthFromToStr = strlen(fromAndTo);
		this->length = paLength + strlen(fromAndTo);
		/*1|12|message+/0   toto bude odosielan� form�t serveru  pri�om kon�� lomenou nulou*/  
		this->message = (char*)malloc(strlen(fromAndTo) + paLength + 1); 
		sprintf_s(this->message, strlen(fromAndTo)+1, "%s", fromAndTo);
		(char*)memcpy((this->message)+ strlen(fromAndTo), pTextMatrix, paLength);
		this->message[strlen(fromAndTo) + paLength] = 0; // ukon�enie stringu pre zistenie ve�kosti cez strlen()
		this->trimMessageEnd(); // nastav� nulu pre skratenie stringu

	}

	void trimMessageEnd() {
		int lastCharId = this->length; // ten navy�e znak sa nastav� na nulu ako je.
		size_t i;
		for (i = this->length -1; i >= 0; i--)
		{
			if (message[i] == ' ') {
				lastCharId = i; // pokia� je medzera ulo�� jeho poz�ciu
			}else {
				message[lastCharId] = 0;//nieje medzera tak posledneho poz�cia sa nastav� na 0. zmen�� sa tak posielany string cez strlen(). strlen naraz� na tuto nulu a ore�e z buffra �o potrebuje
				if (lastCharId == this->lengthFromToStr) {
					this->length = 0; // v�etko je pr�zdne tak je jeho ve�kos� 0
				}
				break;
			}
			
		}
		//this->length = i;
	}

	/*uvolnenie alok�cie z kon�truktora*/
	~Message() {
		this->message = nullptr;
		free(this->message);
	}

	int getFrom() {
		return this->from;
	}
	int getLength() {
		return this->length;
	}

	int getTo() {
		return this->to;
	}

	char* getMessage() {
		return this->message;
	}

	void setMessage(char* paMessage) {
		this->message = paMessage;
	}

	
};

