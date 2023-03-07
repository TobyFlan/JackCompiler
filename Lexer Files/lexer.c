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

int end_reached = 0;

int line_number = 0;


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
  int c = fgetc(CurrentFile);

  //skip through whitespace
  while(isspace(c)){


    if(c == '\n'){
      line_number++;
    }

    c = fgetc(CurrentFile);

  }


  //check for // comments
  if(c == '/'){

    c = fgetc(CurrentFile);
    if(c == '/'){

      //skip all writing until end of line or EOF

      while((c = fgetc(CurrentFile)) != '\n' && c != eof){
        continue;
      }

      line_number++;
      //return next token recursively
      return GetNextToken();

    }
    else{

      ungetc(c, CurrentFile);

    }

    //check for /* comments
    if(c == '*'){

      while((c = fgetc(CurrentFile)) != eof){

        if(c == '\n'){
          line_number++;
        }

        if(c == '*'){
          printf("closing comment\n");
          c = fgetc(CurrentFile);
          if(c == '/'){
            printf("closed comment\n");
            //return next token recursively
            return GetNextToken();
          }
          ungetc(c, CurrentFile);

        }

      }
    }
    else{

      ungetc(c, CurrentFile);

    }

  }

  if(c == eof){
    end_reached = 1;
  }


  //there is no comment


  char* word = (char*)malloc(128*sizeof(char));
  int word_i = 0;

  //check for valid input identifier/keyword
  if(isalpha(c) || c == '_'){

    //while c is valid
    while(isalpha(c) || isdigit(c) || c == '_'){

      //store word dynamically
      word[word_i] = c;
      word_i++;

      c = fgetc(CurrentFile);

    }

    //check if word is keyword
    int i = 0;
    for(i; i < 22; i++){

      if(strcmp(word, keywords[i])){

        //string is a kewyword
        token.tp = RESWORD;
        strcpy(token.lx, word);
        token.ln = line_number;
        token.ec = 0;

        free(word);

        return token;

      }

    }

    //if not a keyword then string is identifier.
    if(token.tp == ERR){

      token.tp = ID;
      strcpy(token.lx, word);
      token.ln = line_number;
      token.ec = 0;

      free(word);

      return token;

    }

  }

  //check for number input
  else if(isdigit(c)){

    while(isdigit(c)){

      //store word dynamically
      word[word_i] = c;
      word_i++;

      c = fgetc(CurrentFile);

    }

    //error if a number is followed by characters or a _:
    if(isalpha(c) || c == '_'){

      token.tp = ERR;
      token.ec = 1;
      token.ln = line_number;

      free(word);
      return token;

    }
    else{

      //token is valid int
      token.tp = INT;
      strcpy(token.lx, word);
      token.ec = 0;
      token.ln = line_number;

      free(word);
      return token;

    }

  }





  //create token here

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
  while (!end_reached){

    printf("token lex, %s    token LN, %d \n", t.lx, t.ln);
    t = GetNextToken();

  }


  
	return 0;
}
// do not remove the next line
#endif
