#include "CPU.h"

CPU::CPU()
{
    memory = Memory();
    registry = Registry();
    opCodeTable = OpCodeTable();
    initMemory();
    initInstructionSet();
    controller = Controller();
    stack.emplace(0x1234);

    // Initialise le tableau de keys à 0
    for (auto &i : keys) 
        i = 0x00;
        
    //TODO Initialiser le tableau de pointeur d'instructions
}

CPU::~CPU()
{
    std::cout << "BYE BYE \n";
}

void CPU::initMemory()
{
    memory.copyCartridge();
}

void CPU::initInstructionSet()
{
      instructionSet =
          {
        &InstructionProcessor::instr0NNN,
        &InstructionProcessor::instr00E0,
        &InstructionProcessor::instr00EE,
        &InstructionProcessor::instr1NNN,
        &InstructionProcessor::instr2NNN,
        &InstructionProcessor::instr3XNN,
        &InstructionProcessor::instr4XNN,
        &InstructionProcessor::instr5XY0,
        &InstructionProcessor::instr6XNN,
        &InstructionProcessor::instr7XNN,
        &InstructionProcessor::instr8XY0,
        &InstructionProcessor::instr8XY1,
        &InstructionProcessor::instr8XY2,
        &InstructionProcessor::instr8XY3,
        &InstructionProcessor::instr8XY4,
        &InstructionProcessor::instr8XY5,
        &InstructionProcessor::instr8XY6,
        &InstructionProcessor::instr8XY7,
        &InstructionProcessor::instr8XYE,
        &InstructionProcessor::instr9XY0,
        &InstructionProcessor::instrANNN,
        &InstructionProcessor::instrBNNN,
        &InstructionProcessor::instrCXNN,
        &InstructionProcessor::instrDXYN,
        &InstructionProcessor::instrEX9E,
        &InstructionProcessor::instrEXA1,
        &InstructionProcessor::instrFX07,
        &InstructionProcessor::instrFX0A,
        &InstructionProcessor::instrFX15,
        &InstructionProcessor::instrFX18,
        &InstructionProcessor::instrFX1E,
        &InstructionProcessor::instrFX29,
        &InstructionProcessor::instrFX33,
        &InstructionProcessor::instrFX55,
        &InstructionProcessor::instrFX65

        };


}

// void CPU::clock(Clock & clock) const
// {
//     clock.decreaseCycle();
//     // Clock* clock = Clock::GetInstance();
//     // uint8_t cycleState = clock->getCycleState();
//     // clock->setCycleState(--cycleState);
// }

uint16_t CPU::getNextOpCode() 
{

    std::cout << "get next opcode - pc = " << pc << "\n";
    uint8_t opCodeMsb = memory.read(pc);
    pc++;
    std::cout << "get next opcode - pc = " << pc << "\n";
    uint8_t opCodeLsb = memory.read(pc);
    pc++;
    std::cout << "get next opcode - pc = " << pc << "\n";

    uint16_t opCode = addBytes(opCodeMsb, opCodeLsb);


    // std::cout << "opcode : " << std::hex << opCode << "\n";

    EnumInstruction instruction = opCodeTable.getInstruction(opCode);

    // std::cout << "instruciton : " << instruction << "\n";
    return opCode;

}


uint16_t CPU::addBytes(uint8_t const &msb, uint8_t const &lsb) 
{
    return byteShiftLeft(msb) + lsb;
}

uint16_t CPU::byteShiftLeft(uint8_t const &data) 
{
    uint16_t res = data<<8;
    return res;
}

void CPU::processInstruction(uint16_t opcode, EnumInstruction instruction) 
{
    // *instructionSet[1] : fonction qui est dans instructionProcessor
    // (instructionProcessor.*instructionSet[instruction])(opcode);
    (instructionSet[instruction])(*this, opcode);

}

void CPU::process() 
{
    std::cout << "process \n";
    uint16_t opcode = getNextOpCode();

    std::cout << "opcode = " << opcode << "\n";
    std::cout << "Process - opcode : " << std::hex << opcode+0 << "\n";

    EnumInstruction instruction = opCodeTable.getInstruction(opcode);  

    processInstruction(opcode, instruction);  

}

void CPU::updateKeys()
{
    
    controller.updateInputState(keys);
    
}

uint8_t CPU::waitForKeys( uint8_t XRegistry)
{
    controller.waitForInput(registry, keys, XRegistry);
}

// void CPU::handleClockCycles(Clock & clock)
// {
//     while (clock.getCycle() > 0) 
//     {
//         Sleep(4);
//         clock.decreaseCycle();
//     }

//     std::cout << " Fin du cycle d'horloge, declenchement de l'instruction suivante" << "\n";
//     process();
// }