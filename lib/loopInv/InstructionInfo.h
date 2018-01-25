#ifndef __INSTRUCTION_INFO__
#define __INSTRUCTION_INFO__

#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Value.h"
#include "llvm/ADT/DenseMap.h"

using namespace llvm;

class InstructionInfo {
 private:
  static int count;
  DenseMap<Value*, unsigned> idMap;
  DenseMap<unsigned, BasicBlock*> blockMap;

 public:

  // InstructionInfo() : F(F) { }

  void build(Function &F);
  unsigned getID(Value *v) { return idMap[v]; }
  BasicBlock* getBasicBlock(unsigned id) { return blockMap[id]; }
  BasicBlock* getBasicBlock(Value *v) { return blockMap[idMap[v]]; }
  void update(Value *V, BasicBlock *B) { blockMap[idMap[V]] = B; }
};

#endif /* __INSTRUCTION_INFO__ */
