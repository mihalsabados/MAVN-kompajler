/* Autor: Mihal Sabadoš Datum: 30.05.2021. */
#pragma once

#include "LexicalAnalysis.h"
#include "Token.h"

class SyntaxAnalysis
{
public:

	SyntaxAnalysis(LexicalAnalysis& lex);

	//Method which performes syntax analysis
	bool Do();

private:
	//Reference to lexical analysis
	LexicalAnalysis& m_lexicalAnalysis;
	//Syntax error indicator
	bool m_errorFound;
	//Token list iterator from lexical analysis
	TokenList::iterator m_tokenIterator;
	//analysed token
	Token m_currentToken;

	//prints syntax error
	void printSyntaxError(Token token);

	//prints token info
	void printTokenInfo(Token token);

	//check for errors
	void eat(TokenType t);


	Token getNextToken();

	void Q();

	void S();

	void L();

	void E();
};