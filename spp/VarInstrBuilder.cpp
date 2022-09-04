/* Autor: Mihal Sabadoš Datum: 30.05.2021. */
#include "VarInstrBuilder.h"

VarInstrBuilder::VarInstrBuilder(LexicalAnalysis& lex):
	m_lexicalAnalysis(lex), m_tokenIterator(lex.getTokenList().begin()), m_varPosition(0), m_instrPosition(0)
{
}

VarInstrBuilder::~VarInstrBuilder()
{
	for (Instruction* instr : m_instructions)
		delete instr;
	for (Variable* var : m_variables)
		delete var;
	for (FuncLab* fLab : m_funcLabs)
		delete fLab;
}

void VarInstrBuilder::generateVariables()
{
	//iterate through token list from lexical analysis
	for (auto it = m_tokenIterator; it != m_lexicalAnalysis.getTokenList().end(); it++)
	{
		if (it->getType() == T_MEM)
		{
			//create memory variable if that variable doesn't exist

			std::string varName = (++it)->getValue();
			int varValue = stoi((++it)->getValue());
			if (!variableNameExists(varName))
			{
				Variable* var = new Variable(varName, -1);
				var->setType(Variable::MEM_VAR);
				var->setValue(varValue);
				m_variables.push_back(var);
			}
			else throw "Memory variable " + varName + " already exists!";
		}
		else if (it->getType() == T_REG)
		{
			//creates registry variable if that variable doesn't exist

			std::string varName = (++it)->getValue();
			if (!variableNameExists(varName))
			{
				Variable* var = new Variable(varName, m_varPosition++);
				var->setType(Variable::REG_VAR);
				m_variables.push_back(var);
			}
			else throw "Register variable " + varName + " already exists!";
		}
		else if (it->getType() == T_FUNC)
		{
			//creates function if that function doesn't exist

			std::string funcName = (++it)->getValue();
			if (!funcLabelExists(funcName, FuncLab::FUNC))
			{
				FuncLab* funcLab = new FuncLab(funcName, m_instrPosition);
				funcLab->setType(FuncLab::FUNC);
				m_funcLabs.push_back(funcLab);
			}
			else throw "Function " + funcName + " already exists!";
		}
		else if (it->getType() == T_ID)
		{
			if (next(it,1)->getType() == T_COL)
			{
				//creates label if that label doesn't exist

				std::string labName = it->getValue();
				if (!funcLabelExists(labName, FuncLab::LABEL))
				{
					FuncLab* funcLab = new FuncLab(labName, m_instrPosition++);
					funcLab->setType(FuncLab::LABEL);
					m_funcLabs.push_back(funcLab);
				}
				else throw "Label " + labName + " already exists!";
			}
		}
		else if (it->getType() == T_ADD || it->getType() == T_SUB || 
			it->getType() == T_ADDI || it->getType() == T_LA || it->getType() == T_LI
			|| it->getType() == T_LW || it->getType() == T_SW || it->getType() == T_BLTZ
			|| it->getType() == T_B || it->getType() == T_NOP || it->getType() == T_MULT 
			|| it->getType() == T_OR || it->getType() == T_SEQ)m_instrPosition++;
	}

}

