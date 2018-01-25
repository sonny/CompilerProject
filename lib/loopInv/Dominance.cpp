#include "Dominance.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

void Dominance::buildIDMap(void) {
  unsigned id = 0;
  int s = F->getBasicBlockList().size();
  for (Function::iterator BB = F->begin(), BE = F->end(); BB != BE; ++BB) {
    idMap.insert(std::make_pair(BB, id));  // forward map
    idRMap.insert(std::make_pair(id, BB)); // reverse map
    Out[BB].resize(s, true);
    id++;
  }
}
  
void Dominance::build(void) {
  buildIDMap();

  int s = idMap.size();
  bool change = false;
  
  ReversePostOrderTraversal<Function*> RPOT(F);
  do {
    for (ReversePostOrderTraversal<Function*>::rpo_iterator I = RPOT.begin(),
           E = RPOT.end(); I != E; ++I) {
      BasicBlock *BB = *I;
      
      unsigned id = idMap[BB];
      BitVector IN(s, false);

      for (pred_iterator PI = pred_begin(BB), PE = pred_end(BB); PI != PE; ++PI) {
        BasicBlock *Pred = *PI;
        if (PI == pred_begin(BB)) 
          IN = BitVector(Out[Pred]);
        else
          IN &= Out[Pred];
      }

        // Calculate OUT
      BitVector OUT = BitVector(IN);
      OUT.set(id);
      
        if (In[BB] != IN || Out[BB] != OUT)
          change = true;
        
        In[BB] = IN;
        Out[BB] = OUT;
    }
  } while (!change);
}

// returns true if A dominates B
bool Dominance::dominates(BasicBlock *A, BasicBlock *B) {
  unsigned id = idMap[A];
  return Out[B].test(id);
}

void Dominance::displayBBList(BitVector &bits) {
  std::cerr << "{ ";
  
  if (bits.any()) {
    std::vector<std::string> bbNames;
    int idx = -1;
    while ((idx = bits.find_next(idx)) != -1) {
      BasicBlock *BB = idRMap[idx];
      bbNames.push_back(std::string(BB->getName().str()));
    } 

    std::sort(bbNames.begin(), bbNames.end());
    for (std::vector<std::string>::iterator it=bbNames.begin(); it!=bbNames.end(); ++it)
      std::cerr << *it << " ";
  }

  std::cerr << "}";
    
  std::cerr.flush();
}

void Dominance::displayNode(BasicBlock *BB) {
  std::cerr << "\nBASIC BLOCK " << BB->getName().str();
  std::cerr << " DOM-Before: ";
  displayBBList(In[BB]);
  std::cerr << " DOM-After: ";
  displayBBList(Out[BB]);
}

void Dominance::display(void) {
  std::cerr << "\nFUNCTION " << F->getName().str() << "\n";
  for (Function::iterator BB = F->begin(), BE = F->end(); BB != BE; ++BB) {
    displayNode(BB);
  }
  std::cerr << "\n";
}
