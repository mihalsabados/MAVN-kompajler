/* Autor: Mihal Sabadoš Datum: 30.05.2021. */
#ifndef __IR__
#define __IR__

#include "Types.h"

class FuncLab
{
public:
	enum Type
	{
		LABEL,
		FUNC,
		NO_TYPE
	};

	FuncLab() : m_type(NO_TYPE), m_name(""), m_position(-1) {}
	FuncLab(std::string name, int pos) : m_type(NO_TYPE), m_name(name), m_position(pos) {}

	std::string getName();

	void setType(Type type);
	Type getType();

	int getPosition();

private:
	Type m_type;
	std::string m_name;
	int m_position;
};

/**
 * This type represents list of instructions from program code.
 */
typedef std::list<FuncLab*> FuncLabs;


/**
 * This class represents one variable from program code.
 */
class Variable
{
public:
	enum VariableType
	{
		MEM_VAR,
		REG_VAR,
		NO_TYPE
	};

	Variable() : m_type(NO_TYPE), m_name(""), m_position(-1), m_assignment(no_assign), m_value(0) {}
	Variable(std::string name, int pos) : m_type(NO_TYPE), m_name(name), m_position(pos), m_assignment(no_assign), m_value(0) {}

	std::string getName();
	std::string getAssignmentName();
	int getPosition()const;
	int getValue();
	VariableType getType();
	void setType(VariableType type);
	void setValue(int value);

	Regs getAssignment();
	void setAssignment(Regs reg);

private:
	VariableType m_type;
	std::string m_name;
	int m_position;
	Regs m_assignment;
	int m_value;
};


/**
 * This type represents list of variables from program code.
 */
typedef std::list<Variable*> Variables;


/**
 * This class represents one instruction in program code.
 */
class Instruction
{
public:
	Instruction () : m_position(0), m_type(I_NO_TYPE), m_offset(0) {}
	Instruction (int pos, InstructionType type, Variables& dst, Variables& src) :
		m_position(pos), m_type(type), m_dst(dst), m_src(src), m_offset(0) {}
	
	Instruction (int pos, InstructionType type, Variables& dst, Variables& src, Variables def, Variables use) :
		m_position(pos), m_type(type), m_dst(dst), m_src(src), m_offset(0), m_use(use), m_def(def) {}

	InstructionType getType();
	std::string getName();
	int getPosition();
	int getOffset();
	void setOffset(int offSet);
	void setLabel(FuncLab* funcLab);
	FuncLab* getLabel();
	Variables& getIn();
	Variables& getOut();
	Variables& getUse();
	Variables& getDef();
	Variables& getDest();
	Variables& getSrc();
	void setIn(Variables vars);
	void setOut(Variables vars);
	void setUse(Variables vars);
	void setDef(Variables vars);

	std::list<Instruction*>& getSucc();
	std::list<Instruction*>& getPred();
	void setSucc(std::list<Instruction*> succ);
	void setPred(std::list<Instruction*> pred);

private:
	int m_position;
	int m_offset;
	InstructionType m_type;
	
	Variables m_dst;
	Variables m_src;
	FuncLab* m_label;

	Variables m_use;
	Variables m_def;
	Variables m_in;
	Variables m_out;
	std::list<Instruction*> m_succ;
	std::list<Instruction*> m_pred;
};


/**
 * This type represents list of instructions from program code.
 */
typedef std::list<Instruction*> Instructions;


typedef std::vector<std::vector<int>> InterferenceMatrix;

typedef struct InterferenceGraphStruct
{
	Variables* m_variables;
	InterferenceMatrix m_matrix;

	InterferenceGraphStruct()
	{
	};
	Variable* findVariable(int pos);
}InterferenceGraph;

#endif
