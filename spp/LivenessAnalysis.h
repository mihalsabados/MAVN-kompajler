/* Autor: Mihal Sabadoš Datum: 30.05.2021. */
#pragma once
#include "Types.h"
#include "IR.h"
#include "vector"

//do liveness analysis
void livenessAnalysis(Instructions& instrucions);

//checks if variable exists in variable list
bool variableExists(Variable* variable, Variables variables);