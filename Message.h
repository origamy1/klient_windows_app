#pragma once
#include <iostream>
//using namespace std;
#define BUFFER_COUNT_SIZE 10
class Message
{
	/**/
	private:
	int length; // dlka char* message
	int lengthFromToStr; // dlka %d|%d|
	int from; // toto bude ma na starosti tie server 
	int to; // povie ktorému klientovi to chce posla
	//std::string idOfCanal; // id správy ktorej sa to tıkalo bude ma na starosti server
	char* message; // správa ktorú chce odosla

	public:

		/* message from user, to user, char* string to copy, length of allocated string*/
	Message(int paFrom, int paTo,const char* pTextMatrix,int paLength) {
		this->from = paFrom;
		this->to = paTo;
		
		char fromAndTo[BUFFER_COUNT_SIZE]  = { 0 };
		sprintf_s(fromAndTo, BUFFER_COUNT_SIZE, "%d|%d|", this->from, this->to);
		this->lengthFromToStr = strlen(fromAndTo);
		this->length = paLength + strlen(fromAndTo);
		/*1|12|message+/0   toto bude odosielanı formát serveru  prièom konèí lomenou nulou*/  
		this->message = (char*)malloc(strlen(fromAndTo) + paLength + 1); 
		sprintf_s(this->message, strlen(fromAndTo)+1, "%s", fromAndTo);
		(char*)memcpy((this->message)+ strlen(fromAndTo), pTextMatrix, paLength);
		this->message[strlen(fromAndTo) + paLength] = 0; // ukonèenie stringu pre zistenie ve¾kosti cez strlen()
		this->trimMessageEnd(); // nastaví nulu pre skratenie stringu

	}

	void trimMessageEnd() {
		int lastCharId = this->length; // ten navyše znak sa nastaví na nulu ako je.
		size_t i;
		for (i = this->length -1; i >= 0; i--)
		{
			if (message[i] == ' ') {
				lastCharId = i; // pokia¾ je medzera uloí jeho pozíciu
			}else {
				message[lastCharId] = 0;//nieje medzera tak posledneho pozícia sa nastaví na 0. zmenší sa tak posielany string cez strlen(). strlen narazí na tuto nulu a oree z buffra èo potrebuje
				if (lastCharId == this->lengthFromToStr) {
					this->length = 0; // všetko je prázdne tak je jeho ve¾kos 0
				}
				break;
			}
			
		}
		//this->length = i;
	}

	/*uvolnenie alokácie z konštruktora*/
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