void VarInstrBuilder::generateInstructions()
{
	m_instrPosition = 0;

	//iterate through token list from lexical analysis
	for (auto it = m_tokenIterator; it != m_lexicalAnalysis.getTokenList().end(); it++)
	{
		//create variable list src and dest which will be added to instruction
		Variables src;
		Variables dest;
		Instruction* instr;
		if (it->getType() == T_ADD || it->getType() == T_SUB)
		{
			InstructionType instType = (it->getType() == T_ADD) ? InstructionType::I_ADD : InstructionType::I_SUB;

			//++it for next Token
			//find variable in m_variables list and add to dest
			dest.push_back(getVariable((++it)->getValue()));
			//skip comma
			++it;
			src.push_back(getVariable((++it)->getValue()));
			++it;
			src.push_back(getVariable((++it)->getValue()));
			instr = new Instruction(m_instrPosition++, instType, dest, src, dest, src);
			m_instructions.push_back(instr);
		}
		else if (it->getType() == T_ADDI)
		{
			dest.push_back(getVariable((++it)->getValue()));
			++it;
			src.push_back(getVariable((++it)->getValue()));
			++it;
			//create new variable for constants
			src.push_back(new Variable((++it)->getValue(), -1));
			instr = new Instruction(m_instrPosition++, InstructionType::I_ADDI, dest, src, dest, src);
			m_instructions.push_back(instr);
		}
		else if (it->getType() == T_LA)
		{
			dest.push_back(getVariable((++it)->getValue()));
			++it;
			src.push_back(getVariable((++it)->getValue()));
			instr = new Instruction(m_instrPosition++, InstructionType::I_LA, dest, src, dest, src);
			m_instructions.push_back(instr);
		}
		else if (it->getType() == T_LI)
		{
			dest.push_back(getVariable((++it)->getValue()));
			++it;
			src.push_back(new Variable((++it)->getValue(), -1));
			instr = new Instruction(m_instrPosition++, InstructionType::I_LI, dest, src, dest, src);
			m_instructions.push_back(instr);
		}
		else if (it->getType() == T_LW || it->getType() == T_SW)
		{
			InstructionType instType = (it->getType() == T_LW) ? InstructionType::I_LW : InstructionType::I_SW;

			dest.push_back(getVariable((++it)->getValue()));
			++it;
			int offset = stoi((++it)->getValue());
			++it;
			src.push_back(getVariable((++it)->getValue()));
			instr = new Instruction(m_instrPosition++, instType, dest, src, dest, src);
			instr->setOffset(offset);
			m_instructions.push_back(instr);
		}
		else if (it->getType() == T_BLTZ)
		{
			src.push_back(getVariable((++it)->getValue()));
			++it;
			FuncLab* label = getFuncLabel((++it)->getValue(), FuncLab::LABEL);
			instr = new Instruction(m_instrPosition++, InstructionType::I_BLTZ, dest, src, dest, src);
			instr->setLabel(label);
			m_instructions.push_back(instr);
		}
		else if (it->getType() == T_B)
		{
			FuncLab* label = getFuncLabel((++it)->getValue(), FuncLab::LABEL);
			instr = new Instruction(m_instrPosition++, InstructionType::I_B, dest, src, dest, src);
			instr->setLabel(label);
			m_instructions.push_back(instr);
		}
		else if (it->getType() == T_NOP)
		{
			instr = new Instruction(m_instrPosition++, InstructionType::I_NOP, dest, src, dest, src);
			m_instructions.push_back(instr);
		}
		else if (it->getType() == T_MULT)
		{
			dest.push_back(getVariable((++it)->getValue()));
			++it;
			src.push_back(getVariable((++it)->getValue()));
			++it;
			instr = new Instruction(m_instrPosition++, InstructionType::I_MULT, dest, src, dest, src);
			m_instructions.push_back(instr);
		}
		else if (it->getType() == T_OR)
		{
			dest.push_back(getVariable((++it)->getValue()));
			++it;
			src.push_back(getVariable((++it)->getValue()));
			++it;
			src.push_back(getVariable((++it)->getValue()));
			instr = new Instruction(m_instrPosition++, InstructionType::I_OR, dest, src, dest, src);
			m_instructions.push_back(instr);
		}
		else if (it->getType() == T_SEQ)
		{
			dest.push_back(getVariable((++it)->getValue()));
			++it;
			src.push_back(getVariable((++it)->getValue()));
			++it;
			src.push_back(getVariable((++it)->getValue()));
			instr = new Instruction(m_instrPosition++, InstructionType::I_SEQ, dest, src, dest, src);
			m_instructions.push_back(instr);
		}
	}
}

void VarInstrBuilder::generateSuccPred()
{
	//iterate through generated list of instructions
	for (auto it = m_instructions.begin(); it != m_instructions.end(); it++)
	{
		Instruction* instr = *it;

		// get next instruction
		auto it1 = next(it);

		if (it1 != m_instructions.end())
		{
			//every instruction except 'b' has successor
			if(instr->getType() != InstructionType::I_B)
				instr->getSucc().push_back(*it1);

			(*it1)->getPred().push_back(instr);
			if ((*it1)->getType() == InstructionType::I_BLTZ || (*it1)->getType() == InstructionType::I_B)
			{
				//adding jump successor

				FuncLab* label = (*it1)->getLabel();
				int pos = label->getPosition();
				Instruction* instr1 = getInstr(pos);
				instr1->getPred().push_back(*it1);
				(*it1)->getSucc().push_back(instr1);
			}
		}
	}
}

bool VarInstrBuilder::variableNameExists(std::string name)
{
	for (auto varIt = m_variables.begin(); varIt != m_variables.end(); varIt++) {
		if ((*varIt)->getName() == name)
			return true;
	}
	return false;
}

bool VarInstrBuilder::funcLabelExists(std::string name, FuncLab::Type type)
{
	for (auto labFuncIt = m_funcLabs.begin(); labFuncIt != m_funcLabs.end(); labFuncIt++) {
		if ((*labFuncIt)->getName() == name && (*labFuncIt)->getType() == type)
			return true;
	}
	return false;
}

Variable* VarInstrBuilder::getVariable(std::string name)
{
	for (auto varIt = m_variables.begin(); varIt != m_variables.end(); varIt++)
	{
		if ((*varIt)->getName() == name)
			return *varIt;
	}
	throw "Undefined variable " + name + " !";
	return NULL;
}

FuncLab* VarInstrBuilder::getFuncLabel(std::string name, FuncLab::Type type)
{
	for (auto labFuncIt = m_funcLabs.begin(); labFuncIt != m_funcLabs.end(); labFuncIt++) {
		if ((*labFuncIt)->getName() == name && (*labFuncIt)->getType() == type)
			return *labFuncIt;
	}
	throw "Undefined " + name + " !";
	return NULL;
}

Instruction* VarInstrBuilder::getInstr(int pos)
{
	for (auto it = m_instructions.begin(); it != m_instructions.end(); it++)
	{
		if (pos == (*it)->getPosition())return *it;
	}
	return NULL;
}

Instructions VarInstrBuilder::getInstructions()
{
	return m_instructions;
}

Variables VarInstrBuilder::getVariables()
{
	return m_variables;
}

FuncLabs VarInstrBuilder::getFuncLabs()
{
	return m_funcLabs;
}
