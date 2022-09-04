/* Autor: Mihal Sabadoš Datum: 30.05.2021. */
#pragma once
#include "LexicalAnalysis.h"
#include "IR.h"

class VarInstrBuilder 
{
public:
	VarInstrBuilder(LexicalAnalysis& lex);

	~VarInstrBuilder();

	//generates list of variables, functions and labels
	void generateVariables();

	//generates list of instructions
	void generateInstructions();

	//generates successors and predecessors for every instruction in list
	void generateSuccPred();

	//checks if variable name already exists in list of variables
	bool variableNameExists(std::string name);

	//checks if function or label exists in list of functions and labels
	bool funcLabelExists(std::string name, FuncLab::Type);

	//finds variable by name in list and returns it
	Variable* getVariable(std::string name);

	//finds type(function or label) by name in list and returns it
	FuncLab* getFuncLabel(std::string name, FuncLab::Type);

	//finds instruction in list by possition
	Instruction* getInstr(int pos);

	//returns list of instructions
	Instructions getInstructions();

	//returns list of variables
	Variables getVariables();

	//returns list of functions and labels
	FuncLabs getFuncLabs();

private:
	//reference to lexical analysis
	LexicalAnalysis& m_lexicalAnalysis;

	//iterator for tokens
	TokenList::iterator m_tokenIterator;

	//list of instructions
	Instructions m_instructions;

	//list of variables
	Variables m_variables;

	//list of functions and labels
	FuncLabs m_funcLabs;

	//variable possition
	int m_varPosition;

	//instruction possition
	int m_instrPosition;
};