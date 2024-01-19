#ifndef PARSER_H
#define PARSER_H

void stmt();
void condition();
void expr();
void term();
void factor();
void error();
void program();

extern int nextToken;
extern char lexeme[100];
extern int errortail;
extern char* LexNames();
#endif
