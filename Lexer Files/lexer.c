/************************************************************************
University of Leeds
School of Computing
COMP2932- Compiler Design and Construction
Lexer Module

I confirm that the following code has been developed and written by me and it is entirely the result of my own work.
I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program from me.
I confirm that I will not publish the program online or share it with anyone without permission of the module leader.

Student Name: Tobias Samuel Flanagan
Student ID: 201510161
Email: sc21tf@leeds.ac.uk
Date Work Commenced: 22/02/20j23
*************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"


// YOU CAN ADD YOUR OWN FUNCTIONS, DECLARATIONS AND VARIABLES HERE

#define eof -1


//global array for all key words
const char* keywords[21] = {"class", "constructor", "method", "function"
  "int", "boolean", "char", "void", "var", "static", "field", "let", "do"
  "if", "else", "while", "return", "true", "false", "null", "this"     };


//global file pointer
FILE* CurrentFile;


// IMPLEMENT THE FOLLOWING functions
//***********************************

// Initialise the lexer to read from source file
// file_name is the name of the source file
// This requires opening the file and making any necessary initialisations of the lexer
// If an error occurs, the function should return 0
// if everything goes well the function should return 1
int InitLexer (char* file_name)
{

  //ninimal error checking currently
  CurrentFile = fopen(file_name, "r");

  //check if file exists
  if (CurrentFile == NULL){

    printf("ERROR: Can not find file: '%s'\n", file_name);
    return 0;

  }

  //TODO: check if file is a JACK file


  return 1;
}


// Get the next token from the source file
Token GetNextToken ()
{

  //initialise token
	Token token;
  token.tp = ERR;

  //c must be int to capture EOF correctly
  int c = getc(CurrentFile);

  //skip through whitespace
  while(isspace(c)){

    c = getc(CurrentFile);

  }


  //check for comments
  if(c == '/'){

    //printf("read first slash\n");
    c = getc(CurrentFile);
    if(c == '/'){

      //skip all writing until end of line or EOF
      printf("line is a comment\n");

      while(c != '\n' || c != eof){
        c = getc(CurrentFile);
      }

    }
    else if(c == '*'){


      //skip all writing until closing brace or eof
      printf("line is a comment until closing\n");

      while(c != eof){
        if(c == '*'){
          c = getc(CurrentFile);
          if(c == '/'){
            //end of comment
            break;
          }
        }
      }

    }
    else{

      printf("line is not a comment\n");
      ungetc(c, CurrentFile);

    }
  }







  return token;
}

// peek (look) at the next token in the source file without removing it from the stream
Token PeekNextToken ()
{
  Token t;
  t.tp = ERR;
  return t;
}

// clean out at end, e.g. close files, free memory, ... etc
int StopLexer ()
{
	return 0;
}

// do not remove the next line
#ifndef TEST
int main ()
{
	// implement your main function here
  // NOTE: the autograder will not use your main function

  InitLexer("Ball.jack");

  Token t = GetNextToken();
  while (t.tp != eof){

    t = GetNextToken();

  }

  
	return 0;
}
// do not remove the next line
#endif
