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
const char* keywords[21] = {"class", "constructor", "method", "function",
  "int", "boolean", "char", "void", "var", "static", "field", "let", "do",
  "if", "else", "while", "return", "true", "false", "null", "this"     };

//clobal array of all symbols
const int symbols[20] = {'(', ')', '[', ']', '{', '}', ',', ';', '=', 
  '.', '+', '*', '/', '&', '|', '~', '<', '>', '-'};

//global file pointer
FILE* CurrentFile;

int end_reached = 0;

int line_number;

char* FileName;

// IMPLEMENT THE FOLLOWING functions
//***********************************

// Initialise the lexer to read from source file
// file_name is the name of the source file
// This requires opening the file and making any necessary initialisations of the lexer
// If an error occurs, the function should return 0
// if everything goes well the function should return 1
int InitLexer (char* file_name)
{

  FileName = file_name;

  line_number = 1;




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

  strcpy(token.fl, FileName);

  int x = 0;
  for(x; x < 128; x++){

    token.lx[x] = '\0';

  }


  //c must be int to capture EOF correctly
  int c = fgetc(CurrentFile);

  if(c == eof){

    end_reached = 1;

    token.tp = EOFile;
    strcpy(token.lx, "End Of File");
    token.ec = 0;
    token.ln = line_number;
    //strcpy(token.fl, FileName);

    return token;

  }


  //skip through whitespace
  while(isspace(c)){


    if(c == '\n'){
      line_number++;
    }

    c = fgetc(CurrentFile);

    if(c == eof){

      end_reached = 1;

      token.tp = EOFile;
      strcpy(token.lx, "End Of File");
      token.ec = 0;
      token.ln = line_number;
      //strcpy(token.fl, FileName);

      return token;

    }

  }


  //check for // comments
  if(c == '/'){

    c = fgetc(CurrentFile);
    if(c == '/'){

      //skip all writing until end of line or EOF

      while((c = fgetc(CurrentFile)) != '\n'){

        if(c == eof){

          //eof in comment error
          token.ec = EofInCom;
          token.ln = line_number;
          strcpy(token.lx, "Error: unexpected eof in comment");

          return token;

        }

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
          c = fgetc(CurrentFile);
          if(c == '/'){
            //return next token recursively
            return GetNextToken();
          }
          ungetc(c, CurrentFile);

        }
      }
      if(c == eof){

        //eof in comment error
        token.ec = EofInCom;
        token.ln = line_number;
        strcpy(token.lx, "Error: unexpected eof in comment");

        return token;

      }
    }
    else{

      //else, is a / symbol

      ungetc(c, CurrentFile);

      token.tp = SYMBOL;
      sprintf(token.lx, "%c", '/');
      //strcpy(token.lx, "/");
      token.ec = 0;
      token.ln = line_number;  
      //strcpy(token.fl, FileName);

      return token;    

    }

  }


  //there is no comment

  //check for symbol input
  int i = 0;
  for(i; i < 20; i++){

    if(c == symbols[i]){

      token.tp = SYMBOL;
      sprintf(token.lx, "%c", c);
      token.ec = 0;
      token.ln = line_number;
      //strcpy(token.fl, FileName);

      //free(word);

      return token;

    }

  }

  //check for string input

  if(c == '"'){

    //is a STRING!!!

    int i = 0;
    while((c = fgetc(CurrentFile)) != '"'){

      //is a STRING!!!!

      //error check if read EOF in string or newline in string
      if(c == eof){

        //eof in string error
        token.ec = EofInStr;
        token.ln = line_number;
        //strcpy(token.fl, FileName);

        strcpy(token.lx , "Error: unexpected eof in string constant");
        
        return token;

      }

      if(c == '\n'){

        //newline in string error
        token.ec = NewLnInStr;
        token.ln = line_number;
        //strcpy(token.fl, FileName);

        strcpy(token.lx , "Error: new line in string constant");

        return token;

      }

      token.lx[i] = c;

      i++; 



    }

    //return string token
    token.tp = STRING;
    token.ec = 0;
    //strcpy(token.fl, FileName);
    token.ln = line_number;

    return token;   


  }


  //char* word = (char*)malloc(128*sizeof(char));
  int word_i = 0;

  //check for valid input identifier/keyword
  if(isalpha(c) || c == '_'){

    //while c is valid
    while(isalpha(c) || isdigit(c) || c == '_'){

      //store word dynamically
      token.lx[word_i] = c;
      word_i++;

      c = fgetc(CurrentFile);

    }

    ungetc(c, CurrentFile);



    //check if word is keyword
    int i = 0;
    for(i; i < 21; i++){

      if(!strcmp(token.lx, keywords[i])){

        //string is a kewyword
        token.tp = RESWORD;
        //strcpy(token.lx, word);
        token.ln = line_number;
        token.ec = 0;
        //strcpy(token.fl, FileName);


        return token;

      }

    }

    //if not a keyword then string is identifier.
    if(token.tp == ERR){

      token.tp = ID;
      // strcpy(token.lx, word);
      token.ln = line_number;
      token.ec = 0;
      //strcpy(token.fl, FileName);

      //free(word);

      return token;

    }

  }

  //check for number input
  else if(isdigit(c)){

    while(isdigit(c)){

      //store word dynamically
      token.lx[word_i] = c;
      word_i++;

      c = fgetc(CurrentFile);

    }
    
    ungetc(c, CurrentFile);

    //error if a number is followed by characters or a _:
    if(isalpha(c) || c == '_'){

      token.tp = ERR;
      token.ec = 1;
      token.ln = line_number;
      //strcpy(token.fl, FileName);

      //free(word);
      return token;

    }
    else{

      //token is valid int
      token.tp = INT;
      //strcpy(token.lx, word);
      token.ec = 0;
      token.ln = line_number;
      //strcpy(token.fl, FileName);

      //free(word);
      return token;

    }

  }
  else{

    //illegal symbol
    token.ec = IllSym;
    token.ln = line_number;
    strcpy(token.lx, "Error: illegal symbol in source file");

    return token;

  }

}

// peek (look) at the next token in the source file without removing it from the stream
Token PeekNextToken ()
{
  Token t;
  fpos_t pos;
  fgetpos(CurrentFile, &pos);
  int currLine = line_number;
  t = GetNextToken();
  line_number = currLine;
  fsetpos(CurrentFile, &pos);
  return t;

}

// clean out at end, e.g. close files, free memory, ... etc
int StopLexer ()
{

  fclose(CurrentFile);

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

    printf("token lex, %s    token LN, %d     token TP = %d\n", t.lx, t.ln, t.tp);
    t = GetNextToken();

  }


  
	return 0;
}
// do not remove the next line
#endif
