// * xvm.c
#include <stdio.h>

int registers[4]; /* Load immediate value, 
                     Add immediate value, 
                     Halt reg */

int is_running = 0;

int bytecode_program[] = { 0x1064, 0x11C8, 0x2201, 0x0000 };

int prog_cnt = 0; /* program counter */

int instrNum = 0;
int reg1 = 0;
int reg2 = 0;
int reg3 = 0;
int imm  = 0;

int fetch() {
    return bytecode_program[ prog_cnt++ ];
}

void decode(int instruction) {
  instrNum  = (instruction & 0xf000) >> 12;
  reg1      = (instruction & 0xf00 ) >> 8;
  reg2      = (instruction & 0xf0  ) >> 4;
  reg3      = (instruction & 0xff  );
  imm       = (instruction & 0xff  );
}

void pretty_print() {
    int counter;
    if(is_running) {
    printf("\n*** instruction load for given call above: \n");
    for(counter = 0; counter < 4; /* hard-coded */ counter++) {
            printf(":: -> %04x\n", registers[counter]);
        }
    }
    printf("\n\n");
}

void parse() {
    FILE *fp;
    fp = fopen("gen.js", "w+");

    switch(instrNum) {
        case 0: 
        // HALT called.
            is_running = 0;
            fprintf(fp, "//HALT bytecode instruction scanned.\n\n");
            printf("\n**Program halted, exiting...**\n");
            break;

        case 1:
        //LOAD IMMEDIATE called.
            fprintf(fp, "//load call\n");
            printf("--loadi called\t -> ");
            printf("loadi r%d #%d\n", reg1, imm);
            registers[reg1] = imm;
            break;

        case 2:
        // ADD IMMEDIATE called.
            printf("add r%d r%d r%d\n", reg1, reg2, reg3);
            registers[reg1] = registers[reg2] + registers[reg3];
            fprintf(fp, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nlet b = %d;\nlet c = %d;\nlet a = b + c;\n", registers[reg1], registers[reg2], registers[reg3]); /* cant be bothred with buffer fuckery. This looks ugly, but it will do for now. */
    }
}

void init_vm() {
    while(is_running) {
        pretty_print();
        int instructions = fetch();
        decode(instructions);
        parse();
        
    }

    pretty_print();

}

int main() {
    is_running = 1;
    printf("Generating Javascript code...\n");
    printf("Parsed bytecode:");
    init_vm();
}
