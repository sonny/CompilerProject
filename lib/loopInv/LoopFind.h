#ifndef __LOOPFIND__
#define __LOOPFIND__

#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/ADT/SmallVector.h"
#include <iostream>
#include <set>
#include <vector>
#include "Dominance.h"

using namespace llvm;

class NaturalLoop {
 private:
  BasicBlock *head;
  BasicBlock *tail;
  std::set<BasicBlock*> body;
  std::vector<BasicBlock*> stack;

  void insert(BasicBlock *M);
 public:

  typedef std::set<BasicBlock*>::iterator iterator;

 NaturalLoop(BasicBlock *head, BasicBlock *tail) : head(head), tail(tail) {}
  void build(void);
  bool compareHead(const NaturalLoop &other) const { return other.head == head; }
  void merge(const NaturalLoop &other);
  bool isMember(BasicBlock *B) const { return body.count(B) == 1; }
  void display(void) const;
  BasicBlock *getHead(void) const { return head; }
  iterator begin(void) const { return body.begin(); }
  iterator end(void) const { return body.end(); }

  friend bool operator<(const NaturalLoop& lhs, const NaturalLoop& rhs);
};

// how to compare 2 NaturalLoop instances
inline bool operator<(const NaturalLoop& lhs, const NaturalLoop& rhs) {
  if (lhs.body.size() == rhs.body.size()) {
    if (lhs.head == rhs.head) {
      return &lhs < &rhs; // as long as the pointer is different
    }
    else
      return lhs.head->getName().str() < rhs.head->getName().str();
  } else
    return rhs.body.size() < lhs.body.size();
}


class LoopFind {
  Function &F;
  Dominance &DOM;
  std::set<NaturalLoop> loops;
  unsigned mergeCount;
 public:

  typedef std::set<NaturalLoop>::iterator iterator;

 LoopFind(Function &F, Dominance &DOM) : F(F), DOM(DOM), mergeCount(0) {}
  void find(void);
  void merge(void);
  void display(void) const;

  iterator begin(void) { return loops.begin(); }
  iterator end(void) { return loops.end(); }
};

#endif /* __LOOPFIND__ */
