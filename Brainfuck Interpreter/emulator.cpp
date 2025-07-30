#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include<string>
#include<fstream>

void print_memory_state(const unsigned char* tape, const unsigned char* ptr, const char* pc) {
    printf("\nInstruction: %c\n", *pc);
    printf("Tape: [ ");
    for (int i = 0; i < 10; i++) {  // Show first 10 cells
        if (&tape[i] == ptr)
            printf("(%d) ", tape[i]); // Highlight pointer
        else
            printf("%d ", tape[i]);
    }
    printf("]\n");
}


void interpret(const char* code) {
    unsigned char tape[30000] = {0};
    unsigned char* ptr = tape;
    const char* pc = code;

    while (*pc) {
    	
    	
        switch (*pc) {
            case '>': ptr++; break;
            case '<': ptr--; break;
            case '+': (*ptr)++; break;
            case '-': (*ptr)--; break;
            case '.': putchar(*ptr); break;
            case ',': *ptr = getchar(); break;
            case 'm': printf("\n%d\n",tape[1]);break;
            case '[':
                if (!*ptr) {
                    int loop = 1;
                    while (loop) {
                        pc++;
                        if (*pc == '[') loop++;
                        else if (*pc == ']') loop--;
                    }
                }
                break;
            case ']':
                if (*ptr) {
                    int loop = 1;
                    while (loop) {
                        pc--;
                        if (*pc == '[') loop--;
                        else if (*pc == ']') loop++;
                    }
                }
                break;
        }
        pc++;
    }
     print_memory_state(tape, ptr, pc);
}


int main(int argc, char* argv[]) {
	if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile) {
        std::cerr << "Error: Could not open file " << argv[1] << "\n";
        return 1;
    }

    std::string fileContent((std::istreambuf_iterator<char>(inputFile)),std::istreambuf_iterator<char>());
                           
    const char *program = fileContent.c_str();
    
    interpret(program);
    return 0;
}
