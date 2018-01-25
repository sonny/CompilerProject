#include "defs.h"
#include "LoopFind.h"

void NaturalLoop::insert(BasicBlock *M) {
  if (body.count(M) == 0) {
    body.insert(M);
    stack.push_back(M);
  }
}

void NaturalLoop::build(void) {
  stack.clear();

  body.insert(head);
  insert(tail);
  while (!stack.empty()) {
    BasicBlock *M = stack.back();
    stack.pop_back();
    for (pred_iterator PI = pred_begin(M), PE = pred_end(M); PI != PE; ++PI) {
      BasicBlock *Pred = *PI;
      insert(Pred);
    }
  }
}

void NaturalLoop::merge(const NaturalLoop &other) {
  for (std::set<BasicBlock*>::iterator it = other.body.begin(); it != other.body.end(); ++it) {
    body.insert(*it);
  }
}

void NaturalLoop::display(void) const {
  std::cerr << "head: " << head->getName().str();
  std::cerr << " body: { ";
  std::vector<std::string> bbNames;
  for (std::set<BasicBlock*>::iterator it = body.begin(); it != body.end(); ++it) {
    if ((*it) == head) continue;
    bbNames.push_back(std::string((*it)->getName().str()));
  }

  std::sort(bbNames.begin(), bbNames.end());
  for (std::vector<std::string>::iterator it=bbNames.begin(); it!=bbNames.end(); ++it)
    std::cerr << *it << " ";

  std::cerr << "}\n";
}

void LoopFind::find(void) {
  for (Function::iterator BB = F.begin(), BE = F.end(); BB != BE; ++BB) {
    for (pred_iterator PI = pred_begin(BB), PE = pred_end(BB); PI != PE; ++PI) {
      BasicBlock *pred = *PI;
      // does BB dominate Pred
      if (DOM.dominates(BB, pred)) /* we have a backedge pred -> BB */ {
        NaturalLoop loop(BB, pred);
        loop.build();
        loops.insert(loop);
      }
    }
  }
  // std::vector<BasicBlock*> stack;
  // DenseMap<BasicBlock*,bool> visited;

  // // Start with entry block (not dominated by any node)
  // BasicBlock &entry = F.getEntryBlock();
  // stack.push_back(&entry);
  // visited[&entry] = true;

  // while(!stack.empty()) {
  //   BasicBlock *BB = stack.back();
  //   stack.pop_back();

  //   // visit each successor of current block
  //   for (succ_iterator SI = succ_begin(BB), SE = succ_end(BB); SI != SE; ++SI) {
  //     BasicBlock *succ = *SI;
  //     // does succ dominate BB
  //     if (DOM.dominates(succ, BB)) /* we have a backedge BB -> succ */ {
  //       NaturalLoop loop(succ, BB);
  //       loop.build();
  //       loops.insert(loop);
  //     }
  //     else if (visited.count(succ) == 0) /* not visited yet */ {
  //       stack.push_back(succ);
  //     }

  //     visited[succ] = true; // mark successor as visited
  //   }
  // }
}

void LoopFind::merge(void) {
  DenseMap<BasicBlock*, std::vector<NaturalLoop*> > mergeMap;
  // build merge Map
  for (std::set<NaturalLoop>::iterator A = loops.begin(); A != loops.end(); ++A) {
    mergeMap[(*A).getHead()].push_back(const_cast<NaturalLoop*>(&*A));
  }

  DenseMap<BasicBlock*, std::vector<NaturalLoop*> >::iterator IT = mergeMap.begin();
  for ( ; IT != mergeMap.end(); ++IT) {
    BasicBlock *head = IT->first;
    if (IT->second.size() > 1) /* more than one loop to merge */ {
      int count  = 1;
      std::vector<NaturalLoop*>::iterator A = IT->second.begin();
      NaturalLoop *mergeInto = *A;
      A++; /* skip to the next one */
      for ( ; A != IT->second.end(); ++A) {
	mergeInto->merge(**A);
	loops.erase(**A); /* remove from list of loops after merge */
	count++;
      }

      if (PRINT_MERGE)
	std::cerr << "merging " << count << " loops with header " << head->getName().str() << std::endl;

    }
  }
  
 //  SmallVector<NaturalLoop&,8> toRemove;
 //  bool merge = false;
 // RESTART:
 //  for (std::set<NaturalLoop>::iterator A = loops.begin(); A != loops.end(); ++A) {
 //    int count = 1;
 //    toRemove.clear();
 //    std::set<NaturalLoop>::iterator B = A;
 //    B++;
 //    for ( ; B != loops.end(); ++B) {
 //      if ((*A).compareHead(*B)) {
 //        NaturalLoop *l = const_cast<NaturalLoop*>(&(*A));
 //        l->merge(*B);
 // 	toRemove.push_back(*B);
 //        //loops.erase(*B);
 //        //mergeCount++;
 // 	count++;
 //        //goto RESTART;
 // 	merge = true;
 //      }
 //    }
    
 //    //    for (SmallVector<NaturalLoop*,8>::iterator R = toRemove.begin(); R != toRemove.end(); ++R) {
 //    //      loops.erase(*R);
 //    //    }

 //    if (PRINT_MERGE && merge)
 //      std::cerr << "merging " << count << " loops with header " << (*A)->getName().str() << std::endl;

 //    if (merge) goto RESTART; // we mucked up the iterator
 //  }  
}

void LoopFind::display(void) const {
  std::cerr << "\nFUNCTION " << F.getName().str() << "\nLOOPS\n";
  for (std::set<NaturalLoop>::iterator it = loops.begin(); it != loops.end(); ++it) {
    it->display();
  }
  std::cerr << "\n";
}
