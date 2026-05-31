#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Plugins/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
struct NumericalPass : public PassInfoMixin<NumericalPass> {
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM) {
	F.removeFnAttr(Attribute::OptimizeNone);
    	F.removeFnAttr(Attribute::NoInline);
        LLVMContext &Ctx = F.getContext();
        Module *M = F.getParent();

        // Declare __numsan_check(float, double, int) in the IR
        // This tells LLVM "this function exists, it'll be linked in later"
        FunctionCallee CheckFn = M->getOrInsertFunction(
            "__numsan_check",
            Type::getVoidTy(Ctx),      // return type: void
            Type::getFloatTy(Ctx),     // arg 1: float (actual result)
            Type::getDoubleTy(Ctx),    // arg 2: double (shadow result)
            Type::getInt32Ty(Ctx)      // arg 3: source line number
        );

        // Collect instructions first - never modify a list while iterating it
        SmallVector<Instruction*, 16> toInstrument;
        for (auto &BB : F)
            for (auto &I : BB)
                if (I.getOpcode() == Instruction::FAdd ||
                    I.getOpcode() == Instruction::FSub ||
                    I.getOpcode() == Instruction::FMul ||
                    I.getOpcode() == Instruction::FDiv)
                    if (I.getType()->isFloatTy())  // only float, not double
                        toInstrument.push_back(&I);

        for (Instruction *I : toInstrument) {
            // Insert new instructions AFTER the float op
            IRBuilder<> Builder(I->getNextNode());

            Value *a = I->getOperand(0);  // left operand of the float op
            Value *b = I->getOperand(1);  // right operand

            // Step 1: Cast both float operands to double (fpext)
            Value *a_shadow = Builder.CreateFPExt(a, Type::getDoubleTy(Ctx), "a_shadow");
            Value *b_shadow = Builder.CreateFPExt(b, Type::getDoubleTy(Ctx), "b_shadow");

            // Step 2: Redo the operation in double precision
            Value *res_shadow = nullptr;
            switch (I->getOpcode()) {
                case Instruction::FAdd:
                    res_shadow = Builder.CreateFAdd(a_shadow, b_shadow, "res_shadow");
                    break;
                case Instruction::FSub:
                    res_shadow = Builder.CreateFSub(a_shadow, b_shadow, "res_shadow");
                    break;
                case Instruction::FMul:
                    res_shadow = Builder.CreateFMul(a_shadow, b_shadow, "res_shadow");
                    break;
                case Instruction::FDiv:
                    res_shadow = Builder.CreateFDiv(a_shadow, b_shadow, "res_shadow");
                    break;
            }

            // Step 3: Call __numsan_check(float_result, double_shadow, line)
            unsigned line = 0;
            if (DILocation *Loc = I->getDebugLoc()) {
                line = Loc->getLine();
            }
            Value *lineVal = Builder.getInt32(static_cast<int>(line));
            Builder.CreateCall(CheckFn, {I, res_shadow, lineVal});

            errs() << "Instrumented: " << *I << "\n";
        }

        return PreservedAnalyses::none();  // we modified the IR
    }

    static bool isRequired() { return true; }
};
} // namespace

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return {LLVM_PLUGIN_API_VERSION, "NumericalPass", LLVM_VERSION_STRING,
            [](PassBuilder &PB) {
                PB.registerPipelineParsingCallback(
                    [](StringRef Name, FunctionPassManager &FPM,
                       ArrayRef<PassBuilder::PipelineElement>) -> bool {
                        if (Name == "numsan") {
                            FPM.addPass(NumericalPass());
                            return true;
                        }
                        return false;
                    });
                PB.registerOptimizerLastEPCallback(
                    [](ModulePassManager &MPM, OptimizationLevel Level,
                       ThinOrFullLTOPhase Phase) {
                        FunctionPassManager FPM;
                        FPM.addPass(NumericalPass());
                        MPM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));
                    });
            }};
}
