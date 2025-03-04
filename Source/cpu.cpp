#include "cpu.h"
#include <cstdint>

class CPU
{
public:
	CPU();
	uint8_t A{}; // Accumulator
	uint8_t B{};
	uint8_t C{};
	uint8_t D{};
	uint8_t E{};
	uint8_t F{}; // Flags
	uint8_t H{};
	uint8_t L{};
	uint8_t IR{}; // Instruction Register
	uint8_t IE{}; // Interrupt Enable
	uint8_t ALU{}; // Arithmetic Logic Unit
	uint16_t IDU{}; // Increment/Decrement Unit
	uint16_t pc{}; // Program counter
	uint16_t sp{}; // Stack pointer
	int cycle {};
	uint8_t memory[MEMORY_SIZE]{}; // placeholder, for logic and stuff

    // these two are placeholders until I figure out the cart stuff
 void setItem(int i, int value){

 }

 uint16_t getItem(int i){
    return memory[i];
 }

};