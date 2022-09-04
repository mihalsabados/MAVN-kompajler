/* Autor: Mihal Sabadoš Datum: 30.05.2021. */
#include "SyntaxAnalysis.h"

#include "stdio.h"
#include <iomanip>

using namespace std;

SyntaxAnalysis::SyntaxAnalysis(LexicalAnalysis& lex)
	:m_lexicalAnalysis(lex), m_errorFound(false), m_tokenIterator(m_lexicalAnalysis.getTokenList().begin())
{

}

bool SyntaxAnalysis::Do()
{
	m_currentToken = getNextToken();

	Q();

	return !m_errorFound;
}

void SyntaxAnalysis::printSyntaxError(Token token)
{
	cout << "Syntax error! Token: " << token.getValue() << " unexpected" << endl;
}

void SyntaxAnalysis::printTokenInfo(Token token)
{
	cout << setw(20) << left << token.tokenTypeToString(token.getType());
	cout << setw(25) << right << token.getValue() << endl;
}

void SyntaxAnalysis::eat(TokenType t)
{
	if (m_errorFound == false)
	{
		if (m_currentToken.getType() == t)
		{
			cout << m_currentToken.getValue() << endl;
			if(m_currentToken.getType() != T_END_OF_FILE)m_currentToken = getNextToken();
		}
		else
		{
			printSyntaxError(m_currentToken);
			m_errorFound = true;
		}
	}
}

Token SyntaxAnalysis::getNextToken()
{
	if (m_tokenIterator == m_lexicalAnalysis.getTokenList().end())
		throw runtime_error("End of input file reached");
	return *m_tokenIterator++;
}

void SyntaxAnalysis::Q()
{
	S();
	eat(T_SEMI_COL);
	L();
}

void SyntaxAnalysis::S()
{
	if (m_currentToken.getType() == T_MEM)
	{
		eat(T_MEM);
		eat(T_M_ID);
		eat(T_NUM);
	}
	else if (m_currentToken.getType() == T_REG)
	{
		eat(T_REG);
		eat(T_R_ID);
	}
	else if (m_currentToken.getType() == T_FUNC)
	{
		eat(T_FUNC);
		eat(T_ID);
	}
	else if (m_currentToken.getType() == T_ID)
	{
		eat(T_ID);
		eat(T_COL);
		E();
	}
	else
	{
		E();
	}
}

void SyntaxAnalysis::L()
{
	if (m_currentToken.getType() == T_END_OF_FILE)
	{
		eat(T_END_OF_FILE);
	}
	else
	{
		Q();
	}
}

void SyntaxAnalysis::E()
{
	if (m_currentToken.getType() == T_ADD)
	{
		eat(T_ADD);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
	}
	else if (m_currentToken.getType() == T_ADDI)
	{
		eat(T_ADDI);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
	}
	else if (m_currentToken.getType() == T_SUB)
	{
		eat(T_SUB);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
	}
	else if (m_currentToken.getType() == T_LA)
	{
		eat(T_LA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_M_ID);
	}
	else if (m_currentToken.getType() == T_LW)
	{
		eat(T_LW);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
		eat(T_L_PARENT);
		eat(T_R_ID);
		eat(T_R_PARENT);
	}
	else if (m_currentToken.getType() == T_LI)
	{
		eat(T_LI);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
	}
	else if (m_currentToken.getType() == T_SW)
	{
		eat(T_SW);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
		eat(T_L_PARENT);
		eat(T_R_ID);
		eat(T_R_PARENT);

	}
	else if (m_currentToken.getType() == T_B)
	{
		eat(T_B);
		eat(T_ID);
	}
	else if (m_currentToken.getType() == T_BLTZ)
	{
		eat(T_BLTZ);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_ID);
	}
	else if (m_currentToken.getType() == T_NOP)
	{
		eat(T_NOP);
	}
	else if (m_currentToken.getType() == T_MULT)
	{
		eat(T_MULT);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
	}
	else if (m_currentToken.getType() == T_OR)
	{
		eat(T_OR);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
	}
	else if (m_currentToken.getType() == T_SEQ)
	{
		eat(T_SEQ);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
	}
}
