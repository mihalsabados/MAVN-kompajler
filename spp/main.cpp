/* Autor: Mihal Sabadoš Datum: 30.05.2021. */
#include <iostream>
#include <exception>

#include "LexicalAnalysis.h"
#include "SyntaxAnalysis.h"
#include "VarInstrBuilder.h"
#include "LivenessAnalysis.h"
#include "ResourceAllocation.h"
#include "File.h"

using namespace std;
    
int main()
{
	try
	{
		std::string fileName = ".\\..\\examples\\simple.mavn";
		bool retVal = false;

		LexicalAnalysis lex;

		if (!lex.readInputFile(fileName))
			throw runtime_error("\nException! Failed to open input file!\n");

		lex.initialize();

		retVal = lex.Do();

		if (retVal)
		{
			cout << "Lexical analysis finished successfully!" << endl;
			lex.printTokens();
		}
		else
		{
			lex.printLexError();
			throw runtime_error("\nException! Lexical analysis failed!\n");
		}

		SyntaxAnalysis syntax(lex);
		
		//excecute syntax analysis
		if (syntax.Do())
		{
			cout << "Syntax analysis finished successfully!" << endl;
		}
		else
		{
			cout << "Syntax analysis failed!" << endl;
		}
		//object for generating and storing variable list, function-label list and instruction list
		VarInstrBuilder varInstr(lex);
		
		//generate variable list and function-label list 
		varInstr.generateVariables();

		//generate instruction list
		varInstr.generateInstructions();

		//add successors and predecessors to instructions
		varInstr.generateSuccPred();

		Instructions instructions = varInstr.getInstructions();

		//do liveness analysis
		livenessAnalysis(instructions);

		InterferenceGraph* ig = buildInterferenceGraph(instructions);

		printInterferenceMatrix(*ig);

		stack<Variable*>* simplificationStack = doSimplification(ig);

		if (doResourceAllocation(simplificationStack, ig))
		{
			cout << "Successful Resource Allocation!!!" << endl;
		}
		else
		{
			cout << "Failed Allocation!!!" << endl;
		}

		//writes to output file
		string outFile = ".\\..\\examples\\resultSimple.s";
		writeFile(outFile, varInstr.getVariables(), varInstr.getFuncLabs(), instructions);

		//free memory
		delete simplificationStack;
		delete ig;
	}
	catch (string error)
	{
		cout << "============== ERROR ==============" << endl;
		cout << error << endl;
		return 1;
	}
	catch (runtime_error e)
	{
		cout << e.what() << endl;
		return 1;
	}

	return 0;
}
