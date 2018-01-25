//===--------------- loopInv.cpp - Project 1 for CS 701 ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file is a skeleton of an implementation for the loopInv
// pass of Univ. Wisconsin-Madison's CS 701 Project 1.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "loopInv"
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Value.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/BitVector.h"
#include <iostream>
#include <vector>

#include "defs.h"
#include "Dominance.h"
#include "LoopFind.h"
#include "InstructionInfo.h"
#include "LoopInvCM.h"

using namespace llvm;

namespace {
  class loopInv : public FunctionPass {

  private:
    //    static int instCount;
    //    DenseMap<Value*, unsigned> instIDMap;
    //    DenseMap<unsigned, BasicBlock*> instBlockMap;
    InstructionInfo II;
  public:
    static char ID; // Pass identification, replacement for typeid
    loopInv() : FunctionPass(ID) {}

    //**********************************************************************
    // runOnFunction
    //**********************************************************************
    virtual bool runOnFunction(Function &F) {
      bool changed = false;
      II.build(F);

      /* dominance */
      Dominance DOM(&F);
      DOM.build();

      if (PRINT_DOM)
	DOM.display();

      /* loops */
      LoopFind LF(F, DOM);
      LF.find();
      LF.merge();

      if (PRINT_LOOPS)
	LF.display();

      /* Loop Invariant Code Motion */
      for (LoopFind::iterator LI = LF.begin(); LI != LF.end(); ++LI) {
        const NaturalLoop &L = *LI;
        LoopInvCM CM(II, L);
        if (CM.moveAll())
          changed = true;
      }

      return changed; 
    }

    //**********************************************************************
    // print (do not change this method)
    //
    // If this pass is run with -f -analyze, this method will be called
    // after each call to runOnFunction.
    //**********************************************************************
    virtual void print(std::ostream &O, const Module *M) const {
      O << "This is loopInv.\n";
    }

    //**********************************************************************
    // getAnalysisUsage
    //**********************************************************************
    
    // We don't modify the program, so we preserve all analyses
    virtual void getAnalysisUsage(AnalysisUsage &AU) const {
      AU.setPreservesAll();
    };
  };

    
  char loopInv::ID = 0;
  //int loopInv::instCount = 1;

  // register the loopInv class:
  //  - give it a command-line argument (loopInv)
  //  - a name ("optional loads")
  //  - a flag saying that we don't modify the CFG
  //  - a flag saying this is not an analysis pass
  RegisterPass<loopInv> X("loopInv", "Live var analysis",
                          false, true);

}

