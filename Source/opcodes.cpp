#include <cstdint>
#include "cpu.cpp"

class Opcodes{
    private:
    public:
        int flag_c = 4; // carry flag, 4th bit in A
        int flag_h = 5; // half carry flag, 5th bit in A
        int flag_n = 6; // subtraction flag, 6th bit in A
        int flag_z = 7; // zero flag, 7th bit in A

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
            uint8_t flag = 0b00000000;
            if(cpu.B == 0)
                flag|= (1 << flag_z);
            if(cpu.B & (1 << 3))
                flag|= (1 << flag_h);
            cpu.F = flag;
            cpu.pc+=1;
            cpu.cycle+=1;
        }

        // 0x05, subtracts 1 from B, do logic for flags z, h, and n, c:1, b:1
        void DEC_B(CPU cpu){
            cpu.B-=1;
            uint8_t flag = 0b00000000;
            if(cpu.B == 0)
                flag|= (1 << flag_z);
            if(cpu.B & (1 << 3))
                flag|= (1 << flag_h);
            flag|= (1 << flag_n);
            cpu.F = flag;
            cpu.pc+=1;
            cpu.cycle+=1;
        }

        // 0x06, load 8 bit immediate opperand d8 into B, c: 2, b: 2
        void LD_B_d8(CPU cpu, uint8_t value){
            cpu.B = value;
            cpu.pc+=2;
            cpu.cycle+=2;
        }

        // 0x07, Rotates contents of A to the left, contents of bit 7 are placed in flag_c and bit 0 of A, c: 1, b:1
        void RLCA(CPU cpu){
            uint8_t shift = (cpu.A << 1) + (cpu.A >> 7);
            uint8_t flag = 0b00000000;
            flag = ((cpu.A >> 7) << flag_c) | flag;
            cpu.A = shift;
            cpu.F = flag;
            cpu.pc+=1;
            cpu.cycle+=1;
        }

        // 0x08, store the lower byte of sp at the address specified by the immediate 16 bit operand a16, and store the pper byte of sp at the address a16 + 1. c: 5, b: 3
        void LD_a16_sp(CPU cpu,uint16_t value){
            cpu.setItem(value, cpu.sp & 0xFF);
            cpu.setItem(value + 1, cpu.sp >> 8);
            cpu.pc+=3;
            cpu.cycle+=5;
        }

        // 0x09, add contents of BC to the contents of HL and store it in HL,set flag_h and flag_c based on their logic, c: 2, b: 1
        void ADD_HL_BC(CPU cpu){
            uint16_t BC_value = (cpu.B << 8) + cpu.C;
            uint16_t HL_value = (cpu.H << 8) + cpu.L;
            int sum = BC_value + HL_value;
            uint16_t sum_16b = BC_value + HL_value;
            uint16_t lsb = sum_16b >> 8;
            uint16_t msb = sum_16b & 0x00FF;
            uint8_t flag = 0b00000000;
            // check bits 11 and 15 in sum for flags
            if(sum & (1 << 11))
                flag|= (1 << flag_h);
            else
                // invert flag then use and to set bit n to 0
                flag|= flag & ~(1 << flag_h);
            if(sum & (1 << 15))
                flag|= (1 << flag_c);
            else
                flag|= flag & ~(1 << flag_c);
            cpu.F = flag;
            cpu.H = lsb;
            cpu.L = msb;
            cpu.pc+=1;
            cpu.cycle+=2;
        }

        // 0x0A, load memory[BC] into A, c: 2, b: 1
        void LD_A_BC(CPU cpu){
            uint16_t BC_value = (cpu.B << 8) + cpu.C;
            uint8_t value_from_memory = cpu.getItem(BC_value);
            cpu.A = value_from_memory;
            cpu.pc+=1;
            cpu.cycle+=2;
        }

        // 0x0B, decrement BC by 1, c: 2, b: 1
        void DEC_BC(CPU cpu){
            uint16_t BC_value = (cpu.B << 8) + cpu.C;
            BC_value-=1;
            uint16_t lsb = BC_value >> 8;
            uint16_t msb = BC_value & 0x00FF;
            cpu.B = lsb;
            cpu.C = msb;
            cpu.pc+=1;
            cpu.cycle+=2;
        }

        // 0x0C, increase C by 1, set flag z to 1 if the result is 0 and set flag h to 1 if bit 3 of the result is 1, c: 1, b: 1
        void INC_C(CPU cpu){
            cpu.C+=1;
            uint8_t flag = 0b00000000;
            if(cpu.C == 0)
                flag|= (1 << flag_z);
            if(cpu.C & (1 << 3))
                flag|= (1 << flag_h);
            cpu.F = flag;
            cpu.pc+=1;
            cpu.cycle+=1;
        }

        // 0x0D, decrement c by 1, do the flag stuff, c: 1, b: 1
        void DEC_C(CPU cpu){
            cpu.C-=1;
            uint8_t flag = 0b00000000;
            if(cpu.C == 0)
                flag|= (1 << flag_z);
            if(cpu.C & (1 << 3))
                flag|= (1 << flag_h);
            flag|= (1 << flag_n);
            cpu.F = flag;
            cpu.pc+=1;
            cpu.cycle+=1;
        }

        // 0x0E, load 8 bit immediate opperand d8 int C, c: 2, b: 2
        void LD_C_d8(CPU cpu, uint8_t value){
            cpu.C = value;
            cpu.pc+=2;
            cpu.cycle+=2;
        }

        // 0x0F, Rotate A to the right, contents of bit 7 are placed in flag_c and bit 0 of A, c: 1, b:1
        void RRCA(CPU cpu){
            uint8_t shift = (cpu.A > 1) + (cpu.A << 7);
            uint8_t flag = 0b00000000;
            flag = ((cpu.A >> 7) << flag_c) | flag;
            cpu.A = shift;
            cpu.F = flag;
            cpu.pc+=1;
            cpu.cycle+=1;
        }

        // 0x10, STOP instruction, stops system clock and oscillator circuit, RAM remains unchanged, c: 1, b: 2
        void STOP(CPU cpu){
            // do the logic for this when system clock and oscillator are implemented
            cpu.pc+=2;
            cpu.cycle+=1;
        }

        // 0x11, loads 2 bytes into register  pair DE, c: 3, b: 3
        void LD_DE_d16(CPU cpu, uint16_t value){
            uint16_t lsb = value >> 8;
            uint16_t msb = value & 0x00FF;
            cpu.D = lsb;
            cpu.E = msb;
            cpu.pc+=3;
            cpu.cycle+=3;
        }

        // 0x12, stores contents of A in location specified by DE, c: 2, b: 1
        void LD_DE_A(CPU cpu){
            cpu.setItem((cpu.D << 8) + cpu.E, cpu.A);
            cpu.pc+=1;
            cpu.cycle+=2;
        }

        // 0x13, increments contents of DE by 1, c:2, b: 1
        void INC_DE(CPU cpu){
            uint16_t DE_value = (cpu.D << 8) + cpu.E;
            DE_value+=1;
            cpu.D = DE_value >> 8;
            cpu.E = DE_value & 0x00FF;
            cpu.pc+=1;
            cpu.cycle+=2;
        }

        // 0x14, adds 1 to D, c:1, b:1
        void INC_D(CPU cpu){
            cpu.D+=1;
            uint8_t flag = 0b00000000;
            if(cpu.D == 0)
                flag|= (1 << flag_z);
            if(cpu.D & (1 << 3))
                flag|= (1 << flag_h);
            cpu.F = flag;
            cpu.pc+=1;
            cpu.cycle+=1;
        }

        // 0x15, subtracts 1 from D, do logic for flags z, h, and n, c:1, b:1
        void DEC_D(CPU cpu){
            cpu.D-=1;
            uint8_t flag = 0b00000000;
            if(cpu.D == 0)
                flag|= (1 << flag_z);
            if(cpu.D & (1 << 3))
                flag|= (1 << flag_h);
            flag|= (1 << flag_n);
            cpu.F = flag;
            cpu.pc+=1;
            cpu.cycle+=1;
        }

        // 0x16, load 8 bit immediate opperand d8 into D, c: 2, b: 2
        void LD_D_d8(CPU cpu, uint8_t value){
            cpu.B = value;
            cpu.pc+=2;
            cpu.cycle+=2;
        }

        // 0x17, shift to the left, value in carry flag is put at bit 0 c: 1, b: 1
        void RLA(CPU cpu){
            uint8_t shift = (cpu.A << 1);
            uint8_t flag = 0b00000000;
            shift = shift + (cpu.F << flag_c) >> 7;
            cpu.A = shift;
            cpu.F = flag;
            cpu.pc+=1;
            cpu.cycle+=1;
        }

        // 0x18, unconditional jump in memory s8 steps from current pc, c: 3, b: 2
        void JR_s8(CPU cpu, uint8_t value){
            cpu.pc+= 2 + ((value ^ 0x80) - 0x80);
            cpu.cycle+=3;
        }

        // 0x19, add contents of DE to the contents of HL and store it in HL,set flag_h and flag_c based on their logic, c: 2, b: 1
        void ADD_HL_DE(CPU cpu){
            uint16_t DE_value = (cpu.D << 8) + cpu.E;
            uint16_t HL_value = (cpu.H << 8) + cpu.L;
            int sum = DE_value + HL_value;
            uint16_t sum_16b = DE_value + HL_value;
            uint16_t lsb = sum_16b >> 8;
            uint16_t msb = sum_16b & 0x00FF;
            uint8_t flag = 0b00000000;
            // check bits 11 and 15 in sum for flags
            if(sum & (1 << 11))
                flag|= (1 << flag_h);
            else
                // invert flag then use and to set bit n to 0
                flag|= flag & ~(1 << flag_h);
            if(sum & (1 << 15))
                flag|= (1 << flag_c);
            else
                flag|= flag & ~(1 << flag_c);
            cpu.F = flag;
            cpu.H = lsb;
            cpu.L = msb;
            cpu.pc+=1;
            cpu.cycle+=2;
        }

         // 0x1A, load memory[DE] into A, c: 2, b: 1
         void LD_A_DE(CPU cpu){
            uint16_t DE_value = (cpu.D << 8) + cpu.E;
            uint8_t value_from_memory = cpu.getItem(DE_value);
            cpu.A = value_from_memory;
            cpu.pc+=1;
            cpu.cycle+=2;
        }

        // 0x1B, decrement DE by 1, c: 2, b: 1
        void DEC_BC(CPU cpu){
            uint16_t DE_value = (cpu.D << 8) + cpu.E;
            DE_value-=1;
            uint16_t lsb = DE_value >> 8;
            uint16_t msb = DE_value & 0x00FF;
            cpu.B = lsb;
            cpu.C = msb;
            cpu.pc+=1;
            cpu.cycle+=2;
        }

        // 0x1C, increase E by 1, set flag z to 1 if the result is 0 and set flag h to 1 if bit 3 of the result is 1, c: 1, b: 1
        void INC_E(CPU cpu){
            cpu.E+=1;
            uint8_t flag = 0b00000000;
            if(cpu.E == 0)
                flag|= (1 << flag_z);
            if(cpu.E & (1 << 3))
                flag|= (1 << flag_h);
            cpu.F = flag;
            cpu.pc+=1;
            cpu.cycle+=1;
        }

        // 0x1D, decrement E by 1, do the flag stuff, c: 1, b: 1
        void DEC_E(CPU cpu){
            cpu.E-=1;
            uint8_t flag = 0b00000000;
            if(cpu.E == 0)
                flag|= (1 << flag_z);
            if(cpu.E & (1 << 3))
                flag|= (1 << flag_h);
            flag|= (1 << flag_n);
            cpu.F = flag;
            cpu.pc+=1;
            cpu.cycle+=1;
        }

        // 0x1E, load 8 bit immediate opperand d8 int E, c: 2, b: 2
        void LD_E_d8(CPU cpu, uint8_t value){
            cpu.E = value;
            cpu.pc+=2;
            cpu.cycle+=2;
        }

        // 0x1F, rotates A to the right, value in carry flag is put at bit 7 c: 1, b: 1
        void RRA(CPU cpu){
            uint8_t shift = (cpu.A >> 1);
            uint8_t flag = 0b00000000;
            shift = shift + (cpu.F << flag_c);
            cpu.A = shift;
            cpu.F = flag;
            cpu.pc+=1;
            cpu.cycle+=1;
        }

        void execute(CPU cpu, uint16_t opcode){
            int value = 0;
            uint16_t pc = cpu.pc;
        }


};