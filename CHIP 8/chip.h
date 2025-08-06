

#ifndef CHIP_H
#define CHIP_H

#include<stdint.h>
#include<random>
#include<iostream>

constexpr int makVIDEO_WIDTH = 64;
constexpr int VIDEO_HEIGHT = 32;

class Chip8
{
	
	
		
    public:
	
		uint8_t V[16]{};                //registors
		uint16_t stack[16]{};            
		uint8_t memory[4096]{};			//4096 bytes
		uint16_t pc{};            		//program counter
		uint16_t index{};
		uint8_t sp{};					//stack pointer
		uint8_t delay_timer{};
		uint8_t sound_timer{};
		uint16_t opcode;
		using Chip8Func = void (Chip8::*)(); 	//function pointer member
		Chip8Func table[0x10];       		// For opcodes 0x0 to 0xF
        Chip8Func table0[0xE + 1];      	 	// For opcodes 0x00E0, 0x00EE
        Chip8Func table8[0xE + 1];       		// For opcodes 0x8XY0 to 0x8XYE
        Chip8Func tableE[0xE + 1];       		// For opcodes EX9E and EXA1
        Chip8Func tableF[0x65 + 1];             // For opcodes Fx07 to Fx65
		std::default_random_engine randGen;
        std::uniform_int_distribution<uint8_t> randByte;
	    uint8_t keys[16]{};
        uint32_t display[64 * 32]{};
        Chip8(); 							//constructor
        ~Chip8();							//destructor
        void loadrom(char const*);			//loading game rom into emulator
		void machine_cycle();
		void OP_00E0();                     //1st  instruction
		void OP_00EE();                     //2
		void OP_1nnn();                     //3
		void OP_2nnn();                     //4
		void OP_3xkk();                     //5
		void OP_4xkk();                     //6
		void OP_5xy0();                     //7
		void OP_6xkk();                     //8
		void OP_7xkk();                     //9
		void OP_8xy0();                     //10
		void OP_8xy1();                     //11
		void OP_8xy2();                     //12
		void OP_8xy3();                     //13
		void OP_8xy4();                     //14
		void OP_8xy5();                     //15
		void OP_8xy6();                     //16
		void OP_8xy7();                     //17
		void OP_8xyE();                     //18
		void OP_9xy0();                     //19
		void OP_Annn();                     //20
		void OP_Bnnn();                     //21
		void OP_Cxkk();                     //22
		void OP_Dxyn();                     //23
		void OP_Ex9E();                     //24
		void OP_ExA1();                     //25
		void OP_Fx07();                     //26
		void OP_Fx0A();                     //27
		void OP_Fx15();                     //28
		void OP_Fx18();                     //29
		void OP_Fx1E();                     //30
		void OP_Fx29();                     //31
		void OP_Fx33();                     //32
		void OP_Fx55();                     //33
		void OP_Fx65();                     //34  " "
		void Table0();
		void Table8();
		void TableE();
		void TableF();
		void OP_NULL();
	

};



#endif