/* Autor: Mihal Sabadoš Datum: 30.05.2021. */
#pragma once
#include "IR.h"

//write assembler code into .s file
void writeFile(std::string fileName, Variables vars, FuncLabs funcLabs, Instructions instr);
