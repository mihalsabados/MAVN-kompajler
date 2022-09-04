/* Autor: Mihal Sabadoš Datum: 30.05.2021. */
#include "IR.h"

std::string Variable::getName()
{
    return m_name;
}

std::string Variable::getAssignmentName()
{
    switch (m_assignment)
    {
    case t0:return "t0";
    case t1:return "t1";
    case t2:return "t2";
    case t3:return "t3";
    case t4:return "t4";
    case no_assign:return "no assign";
    }
}

int Variable::getPosition()const
{
    return m_position;
}

int Variable::getValue()
{
    return m_value;
}

Variable::VariableType Variable::getType()
{
    return m_type;
}

void Variable::setType(VariableType type)
{
    m_type = type;
}

void Variable::setValue(int value)
{
    m_value = value;
}

Regs Variable::getAssignment()
{
    return m_assignment;
}

void Variable::setAssignment(Regs reg)
{
    m_assignment = reg;
}

std::string FuncLab::getName()
{
    return m_name;
}

void FuncLab::setType(Type type)
{
    m_type = type;
}

FuncLab::Type FuncLab::getType()
{
    return m_type;
}

int FuncLab::getPosition()
{
    return m_position;
}

InstructionType Instruction::getType()
{
    return m_type;
}

std::string Instruction::getName()
{
    switch (m_type)
    {
    case I_ADD:return "add";
    case I_ADDI:return "addi";
    case I_SUB:return "sub";
    case I_LA:return "la";
    case I_LW:return "lw";
    case I_LI:return "li";
    case I_SW:return "sw";
    case I_B:return "b";
    case I_BLTZ:return "bltz";
    case I_NOP:return "nop";
    case I_MULT:return "mult";
    case I_OR:return "or";
    case I_SEQ:return "seq";
    default:return "no type";
    }
}

int Instruction::getPosition()
{
    return m_position;
}

int Instruction::getOffset()
{
    return m_offset;
}

void Instruction::setOffset(int offSet)
{
    m_offset = offSet;
}

void Instruction::setLabel(FuncLab* funcLab)
{
    m_label = funcLab;
}

FuncLab* Instruction::getLabel()
{
    return m_label;
}

Variables& Instruction::getIn()
{
    return m_in;
}

Variables& Instruction::getOut()
{
    return m_out;
}

Variables& Instruction::getUse()
{
    return m_use;
}

Variables& Instruction::getDef()
{
    return m_def;
}

Variables& Instruction::getDest()
{
    return m_dst;
}

Variables& Instruction::getSrc()
{
    return m_src;
}

void Instruction::setIn(Variables vars)
{
    m_in = vars;
}

void Instruction::setOut(Variables vars)
{
    m_out = vars;
}

void Instruction::setUse(Variables vars)
{
    m_use = vars;
}

void Instruction::setDef(Variables vars)
{
    m_def = vars;
}

std::list<Instruction*>& Instruction::getSucc()
{
    return m_succ;
}

std::list<Instruction*>& Instruction::getPred()
{
    return m_pred;
}

void Instruction::setSucc(std::list<Instruction*> succ)
{
    m_succ = succ;
}

void Instruction::setPred(std::list<Instruction*> pred)
{
    m_pred = pred;
}

Variable* InterferenceGraphStruct::findVariable(int pos)
{
    for (Variable* var : *m_variables)
    {
        if (var->getPosition() == pos)return var;
    }
    return NULL;
}
