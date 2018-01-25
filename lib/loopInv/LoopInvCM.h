#ifndef __LOOP_INV_CM__
#define __LOOP_INV_CM__

#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"

#include "InstructionInfo.h"
#include "LoopFind.h"
#include "defs.h"

using namespace llvm;

class LoopInvCM {
 private:
  InstructionInfo &II;
  const NaturalLoop &L;

  bool isDefinedInLoop(Value *V);
  bool isInvariant(Instruction *I);
  bool isSafe(Instruction *I);

  BasicBlock *getPreHeader(void);
  void move(Instruction *I);
 public:
 LoopInvCM(InstructionInfo &II, const NaturalLoop &L) : II(II), L(L) {}
  bool moveAll(void);
};

#endif /* __LOOP_INV_CM__ */
