

#include "chip.h"
#include <iomanip>
#include <fstream>
#include<chrono>
#include <random>
#include <memory.h>
using namespace std;




const unsigned int start_address = 0x200;   //Instructions from the ROM will be stored starting at 0x200
const unsigned int FONTSET_SIZE = 80;    
const unsigned int fontset_address = 0x50;  //characters 0-F will be stored from 0x50-0x0A0


uint8_t fontset[FONTSET_SIZE] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::~Chip8()
{
	
}

void Chip8::loadrom(char const* filename)
{
	ifstream file(filename,ios::binary|ios::ate);   			//opening file in binary and moving get pointer to end.
   
	if(file.is_open())
	{

		std::cout << "file opened"<<std::endl;
		streampos size = file.tellg();							//Get the file size.
		char* buffer = new char[size]; 							//create buffer for storing ROM contents
		
		file.seekg(0,ios::beg);									//Get pointer to beginning.
		file.read(buffer,size);									//Read from file and store in buffer
		file.close();											//close the file

		for(long i=0;i<size;i++)								//Load instructions stored in buffer into chip8 memory starting at 0x200
		{
			memory[start_address+i]=buffer[i];
		}

		delete[]buffer;

	}
}


void Chip8::Table0() {
    ((*this).*(table0[opcode & 0x000Fu]))();
}
void Chip8::Table8() {
    ((*this).*(table8[opcode & 0x000Fu]))();
}
void Chip8::TableE() {
    ((*this).*(tableE[opcode & 0x000Fu]))();
}
void Chip8::TableF() {
    ((*this).*(tableF[opcode & 0x00FFu]))();
}

void Chip8::OP_NULL() 
{
    std::cout << "Unknown or unimplemented instruction!\n";
}
	

Chip8::Chip8() : randGen(std::chrono::system_clock::now().time_since_epoch().count())
{
	pc=start_address;
	for(unsigned int i=0;i<FONTSET_SIZE;i++)
	{
		memory[fontset_address+i]=fontset[i];
	}

    randByte = std::uniform_int_distribution<uint8_t>(0, 255U);  
	
		table[0x0] = &Chip8::Table0;
		table[0x1] = &Chip8::OP_1nnn;
		table[0x2] = &Chip8::OP_2nnn;
		table[0x3] = &Chip8::OP_3xkk;
		table[0x4] = &Chip8::OP_4xkk;
		table[0x5] = &Chip8::OP_5xy0;
		table[0x6] = &Chip8::OP_6xkk;
		table[0x7] = &Chip8::OP_7xkk;
		table[0x8] = &Chip8::Table8;
		table[0x9] = &Chip8::OP_9xy0;
		table[0xA] = &Chip8::OP_Annn;
		table[0xB] = &Chip8::OP_Bnnn;
		table[0xC] = &Chip8::OP_Cxkk;
		table[0xD] = &Chip8::OP_Dxyn;
		table[0xE] = &Chip8::TableE;
		table[0xF] = &Chip8::TableF;

		for (size_t i = 0; i <= 0xE; i++)
		{
			table0[i] = &Chip8::OP_NULL;
			table8[i] = &Chip8::OP_NULL;
			tableE[i] = &Chip8::OP_NULL;
		}

		table0[0x0] = &Chip8::OP_00E0;
		table0[0xE] = &Chip8::OP_00EE;

		table8[0x0] = &Chip8::OP_8xy0;
		table8[0x1] = &Chip8::OP_8xy1;
		table8[0x2] = &Chip8::OP_8xy2;
		table8[0x3] = &Chip8::OP_8xy3;
		table8[0x4] = &Chip8::OP_8xy4;
		table8[0x5] = &Chip8::OP_8xy5;
		table8[0x6] = &Chip8::OP_8xy6;
		table8[0x7] = &Chip8::OP_8xy7;
		table8[0xE] = &Chip8::OP_8xyE;

		tableE[0x1] = &Chip8::OP_ExA1;
		tableE[0xE] = &Chip8::OP_Ex9E;

		for (size_t i = 0; i <= 0x65; i++)
		{
			tableF[i] = &Chip8::OP_NULL;
		}

		tableF[0x07] = &Chip8::OP_Fx07;
		tableF[0x0A] = &Chip8::OP_Fx0A;
		tableF[0x15] = &Chip8::OP_Fx15;
		tableF[0x18] = &Chip8::OP_Fx18;
		tableF[0x1E] = &Chip8::OP_Fx1E;
		tableF[0x29] = &Chip8::OP_Fx29;
		tableF[0x33] = &Chip8::OP_Fx33;
		tableF[0x55] = &Chip8::OP_Fx55;
		tableF[0x65] = &Chip8::OP_Fx65;

}

