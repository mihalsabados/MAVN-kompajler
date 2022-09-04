/* Autor: Mihal Sabadoš Datum: 30.05.2021. */
#include "ResourceAllocation.h"

#include "LivenessAnalysis.h"

using namespace std;

InterferenceGraph* buildInterferenceGraph(Instructions& instructions)
{

    InterferenceGraph* ig = new InterferenceGraph();
    ig->m_variables = new Variables();

    //add all variables from 'def' of every instruction to graph variables
    for (Instruction* instr : instructions)
    {
        for (Variable* varDef : instr->getDef())
        {
            ig->m_variables->push_back(varDef);
        }
    }

    //sort variables based on their possition
    ig->m_variables->sort([](const Variable* var1, const Variable* var2)
        {
            return var1->getPosition() < var2->getPosition();
        });
    ig->m_variables->unique();

    //initialize empty matrix
    for (int i = 0; i < ig->m_variables->size(); i++)
    {
        vector<int> vec(ig->m_variables->size());
        ig->m_matrix.push_back(vec);
    }

    //write interference to matrix where variables from 'def' and 'out' collide
    for (Instruction* instr : instructions)
    {
        for (Variable* varDef : instr->getDef())
        {
            for (Variable* varOut : instr->getOut())
            {
                if (varOut->getPosition() != varDef->getPosition() && varDef->getType() == Variable::REG_VAR 
                    && varOut->getType() == Variable::REG_VAR)
                {
                    ig->m_matrix[varDef->getPosition()][varOut->getPosition()] = __INTERFERENCE__;
                    ig->m_matrix[varOut->getPosition()][varDef->getPosition()] = __INTERFERENCE__;
                }
            }
        }
    }
    return ig;
}

stack<Variable*>* doSimplification(InterferenceGraph* ig)
{

    stack<Variable*>* simplificationStack = new stack<Variable*>();
    Variables vars;

    //create copy of interference graph ig

    InterferenceGraph* igCopy = new InterferenceGraph();
    igCopy->m_variables = ig->m_variables;
    igCopy->m_matrix = ig->m_matrix;

    //counter of interference for variable
    int count;
    //k is level of interference, max level __REG_NUMBER__
    int k = 1;
    while (k <= __REG_NUMBER__)
    {
        for (int i = 0; i < ig->m_matrix.size(); i++)
        {
            count = 0;
            //count interference
            for (int j = 0; j < ig->m_matrix[i].size(); j++)
            {
                if (igCopy->m_matrix[i][j] == __INTERFERENCE__)count++;
            }
            if (count == __REG_NUMBER__ - k)
            {
                //variable degree is max
                Variable* var = igCopy->findVariable(i);
                if(!variableExists(var, vars))
                    vars.push_back(var);
                //fill taken variable with no interference
                for (int j = 0; j < ig->m_matrix[i].size(); j++)
                {
                    igCopy->m_matrix[j][i] = __EMPTY__;
                    igCopy->m_matrix[i][j] = __EMPTY__;
                }
                //reset iteration if level is not max
                if (__REG_NUMBER__ - k != 0)
                    i = -1;
            }
        }
        //increase level
        k++;
    }

    //copy from variable list to stack
    for (Variable* var : vars)
        simplificationStack->push(var);

    //spill detection
    if (simplificationStack->size() != ig->m_variables->size())
        throw "Spill!!!";
    return simplificationStack;
}

bool doResourceAllocation(stack<Variable*>* simplificationStack, InterferenceGraph* ig)
{
    while (!simplificationStack->empty())
    {   
        //takes variable from stack and removes it
        Variable* var = simplificationStack->top();
        simplificationStack->pop();

        //creates bool array for free registers and set all to true
        bool regFree[__REG_NUMBER__];
        std::fill_n(regFree, __REG_NUMBER__, true);

        //mark taken registers in bool array with false
        for (Variable* var2 : *ig->m_variables)
        {
            if (ig->m_matrix[var->getPosition()][var2->getPosition()] == __INTERFERENCE__)
            {
                if (var2->getAssignment() != no_assign)
                {
                    regFree[var2->getAssignment()-1] = false;
                }
            }
        }

        //set free register from array to variable
        bool found = false;
        for (int i = 0; i < __REG_NUMBER__; i++)
        {
            if (regFree[i])
            {
                var->setAssignment(static_cast<Regs>(i+1));
                found = true;
                break;
            }
        }
        if (!found)return false;
    }
    return true;
}

void printInterferenceMatrix(InterferenceGraph ig)
{
    if (ig.m_matrix.size() == 0)
    {
        cout << "There is nothing to print!" << endl;
        return;
    }

    cout << "==========================================" << endl;
    cout << "Interference matrix:" << endl;
    cout << "==========================================" << endl;

    // print existing variables in order to mark rows and columns
    for (auto varIt = ig.m_variables->begin(); varIt != ig.m_variables->end(); varIt++)
    {
        cout << "\t" << (*varIt)->getName();
    }
    cout << endl;

    auto varIt = ig.m_variables->begin();
    for (auto it1 = ig.m_matrix.begin(); it1 != ig.m_matrix.end(); it1++)
    {
        cout << (*varIt++)->getName();
        for (auto it2 = (*it1).begin(); it2 != (*it1).end(); it2++)
        {
            cout << "\t" << *it2;
        }
        cout << endl;
    }
    cout << "==========================================" << endl;
}