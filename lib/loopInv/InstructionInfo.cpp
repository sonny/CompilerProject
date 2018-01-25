#include "InstructionInfo.h"

int InstructionInfo::count = 1;

void InstructionInfo::build(Function &F) {
  for (Function::iterator BB = F.begin(), BE = F.end(); BB != BE; ++BB) {
    for (BasicBlock::iterator IB = BB->begin(), IE = BB->end(); IB != IE; ++IB) {
      idMap.insert(std::make_pair(IB, count));
      blockMap.insert(std::make_pair(count, BB));
      count++;
    }
  }
}
