/* Autor: Mihal Sabadoš Datum: 30.05.2021. */
#include "File.h"

#include <string>
#include <iostream>
#include <fstream>


using namespace std;

void writeFile(string fileName, Variables vars, FuncLabs funcLabs, Instructions instrs)
{

	//open file for writting
	ofstream out(fileName);

	out << ".globl main" << endl << endl;
	out << ".data" << endl;

	//writes variables first
	for (Variable* var : vars)
	{
		if (var->getType() == Variable::MEM_VAR)
		{
			out << var->getName() << ":\t.word " << var->getValue() << endl;
		}
	}
	out << endl << ".text" << endl;

	//writes functions, labels and instructions based on their possition
	int pos = 0;
	auto funLabIt = funcLabs.begin();
	auto instrIt = instrs.begin();
	while (instrIt != instrs.end() || funLabIt != funcLabs.end())
	{
		if (funLabIt != funcLabs.end() && (*funLabIt)->getPosition() == pos)
		{
			out << (*funLabIt)->getName() << ":" << endl;
			funLabIt++;
		}
		if (instrIt != instrs.end() && (*instrIt)->getPosition() == pos)
		{
			Instruction* instr = *instrIt;
			out << "\t" << instr->getName() << "\t";
			for (Variable* var : instr->getDef())
			{
				if (var->getAssignment() != no_assign)
					out << "$" << var->getAssignmentName();
				else
					out << var->getName();
			}
			for (Variable* var : instr->getUse())
			{
				if (instr->getType() == I_LW || instr->getType() == I_SW)
				{
					out <<", "<< instr->getOffset() << "($" << var->getAssignmentName() << ")  ";
				}
				else
				{
					if (instr->getType() != I_BLTZ && instr->getType() != I_B)
						out << ",";
					if (var->getAssignment() != no_assign)
						out << " $" << var->getAssignmentName();
					else
						out << " " <<var->getName();
				}
			}
			if (instr->getType() == I_BLTZ || instr->getType() == I_B)out <<", "<< instr->getLabel()->getName();
			out << endl;
			instrIt++;
		}
		pos++;
	}
	out.close();
}
