Emulators
=========

Emulators are software tools that simulate hardware components or entire systems,
allowing you to understand and experiment with computer architecture on your PC.

In this repository, I created a few emulators to help understand how CPU architectures
work at a low level by building them from scratch.

Projects
--------

1. Brainfuck Interpreter
------------------------
A simple interpreter for the Brainfuck programming language — an esoteric language with only 8 commands.

Files:
- emulator.cpp  -> Source code for the interpreter
- Examples/     -> Contains sample exmaples in .txt format.
- output.exe    -> Compiled executable (Windows)

To compile:
    g++ emulator.cpp -o output

To run:
    output.exe test.txt

2. Coming Soon: Intel 8080 Emulator
-----------------------------------
This will emulate the classic Intel 8080 CPU, including:
- Opcode decoding
- Registers and flags
- Memory simulation
- Optional assembler support

License
-------
This project is licensed under the MIT License.

Author
------
EmbeddedKidaa (https://github.com/EmbeddedKidaa)
