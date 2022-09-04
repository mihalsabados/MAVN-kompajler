/* Autor: Mihal Sabadoš Datum: 30.05.2021. */
#pragma once
#include "IR.h"

#include <vector>
#include <stack>


//builds interference graph and interference matrix
InterferenceGraph* buildInterferenceGraph(Instructions& instructions);

//build simplification stack
std::stack<Variable*>* doSimplification(InterferenceGraph* ig);

//do resource allocation from simplification list and interference graph
bool doResourceAllocation(std::stack<Variable*>* simplificationStack, InterferenceGraph* ig);

//prints interference matrix ig
void printInterferenceMatrix(InterferenceGraph ig);
