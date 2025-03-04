#include <cstdint>
#include "cpu.cpp"

class Opcodes{
    private:
    public:
        
        // 0x00, just adds 1 to pc, cycles: 1, bytes: 3
        void NOP(CPU cpu){ 
            cpu.pc+=1;
            cpu.cycle+=1;
        }

        // 0x01, loads 2 bytes into register  pair BC, c: 3, b: 3
        void LD_BC_d16(CPU cpu, uint16_t value){
            uint16_t lsb = value >> 8;
            uint16_t msb = value & 0x00FF;
            cpu.B = lsb;
            cpu.C = msb;
            cpu.pc+=3;
            cpu.cycle+=3;
        }

        // 0x02, stores contents of A in location specified by BC, c: 2, b: 1
        void LD_BC_A(CPU cpu){
            cpu.setItem((cpu.B << 8) + cpu.C, cpu.A);
            cpu.pc+=1;
            cpu.cycle+=2;
        }

        // 0x03, increments contents of BC by 1, c:2, b: 1
        void INC_BC(CPU cpu){
            uint16_t BC_value = (cpu.B << 8) + cpu.C;
            BC_value+=1;
            cpu.B = BC_value >> 8;
            cpu.C = BC_value & 0x00FF;
            cpu.pc+=1;
            cpu.cycle+=2;
        }

        // 0x04, adds 1 to B, c:1, b:1
        void INC_B(CPU cpu){
            cpu.B+=1;
            cpu.pc+=1;
            cpu.cycle+=1;
        }

        // 0x05, subtracts 1 from B, c:1, b:1
        void DEC_B(CPU cpu){
            cpu.B-=1;
            cpu.pc+=1;
            cpu.cycle+=1;
        }

        // 0x06, load 8 bit immediate opperand d8 into B, c: 2, b: 2
        void LD_B_d8(CPU cpu, uint8_t value){
            cpu.B = value;
            cpu.pc+=2;
            cpu.cycle+=2;
        }

        // 0x07, Rotates contents of A to the left, contents of bit 7 are placed in CY flag and bit 0 of A, c: 1, b:1
        void RLCA(CPU cpu){
            
        }


        void execute(CPU cpu, uint16_t opcode){
            int value = 0;
            uint16_t pc = cpu.pc;
        }


};