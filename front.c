/**
 * This the example lexical analyzer code in pages 173 - 177 of the
 * textbook,
 *
 * Sebesta, R. W. (2012). Concepts of Programming Languages.
 * Pearson, 10th edition.
 *
 */

/* front.c - a lexical analyzer system for simple arithmetic expressions */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "front.h"
#include "parser.h"

/* Global Variable */
int nextToken;
int errortail = 0;

/* Local Variables */
static int charClass;
char lexeme[100];
static char nextChar;
static int lexLen;
static FILE *in_fp;

/* Local Function declarations */
static void addChar();
static void getChar();
static void getNonBlank();

char look_ahead_in_file();

int main(int argc, char *argv[]) {
  /* Open the input data file and process its contents */
  printf("DCooke Analyzer :: R11720649 \n \n");
  if (argc < 2) {
    printf("Error - No file provided");
    exit(2);
  } else if ((in_fp = fopen(argv[1], "r")) == NULL) {
    printf("ERROR - cannot open file \n");
    exit(3);
  } else {
    getChar();
    do {
      lex();
      program();
    } while (nextToken != EOF);
    
     if (errortail == 0) {
        printf("Syntax Validated");
    }
  }
  return 0;
}

char *LexNames(int token) {
  switch (token) {
  case LEFT_PAREN:
    return "LEFT_PAREN";
  case RIGHT_PAREN:
    return "RIGHT_PAREN";
  case ADD_OP:
    return "ADD_OP";
  case SUB_OP:
    return "SUB_OP";
  case MULT_OP:
    return "MULT_OP";
  case DIV_OP:
    return "DIV_OP";
  case LESSER_OP:
    return "LESSER_OP";
  case GREATER_OP:
    return "GREATER_OP";
  case ASSIGN_OP:
    return "ASSIGN_OP";
  case SEMICOLON:
    return "SEMICOLON";
  case LEFT_CBRACE:
    return "LEFT_CBRACE";
  case RIGHT_CBRACE:
    return "RIGHT_CBRACE";
  case INC_OP:
    return "INC_OP";
  case DEC_OP:
    return "DEC_OP";
  case LEQUAL_OP:
    return "LEQUAL_OP";
  case GEQUAL_OP:
    return "GEQUAL_OP";
  case EQUAL_OP:
    return "EQUAL_OP";
  case NEQUAL_OP:
    return "NEQUAL_OP";
  case IDENT:
    return "IDENT";
  case INT_LIT:
    return "INT_LIT";
  case EOF:
    return "EOF";
  case KEY_WRITE:
    return "KEY_WRITE";
  case KEY_READ:
    return "KEY_READ";
  case KEY_WHILE:
    return "KEY_WHILE";
  case KEY_DO:
    return "KEY_DO";
  default:
    return "UNKNOWN";
  }
}

/******************************************************/
/* main driver */

char look_ahead_in_file() {
  char c = fgetc(in_fp);
  ungetc(c, in_fp);
  return c;
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses and return the
 * token */
static int lookup(char ch) {
  switch (ch) {
  case '(':
    addChar();
    nextToken = LEFT_PAREN;
    break;
  case ')':
    addChar();
    nextToken = RIGHT_PAREN;
    break;
  case '+':
    if (look_ahead_in_file() == '+') {
      addChar();
      getChar();
      addChar();
      nextToken = INC_OP;
    } else {
      addChar();
      nextToken = ADD_OP;
    }
    break;
  case '-':
    if (look_ahead_in_file() == '-') {
      addChar();
      getChar();
      addChar();
      nextToken = DEC_OP;
    } else {
      addChar();
      nextToken = SUB_OP;
    }
    break;
  case '*':
    addChar();
    nextToken = MULT_OP;
    break;
  case '/':
    addChar();
    nextToken = DIV_OP;
    break;
  case '<':
    if (look_ahead_in_file() == '=') {
      addChar();
      getChar();
      addChar();
      nextToken = LEQUAL_OP;
    } else {
      addChar();
      nextToken = LESSER_OP;
    }
    break;
  case '>':
    if (look_ahead_in_file() == '=') {
      addChar();
      getChar();
      addChar();
      nextToken = GEQUAL_OP;
    } else {
      addChar();
      nextToken = GREATER_OP;
    }
    break;
  case '=':
    if (look_ahead_in_file() == '=') {
      addChar();
      getChar();
      addChar();
      nextToken = EQUAL_OP;
    } else {
      addChar();
      nextToken = ASSIGN_OP;
    }
    break;
  case '!':
    if (look_ahead_in_file() == '=') {
      addChar();
      getChar();
      addChar();
      nextToken = NEQUAL_OP;
    } else {
      addChar();
      nextToken = UNKNOWN;
    }
    break;
  case ';':
    addChar();
    nextToken = SEMICOLON;
    break;
  case '{':
    addChar();
    nextToken = LEFT_CBRACE;
    break;
  case '}':
    addChar();
    nextToken = RIGHT_CBRACE;
    break;
  default:
    addChar();
    nextToken = UNKNOWN;
    break;
  }
  return nextToken;
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
static void addChar() {
  if (lexLen <= 98) {
    lexeme[lexLen++] = nextChar;
    lexeme[lexLen] = 0;
  } else {
    printf("Error - lexeme is too long \n");
  }
}

/*****************************************************/
/* getChar - a function to get the next character of input and determine its
 * character class */
static void getChar() {
  if ((nextChar = getc(in_fp)) != EOF) {
    if (isalpha(nextChar))
      charClass = LETTER;
    else if (isdigit(nextChar))
      charClass = DIGIT;
    else
      charClass = UNKNOWN;
  } else {
    charClass = EOF;
  }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace
 * character */
static void getNonBlank() {
  while (isspace(nextChar))
    getChar();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
  lexLen = 0;
  getNonBlank();

  switch (charClass) {
  /* Parse identifiers */
  case LETTER:
    addChar();
    getChar();
    while (charClass == LETTER || charClass == DIGIT) {
      addChar();
      getChar();
    }

    if (strcmp(lexeme, "write") == 0) {
      nextToken = KEY_WRITE;
      break;
    } else if (strcmp(lexeme, "do") == 0) {
      nextToken = KEY_DO;
      break;
    } else if (strcmp(lexeme, "while") == 0) {
      nextToken = KEY_WHILE;
      break;
    } else if (strcmp(lexeme, "read") == 0) {
      nextToken = KEY_READ;
      break;
    }

    nextToken = IDENT;
    break;

  /* Parse integer literals */
  case DIGIT:
    addChar();
    getChar();
    while (charClass == DIGIT) {
      addChar();
      getChar();
    }
    nextToken = INT_LIT;
    break;

  /* Parentheses and operators */
  case UNKNOWN:
    lookup(nextChar);
    getChar();
    break;

    /* EOF */
  case EOF:
    nextToken = EOF;
    lexeme[0] = 'E';
    lexeme[1] = 'O';
    lexeme[2] = 'F';
    lexeme[3] = '0';
    break;
  } /* End of switch */

  if (nextToken != EOF)
    printf("%s \t %s\n", lexeme, LexNames(nextToken));
  return nextToken;
} /* End of function lex */