//Instructions - start

void Chip8::OP_00E0()
{
	memset(display,0,sizeof(display));
}

void Chip8::OP_00EE()
{
	--sp;
	pc = stack[sp];

}

void Chip8::OP_1nnn()
{
	uint16_t address = opcode & 0x0FFFu;
	pc = address;
}


void Chip8::OP_2nnn()
{
	uint16_t address = opcode & 0x0FFFu;
	stack[sp]=pc;
	++sp;
	pc = address;
}

void Chip8::OP_3xkk()
{
	uint8_t Vx =(opcode & 0x0F00)>>8u;
	uint8_t byte = opcode & 0x0FF0u;
	if(V[Vx]==byte)
	{
		pc+=2;
	}
}


void Chip8::OP_4xkk()
{
	uint8_t Vx =(opcode & 0x0F00)>>8u;
	uint8_t byte = opcode & 0x0FF0u;
	if(V[Vx]!=byte)
	{
		pc+=2;
	}
}

void Chip8::OP_5xy0()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >>4u;
	if(V[Vx]==V[Vy])
	{
		pc+=2;
	}
}

void Chip8::OP_6xkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;
	V[Vx]= byte;
}

void Chip8::OP_7xkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;
	V[Vx] += byte;
}

void Chip8::OP_8xy0()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	V[Vx]= V[Vy];
}

void Chip8::OP_8xy1()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	V[Vx] |= V[Vy];
}

void Chip8::OP_8xy2()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	V[Vx] &= V[Vy];
}

void Chip8::OP_8xy3()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	V[Vx] ^= V[Vy];
}

void Chip8::OP_8xy4()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	uint16_t sum = V[Vx] + V[Vy];

	if (sum > 255U)
	{
		V[0xF] = 1;
	}
	else
	{
		V[0xF] = 0;
	}

	V[Vx] = sum & 0xFFu;
}

void Chip8::OP_8xy5()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (V[Vx] > V[Vy])
	{
		V[0xF] = 1;
	}
	else
	{
		V[0xF] = 0;
	}

	V[Vx] -= V[Vy];
}

void Chip8::OP_8xy6()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	// Save LSB in VF
	V[0xF] = (V[Vx] & 0x1u);

	V[Vx] >>= 1;
}

void Chip8::OP_8xy7()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (V[Vy] > V[Vx])
	{
		V[0xF] = 1;
	}
	else
	{
		V[0xF] = 0;
	}

	V[Vx] = V[Vy] - V[Vx];
}


void Chip8::OP_8xyE()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	// Save MSB in VF
	V[0xF] = (V[Vx] & 0x80u) >> 7u;

	V[Vx] <<= 1;
}

void Chip8::OP_9xy0()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (V[Vx] != V[Vy])
	{
		pc += 2;
	}
}

void Chip8::OP_Annn()
{
	uint16_t address = opcode & 0x0FFFu;

	index = address;
}

void Chip8::OP_Bnnn()
{
	uint16_t address = opcode & 0x0FFFu;

	pc = V[0] + address;
}

void Chip8::OP_Cxkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	V[Vx] = randByte(randGen) & byte;
}

void Chip8::OP_Dxyn()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	uint8_t height = opcode & 0x000Fu;

	// Wrap if going beyond screen boundaries
	uint8_t xPos = V[Vx] % 64;
	uint8_t yPos = V[Vy] % 32;

	V[0xF] = 0;

	for (unsigned int row = 0; row < height; ++row)
	{
		uint8_t spriteByte = memory[index + row];

		for (unsigned int col = 0; col < 8; ++col)
		{
			uint8_t spritePixel = spriteByte & (0x80u >> col);
			uint32_t* screenPixel = &display[(yPos + row) * 64 + (xPos + col)];

			// Sprite pixel is on
			if (spritePixel)
			{
				// Screen pixel also on - collision
				if (*screenPixel == 0xFFFFFFFF)
				{
					V[0xF] = 1;
				}

				// Effectively XOR with the sprite pixel
				*screenPixel ^= 0xFFFFFFFF;
			}
		}
	}
}

