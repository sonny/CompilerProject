#ifndef __DOMINANCE__
#define __DOMINANCE__

#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/Analysis/CFG.h"


using namespace llvm;

class Dominance {
private:
  Function *F;
  DenseMap<BasicBlock*, unsigned> idMap;
  DenseMap<unsigned, BasicBlock*> idRMap;
  DenseMap<BasicBlock*, BitVector> In;
  DenseMap<BasicBlock*, BitVector> Out;
  
  void buildIDMap(void);
  void displayNode(BasicBlock *BB);
  void displayBBList(BitVector &bits);
public:
  Dominance(Function *F) : F(F) {} 
  void build(void);
  bool dominates(BasicBlock *A, BasicBlock *B);
  void display(void);
};

#endif /* __DOMINANCE__ */
