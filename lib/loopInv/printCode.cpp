//===--------------- printCode.cpp - Project 1 for CS 701 ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file is a skeleton of an implementation for the printCode
// pass of Univ. Wisconsin-Madison's CS 701 Project 1.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "printCode"
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/ADT/DenseMap.h"
#include <iostream>
#include <vector>
using namespace llvm;

namespace {
    class printCode : public FunctionPass {
    private:
      static int instCount;
      DenseMap<Instruction*, unsigned> instMap;

    public:
        static char ID; // Pass identification, replacement for typeid
        printCode() : FunctionPass(ID) {}

        //**********************************************************************
        // runOnFunction
        //**********************************************************************
        virtual bool runOnFunction(Function &F) {

	  // Build instMap
	  for (Function::iterator BB = F.begin(), BE = F.end(); BB != BE; ++BB) {
	    for (BasicBlock::iterator IB = BB->begin(), IE = BB->end(); IB != IE; ++IB) {
	      instMap.insert(std::make_pair(IB, instCount++));
	    }
	  }
	  
	  // print fn name
	  std::cerr << "FUNCTION " << F.getName().str() << "\n";

	  // Iterate over function...enumerate basic blocks
	  for (Function::iterator BB = F.begin(), BE = F.end(); BB != BE; ++BB) {
	    std::cerr << "\nBASIC BLOCK " << BB->getName().str() << "\n";

	    // Iterate over Basic Block...enumerate all instructions
	    for (BasicBlock::iterator IB = BB->begin(), IE = BB->end(); IB != IE; ++IB) {
	      std::cerr << "%" << instMap[IB] << ":\t" << IB->getOpcodeName() << "\t";


	      for (User::op_iterator UB = IB->op_begin(), UE = IB->op_end(); UB != UE; ++UB) {
		// if the operand is an instruction...look it up in our map
		bool found_operand = false;
		if (Instruction *I = dyn_cast<Instruction>(UB->get())) {
		  if (instMap.count(I)) {
		    unsigned instID = instMap[I];
		    std::cerr << " %" << instID;
		    found_operand = true;
		  }
		}

		if (!found_operand) {
		  if (UB->get()->hasName())
		    std::cerr << " " << UB->get()->getName().str();
		  else
		    std::cerr << " XXX";
		}
	      }

	      std::cerr << "\n";

	    }
	  }
	  return false;  // because we have NOT changed this function
        }

        //**********************************************************************
        // print (do not change this method)
        //
        // If this pass is run with -f -analyze, this method will be called
        // after each call to runOnFunction.
        //**********************************************************************
        virtual void print(std::ostream &O, const Module *M) const {
            O << "This is printCode.\n";
        }

        //**********************************************************************
        // getAnalysisUsage
        //**********************************************************************

        // We don't modify the program, so we preserve all analyses
        virtual void getAnalysisUsage(AnalysisUsage &AU) const {
            AU.setPreservesAll();
        };

    };
    
    char printCode::ID = 0;
    int printCode::instCount = 1;

    // register the printCode class:
    //  - give it a command-line argument (printCode)
    //  - a name ("print code")
    //  - a flag saying that we don't modify the CFG
    //  - a flag saying this is not an analysis pass
    RegisterPass<printCode> X("printCode", "print code",
                              true, false);
}

