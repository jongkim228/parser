/************************************************************************
University of Leeds
School of Computing
COMP2932- Compiler Design and Construction
Lexer Module

I confirm that the following code has been developed and written by me and it is entirely the result of my own work.
I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program from me.
I confirm that I will not publish the program online or share it with anyone without permission of the module leader.

Student Name: Jonghyun Kim 
Student ID: 201743821
Email: sc23j3k@leeds.ac.uk
Date Work Commenced:2025-02-28
*************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"


// YOU CAN ADD YOUR OWN FUNCTIONS, DECLARATIONS AND VARIABLES HERE

FILE* f;
char currentFile[128];
int LineCount = 1;

#define NumberKeywords 21

const char* keywords[NumberKeywords] = {
  "class","constructor","method","function","int", "boolean","char","void","var","static","field","let","do","if","else","while",
  "return","true","false","null","this"
};

int IsKeyWord(char* str)
{
	for (int i = 0; i < NumberKeywords; i++)
		if (!strcmp(keywords[i], str))
			return 1;
	return 0;
}

int EatWC()
{
    int c;
    c = getc(f); 

    if (c == EOF) {
        return -1; 
    }

    while (c != EOF && isspace(c)) {
        if ((char)c == '\n') {
            LineCount++;
        }
        c = getc(f);
    }

    if (c == '/') { 
        c = getc(f);

        if (c == '/') {
            while (c != EOF && c != '\n') {
                c = getc(f); 
                if (c == '\n') {
                    LineCount++; 
                }
            }
        }
        else if (c == '*') {
            c = getc(f);

            if (c == '*') {
                while (c != EOF) {
                    c = getc(f);
                    if (c == '*' && (c = getc(f)) == '/') {
                        break;
                    }
                    if (c == '\n') {
                        LineCount++; 
                    }
                }
            }

            else {
                while (c != EOF) {
                    c = getc(f); 
                    if (c == '*' && (c = getc(f)) == '/') {
                        break;
                    }
                    if (c == '\n') {
                        LineCount++; 
                    }
                }
            }
        }
        return EatWC();
    }

    return c;
}


const char* TokenTypeToString(TokenType type) {
  switch (type) {
      case RESWORD: return "RESWORD";
      case ID: return "ID";
      case INT: return "INT";
      case SYMBOL: return "SYMBOL";
      case STRING: return "STRING";
      case EOFile: return "EOFile";
      case ERR: return "ERR";
  }
}



// IMPLEMENT THE FOLLOWING functions
//***********************************

// Initialise the lexer to read from source file
// file_name is the name of the source file
// This requires opening the file and making any necessary initialisations of the lexer
// If an error occurs, the function should return 0
// if everything goes well the function should return 1
int InitLexer (char* file_name)
{
  f = fopen(file_name,"r");
  if (f == 0){
    return 0;
  }else{
    strncpy(currentFile, file_name, sizeof(currentFile) - 1);
    currentFile[sizeof(currentFile) - 1] = '\0';
    return 1;
  }


}


// Get the next token from the source file

Token GetNextToken ()
{
	Token t;
  t.tp = ERR;

  int c = EatWC();

  char temp[128];
  int i = 0;

  if(c == -1){
    t.tp = EOFile;
    strcpy(t.lx,"End of File");
    strcpy(t.fl, currentFile);
    t.ln = LineCount;
    return t;
  }

  if(c == '"'){
    c = getc(f);
    while(c != '"'){
      temp[i++] = c;
      c = getc(f);
    }

    temp[i] = '\0';
    strcpy(t.lx,temp);
    t.tp = STRING;
    strcpy(t.fl, currentFile);
    t.ln = LineCount;
    return t;
  }

  if(isalpha(c)){
    while (isalpha(c) || isdigit(c)){
      temp[i++] = c;
      c = getc(f);
    }
    temp[i] = '\0';

    if(IsKeyWord(temp)){
      t.tp = RESWORD;
      strcpy(t.lx,temp);
    }else{
      t.tp = ID;
      strcpy(t.lx,temp);
    }
    strcpy(t.fl, currentFile);
    t.ln = LineCount;
    return t;
  }

  if(isdigit(c)){
    while(isdigit(c)){
      temp[i++] = c;
      c = getc(f);
    } 
    temp[i] = '\0';
    t.ln = atoi(temp);
    strcpy(t.fl, currentFile);
    t.ln = LineCount;
    return t;
  }

  if(!isdigit(c) && !isalpha(c)){
    t.tp = SYMBOL;
    t.lx[0] = c;
    t.lx[1] = '\0';
    strcpy(t.fl, currentFile);
    t.ln = LineCount;
    return t;
  }

  return t;
}


// peek (look) at the next token in the source file without removing it from the stream
Token PeekNextToken ()
{
  Token t;
  t.tp = ERR;

  long pos = ftell(f);
  int savedLine = LineCount;

  t = GetNextToken();

  fseek(f, pos, SEEK_SET);
  LineCount = savedLine;   

  return t;
}

// clean out at end, e.g. close files, free memory, ... etc
int StopLexer ()
{
  if(f){
    fclose(f);
  }
	return 0;
}

// do not remove the next line
#ifndef TEST


// do not remove the next line
#endif

