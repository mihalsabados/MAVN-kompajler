/* Autor: Mihal Sabadoš Datum: 30.05.2021. */
#include "LivenessAnalysis.h"

void livenessAnalysis(Instructions& instructions)
{
	//clear in and out of every instruction

	for (Instruction* instr : instructions)
	{
		instr->getIn().clear();
		instr->getOut().clear();
	}

	bool change = true;
	while (change)
	{
		//iterate through instructions from back using reverse iterator
		for (Instructions::reverse_iterator it = instructions.rbegin(); it != instructions.rend(); it++)
		{
			Instruction* instr = *it;

			//copy of lists in and out
			Variables in;
			Variables out;

			for (Variable* var : instr->getIn())
				in.push_back(var);
			
			for (Variable* var : instr->getOut())
				out.push_back(var);

			//for every successor add his 'in' variables to current instruction 'out' variables
			for (Instruction* succ : instr->getSucc())
			{
				for (Variable* var : succ->getIn())
					instr->getOut().push_back(var);
			}

			//add all 'use' variables to 'in' of current instruction
			for (Variable* var : instr->getUse())
			{
				instr->getIn().push_back(var);
			}

			//add every variable from 'out' to 'in' if variable is not in 'def' variables
			for (Variable* var : instr->getOut())
			{
				if(!variableExists(var, instr->getDef()))instr->getIn().push_back(var);
			}

			instr->getIn().sort();
			instr->getIn().unique();

			instr->getOut().sort();
			instr->getOut().unique();

			//check if there are changes or not
			change = false;
			for (Variable* var : instr->getIn())
			{
				if (!variableExists(var, in))
					change = true;
			}

			for (Variable* var : instr->getOut())
			{
				if (!variableExists(var, out))
					change = true;
			}
		}
	}
}

bool variableExists(Variable* variable, Variables variables)
{
	bool ret = false;

	Variables::iterator it;

	for (it = variables.begin(); it != variables.end(); it++)
	{
		if ((*it)->getName() == variable->getName())
		{
			ret = true;
			break;
		}
	}

	return ret;
}
