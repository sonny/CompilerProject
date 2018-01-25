#include "LoopInvCM.h"

bool LoopInvCM::isDefinedInLoop(Value *V) {
  BasicBlock *BB = II.getBasicBlock(V);
  return L.isMember(BB);
}

bool LoopInvCM::isInvariant(Instruction *I) {
  // are all of the operands either literals or 
  // defined outside of the loop
  bool result = true;
  for (User::const_op_iterator O = I->op_begin(), E = I->op_end(); O != E; ++O) {
    Value *V = O->get();
    if (!isa<Constant>(V) && isDefinedInLoop(V))
      result = false;
  }
  return result;
}

bool LoopInvCM::isSafe(Instruction *I) {
  bool result = true;

  if (I->getOpcode() == Instruction::UDiv ||
      I->getOpcode() == Instruction::SDiv ||
      I->getOpcode() == Instruction::FDiv ||
      I->getOpcode() == Instruction::URem ||
      I->getOpcode() == Instruction::SRem ||
      I->getOpcode() == Instruction::FRem ||
      /* Memory Instructions */
      I->getOpcode() == Instruction::Alloca ||
      I->getOpcode() == Instruction::Load ||
      I->getOpcode() == Instruction::Store ||
      I->getOpcode() == Instruction::GetElementPtr ||
      I->getOpcode() == Instruction::Fence ||
      I->getOpcode() == Instruction::AtomicCmpXchg ||
      I->getOpcode() == Instruction::AtomicRMW ||
      /* end Memory Instructions */
      I->getOpcode() == Instruction::Call ||
      I->getOpcode() == Instruction::Invoke ||
      I->getOpcode() == Instruction::PHI ||
      /* other unsafe conditions */
      I->isTerminator() )
    result = false;

  return result;
}

BasicBlock *LoopInvCM::getPreHeader(void) {
  BasicBlock *head = L.getHead();
  BasicBlock *result = head->getSinglePredecessor();
  // if result is null, then we need to build a pre-header
  if (!result) {
    SmallVector<BasicBlock*,8> preds;
    for (pred_iterator PI = pred_begin(head), PE = pred_end(head); PI != PE; ++PI) {
      BasicBlock *P = *PI;
      preds.push_back(P);
    }
    result = SplitBlockPredecessors(head, preds, ".preheader");

    if (PRINT_PRE)
      std::cerr << "adding preheader for loop with header " << head->getName().str() << std::endl;
  }
  return result;
}

void LoopInvCM::move(Instruction *I) {
  if (PRINT_MOVING)
    std::cerr << "moving instruction %" << II.getID(I) << std::endl;

  BasicBlock *preheader = getPreHeader();
  I->moveBefore(preheader->getTerminator());
  II.update(I, preheader);
}

bool LoopInvCM::moveAll(void) {
  bool changed;
  bool result = false; // assume nothing changed
  do {
    changed = false;
    for (NaturalLoop::iterator BI = L.begin(); BI != L.end(); ++BI) {
      BasicBlock *BB = *BI;
      for (BasicBlock::iterator IT = BB->begin(); IT != BB->end(); /* increment in body */) {
        Instruction *I = IT;
        IT++; // increment iterator before possible move
        unsigned id = II.getID(I);
        if (isInvariant(I) && isSafe(I)) {
          move(I);
          changed = true;
          result = true;
        }
      }
    }
  } while(changed);
  return result;
}
