//
// Created by Vikram Khatavkar on 10/3/17.
//

#ifndef SIMULATORPROJECT_C_SIMULATORPROJECT_H
#define SIMULATORPROJECT_C_SIMULATORPROJECT_H

#endif //SIMULATORPROJECT_C_SIMULATORPROJECT_H

#include<stdbool.h> // for boolean values

#define TRUE 1
#define FALSE 0
#define ALLOCATED 1
#define FREE 0
#define R 0
#define P 1
#define YES 1
#define NO 0
#define VALID 1
#define INVALID 0
//file instruction counter used in file_read()
int gfile_inst_counter = 1;

int counter_fetch = 0;
//int gintermediate_result=0;
int register_status;
int updated_PC = 0;
int is_inst_jump = FALSE;
int is_inst_jal = FALSE;

int physical_index=0;
int new_base_address = 0;


int execute_div4_free = TRUE;
int execute_mul2_free = TRUE;
int execute_int_free = TRUE;
int decode_stalled_flag = FALSE;
int execute_mul1_free = TRUE;
int execute_div1_free = TRUE;

//function prototypes
void output_buffer_fetch(void);

void input_buffer_decode(void);

void output_buffer_decodeA(void);

void output_buffer_decodeB(void);

void input_buffer_execute(void);

void output_buffer_execute(void);

void output_buffer_execute_multiplication1(void);

void input_buffer_execute_multiplication1(void);

void output_buffer_execute_multiplication2(void);

void input_buffer_execute_multiplication2(void);

void input_buffer_execute_div1(void);

void output_buffer_execute_div1(void);

void input_buffer_execute_div2(void);

void output_buffer_execute_div2(void);

void input_buffer_execute_div3(void);

void output_buffer_execute_div3(void);

void input_buffer_execute_div4(void);

void output_buffer_execute_div4(void);

void input_buffer_memory_div(void);

void input_buffer_memory_mul(void);

void input_buffer_memory_exec(void);

void output_buffer_memory(void);

void input_buffer_writeback(void);

void decode_stage_operations(void);

void simulation(int);

void initialization(void);

void display_values(void);

void fetch(void);

void decode(void);

void issue_queue(void);

void copy_buffer_to_execute(void);

void execute(void);

void execute_multiplication1(void);

void execute_multiplication2(void);

void execute_division1(void);

void execute_division2(void);

void execute_division3(void);

void execute_division4(void);

void execute_arithmetic(void);

void memory(void);

void writeback(void);

void input_buffer_iq(void);

void output_buffer_iq(void);

void issue_queue_decode(void);
void output_buffer_iq_intFU(int num);
void output_buffer_iq_MUL(int num);
void output_buffer_iq_DIV(int num);



char **split_line(char *shellLine);


struct code_line {
    int file_line_number;
    int address; //Multiple of 4
    char instruction_string[25];
};

struct code_memory {
    struct code_line arr_code_line[40]; // - filled in by reading from code file.
};
struct code_memory code_memory1;

struct data_memory {
    int base_address;// integer (where this starts)
    int data_array[4000]; // integers, indexed by offset of word address from base address
};
struct data_memory data_memory1;

struct registers {
    int value; // integer
    int address;
    int status; // VALID or INVALID
};

struct register_file {
    struct registers reg[16]; //: array of struct Registers
    struct registers dummy;
};
struct register_file register_file1;



struct physical_register_file {
    struct registers prf[32];
};
struct physical_register_file physical_RF;

struct RAT {
    int RorP;
    int index_R;
    int index_P;
};
struct RAT RAT1[16];

struct stats {
    int cycle; // integer (initialize to zero)
};
struct stats stats1;


struct instruction_info {
    int PC;
    char instruction_string[25];// (YOU WILL FIND IT USEFUL TO ADD A NOP)
    char mnemonic[6];
    struct registers source_register1; //value of source register 1
    int src1_RorP;
    struct registers source_register2; //value of source register 2
    int src2_RorP;
    struct registers dest_register; // value of dest register
    int dest_RorP;
    int target_memory_addr; //= NULL; //(address, null if absent)
    int target_memory_data; //= NULL; //(data, null if absent)
    int result;
    int literal;
    int validity;
    int selected_inst;
};
struct instruction_info input_buffer, output_buffer;

struct stage {
    struct instruction_info input_buffer;
    struct instruction_info output_buffer;
    bool stalled; //0=false, 1=True
};
struct stage stage_fetch,
        stage_decode,
        stage_issue_queue,stage_iq_intFU,stage_iq_MUL,stage_iq_DIV,
        stage_execute,
        stage_mul1, stage_mul2,
        stage_div1, stage_div2, stage_div3, stage_div4,
        stage_mem,
        stage_wb,
        common;

struct flags {
    bool zero; // : (true or false)
    bool nonzero;
    bool jump;
    bool BZ;
    bool BNZ;
    bool HALT;
    //bool carry; //: (true or false)
    // bool Negative; //: (true or false)
};

struct flags flags1;

struct forward_bus {
    int result;
    int register_address;
};

struct forward_bus bus1, bus2, bus3;

struct issue_queue_fields{
    int status_of_allocation;
    int src1_value;
    int src1_index;
    int src1_status;
    int src1_RorP;
    int src2_value;
    int src2_index;
    int src2_status;
    int src2_RorP;
    int dest_index;
    int dest_RorP;
    char inst_opcode[6];
    int ROB_index;
    int LSQ_index;
    char original_instruction_string[25];
    char translated_instruction_string[25];
    int clock_cycle;
    int wakeup;
};
struct issue_queue_fields issue_queue_struct[32];

struct issue_queue_selection_logic{
    struct issue_queue_fields selection_logic[32];
};

struct issue_queue_selection_logic issue_queue_selection;

struct issue_queue_FU_selection_logic{
    struct issue_queue_fields selection_FU[32];
};

struct issue_queue_FU_selection_logic issue_queue_FU_selection;

struct Queue
{
    int front, rear, size;
    unsigned capacity;
    int* array;
};

struct ROB_struct{
    int status_of_allocation_ROB;
    int src1_value;
    int src1_index;
    int src1_status;
    int src1_RorP;
    int src2_value;
    int src2_index;
    int src2_status;
    int src2_RorP;
    int dest_index;
    int dest_RorP;
    char inst_opcode[6];
    int ROB_index;
    int LSQ_index;
    char original_instruction_string[25];
    char translated_instruction_string[25];
    int clock_cycle;
    int wakeup;
    int result;
    int ex_code;
};
struct ROB_struct ROB[32];

void create_ROB(void);

void ROB_stage(void);
void LSQ_stage(void);