void Chip8::OP_Ex9E()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	uint8_t key = V[Vx];

	if (keys[key])
	{
		pc += 2;
	}
}
void Chip8::OP_ExA1()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	uint8_t key = V[Vx];

	if (!keys[key])
	{
		pc += 2;
	}
}

void Chip8::OP_Fx07()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	V[Vx] = delay_timer;
}

void Chip8::OP_Fx0A()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	if (keys[0])
	{
		V[Vx] = 0;
	}
	else if (keys[1])
	{
		V[Vx] = 1;
	}
	else if (keys[2])
	{
		V[Vx] = 2;
	}
	else if (keys[3])
	{
		V[Vx] = 3;
	}
	else if (keys[4])
	{
		V[Vx] = 4;
	}
	else if (keys[5])
	{
		V[Vx] = 5;
	}
	else if (keys[6])
	{
		V[Vx] = 6;
	}
	else if (keys[7])
	{
		V[Vx] = 7;
	}
	else if (keys[8])
	{
		V[Vx] = 8;
	}
	else if (keys[9])
	{
		V[Vx] = 9;
	}
	else if (keys[10])
	{
		V[Vx] = 10;
	}
	else if (keys[11])
	{
		V[Vx] = 11;
	}
	else if (keys[12])
	{
		V[Vx] = 12;
	}
	else if (keys[13])
	{
		V[Vx] = 13;
	}
	else if (keys[14])
	{
		V[Vx] = 14;
	}
	else if (keys[15])
	{
		V[Vx] = 15;
	}
	else
	{
		pc -= 2;
	}
}


void Chip8::OP_Fx15()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	delay_timer = V[Vx];
}

void Chip8::OP_Fx18()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	sound_timer = V[Vx];
}
void Chip8::OP_Fx1E()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	index += V[Vx];
}

void Chip8::OP_Fx29()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t digit = V[Vx];

	index = fontset_address + (5 * digit);
}

void Chip8::OP_Fx33()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t value = V[Vx];

	// Ones-place
	memory[index + 2] = value % 10;
	value /= 10;

	// Tens-place
	memory[index + 1] = value % 10;
	value /= 10;

	// Hundreds-place
	memory[index] = value % 10;
}

void Chip8::OP_Fx55()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (uint8_t i = 0; i <= Vx; ++i)
	{
		memory[index + i] = V[i];
	}
}

void Chip8::OP_Fx65()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (uint8_t i = 0; i <= Vx; ++i)
	{
		V[i] = memory[index + i];
	}
}

//Instructions - end 

int srno = 0;

void Chip8::machine_cycle()
{
	// Fetch
	opcode = (memory[pc] << 8u) | memory[pc + 1];
	/* std::cout<<endl;
	std::cout<<endl;
    std::cout << "  " <<setw(2)<<setfill('0')<< srno << "  "<<"PC: 0x" << std::hex << pc  << " | Opcode: 0x" << opcode<< " | SP: 0x" <<setw(4)<<setfill('0')<<std::hex<<sp<<endl;
	std::cout<<"=================================================================================================="<<std::endl;
	std::cout<<"   REGISTORS "<<"\t\t"<<"      STACK "<<endl;
	 std::cout<<"=================================================================================================="<<std::endl;
	
	for (int i = 0; i < 16; i++) {
    std::cout <<  "  V[" << std::setw(2)<<std::setfill('0') << i << "] = 0x" << std::setw(2)<<std::setfill('0') <<std::hex << static_cast<int>(V[i]) << "           " <<"Stack["<< std::dec << std::setw(2)<<std::setfill('0') << i <<"]= 0x"<< std::setw(2)<<std::setfill('0') <<stack[i]<<std::endl;
}      
   
    std::cout<<"=================================================================================================="<<std::endl; 
	srno++; */

	// Increment the PC before we execute anything
	pc += 2;

	// Decode and Execute-
	((*this).*(table[(opcode & 0xF000u) >> 12u]))();

	// Decrement the delay timer if it's been set
	if (delay_timer > 0)
	{
		--delay_timer;
	}

	// Decrement the sound timer if it's been set
	if (sound_timer > 0)
	{
		--sound_timer;
	}
}

