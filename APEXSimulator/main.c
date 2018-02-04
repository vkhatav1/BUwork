#include <stdio.h>
#include <string.h>
#include<stdlib.h> // for exit(0) predefined function
#include "SimulatorProject.h"
#include <limits.h>

int max_number;

int main(int argc, char *argv[]) {

    initialization();
    //display_values();

    //file_read();
    char instruction[25];
    memset(instruction, 0, sizeof(instruction));
    int file_inst_counter = 0;


    FILE *fptr = fopen(argv[1], "r");
    if (fptr != NULL) {

        while (fgets(instruction, sizeof(instruction), fptr) !=
               NULL) //fgets() reads one line at a time and store it in the array
        {

            //printf(stdout, "%s", line); //print the file contents on stdout.
            code_memory1.arr_code_line[file_inst_counter].file_line_number = file_inst_counter;

            if (file_inst_counter == 0) // when the function is called for the first time
                code_memory1.arr_code_line[file_inst_counter].address = 4000; // base address where the instruction starts
            else
                code_memory1.arr_code_line[file_inst_counter].address = (
                        (code_memory1.arr_code_line[file_inst_counter - 1].address) +
                        4);  //increment the address by 4 for every instruction
            strcpy(code_memory1.arr_code_line[file_inst_counter].instruction_string, instruction);
            file_inst_counter++; //increment the file instruction counter
        }
        fclose(fptr);
    } else {
        printf("\n File not found!!!\nEXITING..!!!");
        exit(0);
    }

    for (int i = 0; i < file_inst_counter; ++i) {
        printf("\n The Line Number is %d", (code_memory1.arr_code_line[i].file_line_number + 1));
        printf("\n The Instruction Address is %d", code_memory1.arr_code_line[i].address);
        printf("\n The String is %s\n", code_memory1.arr_code_line[i].instruction_string);
    }

    printf("\n Enter the number of times you want to execute the pipeline\n");
    scanf("%d", &max_number);

    simulation(max_number);

    return 0;
}

void simulation(int max_number) {
    int n = 0;
    while (n < max_number) {   //stats1.cycle=1;
        printf("\n**********************************************************\n");
        printf("\nCurrent cycle is : %d", stats1.cycle);
        if (flags1.HALT == TRUE) {
            // Empty all the pipeline
            writeback();
            memory();
            writeback();
            exit(0);
        }
        writeback();
        memory();
        execute();
        issue_queue();
        decode();
        fetch();
        //set the stalled flag to default
        stage_fetch.stalled = FALSE;
        stage_decode.stalled = FALSE;
        if (register_status == TRUE) {
            common.stalled = FALSE;
        }
        stats1.cycle++;
        n++;
    }
}

void execute(void) {
    if ((stage_div3.output_buffer.PC != 0))
        execute_division4();
    else if ((stage_div2.output_buffer.PC != 0))
        execute_division3();
    else if ((stage_div2.input_buffer.PC != 0))
        execute_division2();
    else if (stage_iq_DIV.output_buffer.PC != 0)
        execute_division1();

    if ((stage_mul2.input_buffer.PC != 0))
        execute_multiplication2();
    else if (stage_iq_MUL.output_buffer.PC != 0)
        execute_multiplication1();

    if (stage_iq_intFU.output_buffer.PC != 0)
        execute_arithmetic();



    // transferring data to the input buffer of memory stage
    if ((stage_execute.output_buffer.PC != 0) && (stage_div4.output_buffer.PC != 0) &&
        (stage_mul2.output_buffer.PC != 0)) {
        input_buffer_memory_div();
        execute_div4_free = TRUE;

    } else if ((stage_execute.output_buffer.PC != 0) && (stage_div4.output_buffer.PC != 0) &&
               (stage_mul2.output_buffer.PC == 0)) {
        input_buffer_memory_div();
        execute_div4_free = TRUE;
    } else if ((stage_execute.output_buffer.PC != 0) && (stage_div4.output_buffer.PC == 0) &&
               (stage_mul2.output_buffer.PC == 0)) {
        input_buffer_memory_exec();
        execute_int_free = TRUE;
    } else if ((stage_execute.output_buffer.PC == 0) && (stage_div4.output_buffer.PC == 0) &&
               (stage_mul2.output_buffer.PC == 0)) {
        return;
    } else if ((stage_execute.output_buffer.PC == 0) && (stage_div4.output_buffer.PC == 0) &&
               (stage_mul2.output_buffer.PC != 0)) {
        input_buffer_memory_mul();
        execute_mul2_free = TRUE;
    } else if ((stage_execute.output_buffer.PC == 0) && (stage_div4.output_buffer.PC != 0) &&
               (stage_mul2.output_buffer.PC != 0)) {
        input_buffer_memory_div();
        execute_div4_free = TRUE;
    } else if ((stage_execute.output_buffer.PC == 0) && (stage_div4.output_buffer.PC != 0) &&
               (stage_mul2.output_buffer.PC == 0)) {
        input_buffer_memory_div();
        execute_div4_free = TRUE;
    } else if ((stage_execute.output_buffer.PC != 0) && (stage_div4.output_buffer.PC == 0) &&
               (stage_mul2.output_buffer.PC != 0)) {
        input_buffer_memory_mul();
        execute_mul2_free = TRUE;
    }
}

void fetch(void) {
    if (updated_PC != 0 && is_inst_jump == TRUE) {
        //instruction address
        for (int i = 0; i < 16; ++i) {
            if (updated_PC == code_memory1.arr_code_line[i].address) {
                stage_fetch.output_buffer.PC = code_memory1.arr_code_line[i].address;
                strcpy(stage_fetch.output_buffer.instruction_string, code_memory1.arr_code_line[i].instruction_string);
                printf("\nInstruction Address: %d\tCurrent Instruction: %s \n", stage_fetch.output_buffer.PC,
                       stage_fetch.output_buffer.instruction_string);
                counter_fetch = i;
                counter_fetch++;
            }
        }

        updated_PC = 0;
        is_inst_jump = FALSE;
    } else if (updated_PC != 0 && is_inst_jal == TRUE) {
        //instruction address
        for (int i = 0; i < 16; ++i) {
            if (updated_PC == code_memory1.arr_code_line[i].address) {
                stage_fetch.output_buffer.PC = code_memory1.arr_code_line[i].address;
                strcpy(stage_fetch.output_buffer.instruction_string, code_memory1.arr_code_line[i].instruction_string);
                printf("\nInstruction Address: %d\tCurrent Instruction: %s \n", stage_fetch.output_buffer.PC,
                       stage_fetch.output_buffer.instruction_string);
                counter_fetch = i;
                counter_fetch++;
                break;
            }
        }

        updated_PC = 0;
        is_inst_jal = FALSE;
    } else {
        if (flags1.BZ == TRUE) {
            //new_base_address=stage_execute.input_buffer.result;
            output_buffer_fetch();
            flags1.BZ = FALSE;
        } else if (common.stalled == FALSE && decode_stalled_flag == FALSE &&
                   stage_fetch.stalled == FALSE) //check for stall
        {
            //fetch data
            printf("\n***************************Fetch Stage*********************************\n");

            gfile_inst_counter++;
            //display_values();
            output_buffer_fetch();
        }
    }

}

void decode(void) {

    if ((stage_fetch.output_buffer.PC != 0) &&
        stage_decode.stalled == FALSE) //Check if the stall is triggered or output buffer is allocated
    {
        //decode instruction
        printf("\n***************************Decode/RF Stage*********************************\n");
        input_buffer_decode();
        decode_stage_operations();
        //copy_buffer_to_execute();
    }
}

void copy_buffer_to_execute(void) {
    if (decode_stalled_flag == FALSE) {
        if (!strcmp(stage_iq_DIV.output_buffer.mnemonic, "DIV")) {
            input_buffer_execute_div1();
        } else if (!strcmp(stage_iq_MUL.output_buffer.mnemonic, "MUL")) {
            input_buffer_execute_multiplication1();
        } else if (strcmp(stage_iq_DIV.output_buffer.mnemonic, "DIV") &&
                   strcmp(stage_iq_MUL.output_buffer.mnemonic, "MUL")) {
            input_buffer_execute();
        }
    } else if (decode_stalled_flag == TRUE) {
        if (!strcmp(stage_iq_DIV.output_buffer.mnemonic, "DIV")) {
            if (execute_div1_free == TRUE)
                input_buffer_execute_div1();
        } else if (!strcmp(stage_iq_MUL.output_buffer.mnemonic, "MUL")) {
            if (execute_mul1_free == TRUE)
                input_buffer_execute_multiplication1();
        } else if (strcmp(stage_iq_DIV.output_buffer.mnemonic, "DIV") &&
                   strcmp(stage_iq_MUL.output_buffer.mnemonic, "MUL")) {
            if (execute_int_free == TRUE)
                input_buffer_execute();
        }
    }

}

void execute_division4(void) {
    //division 4
    if ((stage_div3.output_buffer.PC != 0)) {
        printf("\n***************************DIV4 Stage*********************************\n");
        execute_div4_free = FALSE;

        input_buffer_execute_div4();
        //just forward the data
        //in DIV divide the value of source_register1 and source_register2,
        //and store the output in the result
        stage_div4.input_buffer.result =
                (stage_div4.input_buffer.source_register1.value) / (stage_div4.input_buffer.source_register2.value);
        bus1.result = stage_div4.input_buffer.result;
        bus1.register_address = stage_div4.input_buffer.dest_register.address;
        printf("The result in DIV4 is %d: \n", stage_div4.input_buffer.result);
        output_buffer_execute_div4();
    }
}

void execute_division3(void) {
    //division 3
    if ((stage_div2.output_buffer.PC != 0)) {
        printf("\n***************************DIV3 Stage*********************************\n");

        input_buffer_execute_div3();
        //just forward the data
        output_buffer_execute_div3();
    }
}

void execute_division2(void) {
    //division 2
    if ((stage_div2.input_buffer.PC != 0)) {
        printf("\n***************************DIV2 Stage*********************************\n");

        // input_buffer_execute_div2();
        //just forward the data
        output_buffer_execute_div2();
    }
}

void execute_division1(void) {
    //division 1
    printf("\n***************************DIV1 Stage*********************************\n");
    execute_div1_free = FALSE;
    input_buffer_execute_div1();
    //div 1 operations
    printf("\n Inside execute_division 1 \n");
    output_buffer_execute_div1();
    input_buffer_execute_div2();
    execute_div1_free = TRUE;

}

void execute_multiplication2(void) {
    //multiplication 2
    if ((stage_mul2.input_buffer.PC != 0)) {
        execute_mul2_free = FALSE;
        printf("\n***************************MUL2 Stage*********************************\n");

        printf("\n Inside execute_multiplication 2 \n");

        stage_mul2.input_buffer.result =
                (stage_mul2.input_buffer.source_register1.value) * (stage_mul2.input_buffer.source_register2.value);
        bus2.result = stage_mul2.input_buffer.result;
        bus2.register_address = stage_mul2.input_buffer.dest_register.address;
        printf("The result in MUL 2 : %d\n", stage_mul2.input_buffer.result);
        output_buffer_execute_multiplication2();
    }
}

void execute_multiplication1(void) {
    //multiplication 1
    printf("\n***************************MUL1 Stage*********************************\n");
    execute_mul1_free = FALSE;

    input_buffer_execute_multiplication1();

    output_buffer_execute_multiplication1();
    input_buffer_execute_multiplication2();
    execute_mul1_free = TRUE;
}

void execute_arithmetic(void) {
    //perform arithmetic operations
    printf("\n***************************Execute Stage*********************************\n");
    input_buffer_execute();
    execute_int_free = FALSE;
    int result = 0;
    if (!strcmp(stage_execute.input_buffer.mnemonic, "NOP")) {
        // Push NOP instruction
        printf("\nThe NOP instruction is being executed\n");
        stage_execute.output_buffer.PC = 1;
        strcpy(stage_execute.output_buffer.mnemonic, "NOP");
        return;
    } else if (!strcmp(stage_execute.input_buffer.mnemonic, "STORE")) {
        // in store add the values of source_register2 and literal,
        // which will give you the memory address to be assessed
        result = (stage_execute.input_buffer.source_register2.value) + (stage_execute.input_buffer.literal);
        stage_execute.input_buffer.target_memory_addr = result;

        printf("Memory location to be accessed: %d\n", result);

    } else if (!strcmp(stage_execute.input_buffer.mnemonic, "LOAD")) {
        // in load add the values of source_register1 and literal,
        // which will give you the memory address to be assessed
        result = (stage_execute.input_buffer.source_register1.value) + (stage_execute.input_buffer.literal);
        stage_execute.input_buffer.target_memory_addr = result;
        printf("The memory location to be accessed : %d\n", result);

    } else if (!strcmp(stage_execute.input_buffer.mnemonic, "ADD")) {
        //in ADD add the value of source_register1 and source_register2,
        //and store the output in the result
        result = (stage_execute.input_buffer.source_register1.value) +
                 (stage_execute.input_buffer.source_register2.value);
        stage_execute.input_buffer.result = result;
        bus3.register_address = stage_execute.input_buffer.dest_register.address;
        bus3.result = stage_execute.input_buffer.result;
        printf("The result after addition : %d\n", result);
    } else if (!strcmp(stage_execute.input_buffer.mnemonic, "MOVC")) {
        //if the instruction contains register to register inst. then the value of source reg will be added with 0 and the literal with will be 0
        result = (stage_execute.input_buffer.literal) + 0 + (stage_execute.input_buffer.source_register1.value);
        stage_execute.input_buffer.result = result;
        bus3.register_address = stage_execute.input_buffer.dest_register.address;
        bus3.result = stage_execute.input_buffer.result;
        printf("The value to be moved in the destination register : %d\n", result);
    } else if (!strcmp(stage_execute.input_buffer.mnemonic, "SUB")) {
        //in SUB subtract the value of source_register1 and source_register2,
        //and store the output in the result
        result = (stage_execute.input_buffer.source_register1.value) -
                 (stage_execute.input_buffer.source_register2.value);
        stage_execute.input_buffer.result = result;
        bus3.register_address = stage_execute.input_buffer.dest_register.address;
        bus3.result = stage_execute.input_buffer.result;
        printf("The result after Subtraction : %d\n", result);
    } else if (!strcmp(stage_execute.input_buffer.mnemonic, "BNZ")) {
        //BNZ operation
        printf("BNZ operation\n");
        if (flags1.nonzero == TRUE) {
            result = stage_execute.input_buffer.PC + stage_execute.input_buffer.literal;
            new_base_address = result;
            flags1.BNZ = TRUE;
            stage_mem.output_buffer.PC = 0;
            stage_execute.output_buffer.PC = 0;
            stage_mul2.output_buffer.PC = 0;
            stage_mul1.output_buffer.PC = 0;
            stage_decode.output_buffer.PC = 0;
            stage_decode.output_buffer.PC = 0;
            stage_fetch.output_buffer.PC = 0;
            return;
        }
    } else if (!strcmp(stage_execute.input_buffer.mnemonic, "BZ")) {
        //BZ operation
        printf("BZ operation\n");
        if (flags1.zero == TRUE) {
            result = stage_execute.input_buffer.PC + stage_execute.input_buffer.literal;
            new_base_address = result;
            flags1.BZ = TRUE;
            stage_mem.output_buffer.PC = 0;
            stage_execute.output_buffer.PC = 0;
            stage_mul2.output_buffer.PC = 0;
            stage_mul1.output_buffer.PC = 0;
            stage_decode.output_buffer.PC = 0;
            stage_decode.output_buffer.PC = 0;
            stage_fetch.output_buffer.PC = 0;
            return;
        }

    } else if (!strcmp(stage_execute.input_buffer.mnemonic, "OR")) {
        // OR operation
        result = (stage_execute.input_buffer.source_register1.value) |
                 (stage_execute.input_buffer.source_register2.value);
        stage_execute.input_buffer.result = result;
        bus3.register_address = stage_execute.input_buffer.dest_register.address;
        bus3.result = stage_execute.input_buffer.result;
        printf("The result after OR operation : %d\n", result);
    } else if (!strcmp(stage_execute.input_buffer.mnemonic, "AND")) {
        // AND operation
        result = (stage_execute.input_buffer.source_register1.value) &
                 (stage_execute.input_buffer.source_register2.value);
        stage_execute.input_buffer.result = result;
        bus3.register_address = stage_execute.input_buffer.dest_register.address;
        bus3.result = stage_execute.input_buffer.result;
        printf("The result after AND operation : %d\n", result);

    } else if (!strcmp(stage_execute.input_buffer.mnemonic, "EX-OR")) {
        // EX-OR operation
        result = (stage_execute.input_buffer.source_register1.value) ^
                 (stage_execute.input_buffer.source_register2.value);
        stage_execute.input_buffer.result = result;
        bus3.register_address = stage_execute.input_buffer.dest_register.address;
        bus3.result = stage_execute.input_buffer.result;
        printf("The result after EX-OR operation : %d\n", result);
    } else if (!strcmp(stage_execute.input_buffer.mnemonic, "JUMP")) {
        // JUMP operation
        printf("JUMP operation\n");
        // flush fetch and decode stages
        memset(stage_fetch.output_buffer.instruction_string, 0, sizeof(stage_fetch.output_buffer.instruction_string));
        stage_fetch.output_buffer.PC = 0;

        stage_decode.input_buffer.PC = 0;
        memset(stage_decode.input_buffer.instruction_string, 0, sizeof(stage_decode.input_buffer.instruction_string));
        memset(stage_decode.input_buffer.mnemonic, 0, sizeof(stage_decode.input_buffer.mnemonic));
        memset(&stage_decode.input_buffer.dest_register, 0, sizeof(stage_decode.input_buffer.dest_register));
        memset(&stage_decode.input_buffer.source_register1, 0, sizeof(stage_decode.input_buffer.source_register1));
        memset(&stage_decode.input_buffer.source_register2, 0, sizeof(stage_decode.input_buffer.source_register2));
        stage_decode.input_buffer.literal = 0;

        stage_decode.output_buffer.PC = 0;
        memset(stage_decode.output_buffer.instruction_string, 0,
               sizeof(stage_decode.output_buffer.instruction_string));
        memset(stage_decode.output_buffer.mnemonic, 0, sizeof(stage_decode.output_buffer.mnemonic));
        memset(&stage_decode.output_buffer.dest_register, 0, sizeof(stage_decode.output_buffer.dest_register));
        memset(&stage_decode.output_buffer.source_register1, 0, sizeof(stage_decode.output_buffer.source_register1));
        memset(&stage_decode.output_buffer.source_register2, 0, sizeof(stage_decode.output_buffer.source_register2));
        stage_decode.output_buffer.literal = 0;

        //fetch and decode stage stall
        stage_fetch.stalled = TRUE;
        stage_decode.stalled = TRUE;

        // set jump flag
        is_inst_jump = TRUE;

        // calculate the new pc value
        stage_execute.input_buffer.result =
                stage_execute.input_buffer.source_register1.value + stage_execute.input_buffer.literal;
        updated_PC = stage_execute.input_buffer.result;
        printf("\n Result in execution stage is : %d", updated_PC);

    } else if (!strcmp(stage_execute.input_buffer.mnemonic, "JAL")) {
        // JAL operation
        printf("JAL operation\n");
        // flush fetch and decode stages
        memset(stage_fetch.output_buffer.instruction_string, 0, sizeof(stage_fetch.output_buffer.instruction_string));
        stage_fetch.output_buffer.PC = 0;

        stage_decode.input_buffer.PC = 0;
        memset(stage_decode.input_buffer.instruction_string, 0, sizeof(stage_decode.input_buffer.instruction_string));
        memset(stage_decode.input_buffer.mnemonic, 0, sizeof(stage_decode.input_buffer.mnemonic));
        memset(&stage_decode.input_buffer.dest_register, 0, sizeof(stage_decode.input_buffer.dest_register));
        memset(&stage_decode.input_buffer.source_register1, 0, sizeof(stage_decode.input_buffer.source_register1));
        memset(&stage_decode.input_buffer.source_register2, 0, sizeof(stage_decode.input_buffer.source_register2));
        stage_decode.input_buffer.literal = 0;

        stage_decode.output_buffer.PC = 0;
        memset(stage_decode.output_buffer.instruction_string, 0,
               sizeof(stage_decode.output_buffer.instruction_string));
        memset(stage_decode.output_buffer.mnemonic, 0, sizeof(stage_decode.output_buffer.mnemonic));
        memset(&stage_decode.output_buffer.dest_register, 0, sizeof(stage_decode.output_buffer.dest_register));
        memset(&stage_decode.output_buffer.source_register1, 0, sizeof(stage_decode.output_buffer.source_register1));
        memset(&stage_decode.output_buffer.source_register2, 0, sizeof(stage_decode.output_buffer.source_register2));
        stage_decode.output_buffer.literal = 0;

        //fetch and decode stage stall
        stage_fetch.stalled = TRUE;
        stage_decode.stalled = TRUE;

        // set JAL flag
        is_inst_jal = TRUE;

        // calculate the new pc value
        stage_execute.input_buffer.result =
                stage_execute.input_buffer.source_register1.value + stage_execute.input_buffer.literal;
        updated_PC = stage_execute.input_buffer.result;
        printf("\n Result in execution stage is : %d", updated_PC);

    } else if (!strcmp(stage_execute.input_buffer.mnemonic, "HALT")) {
        // HALT operation
        printf("HALT operation\n");
    }
    output_buffer_execute();
}

void memory(void) {
    //  if((stage_execute.output_buffer.PC!=0) && (stage_div4.output_buffer.PC!=0)&&(stage_mul2.output_buffer.PC!=0))
    if (stage_mem.input_buffer.PC != 0) {
        // input_buffer_memory_div();
        printf("\n***************************Memory Stage*********************************\n");

        if (!strcmp(stage_mem.input_buffer.mnemonic, "DIV")) {
            // NO ACTION
            printf("***NO ACTION***\n");
        } else if (!strcmp(stage_mem.input_buffer.mnemonic, "MUL")) {
            // NO ACTION
            printf("***NO ACTION***\n");
        } else if (!strcmp(stage_mem.input_buffer.mnemonic, "NOP")) {
            // Push NOP instruction
            printf("\nThe NOP instruction is being processed\n");
            stage_mem.output_buffer.PC = 1;
            strcpy(stage_mem.output_buffer.mnemonic, "NOP");
            return;
        } else if (!strcmp(stage_mem.input_buffer.mnemonic, "STORE")) {
            // access the memory location updated in target_memory_addr and
            // write the value of the source_register1 in the target memory
            data_memory1.base_address = stage_mem.input_buffer.target_memory_addr;
            data_memory1.data_array[data_memory1.base_address] = stage_mem.input_buffer.source_register1.value;
            printf("Data is being written in the memory\n");
            printf("MEMORY[%d]=%d\n", data_memory1.base_address, stage_mem.input_buffer.source_register1.value);

        } else if (!strcmp(stage_mem.input_buffer.mnemonic, "MOVC")) {
            // NO ACTION
            printf("***NO ACTION***\n");
        } else if (!strcmp(stage_mem.input_buffer.mnemonic, "LOAD")) {
            // access the memory location updated in target_memory_addr and
            // write the value of from the target memory to the dest_register
            data_memory1.base_address = stage_mem.input_buffer.target_memory_addr;
            stage_mem.input_buffer.target_memory_data = data_memory1.data_array[data_memory1.base_address];
            printf("Data to be written on the destination register\n");
            printf("MEMORY[%d]=%d\n", data_memory1.base_address, stage_mem.input_buffer.target_memory_data);
        } else if (!strcmp(stage_mem.input_buffer.mnemonic, "ADD")) {
            // NO ACTION
            printf("***NO ACTION***\n");
        } else if (!strcmp(stage_mem.input_buffer.mnemonic, "SUB")) {
            // NO ACTION
            printf("***NO ACTION***\n");
        } else if (!strcmp(stage_mem.input_buffer.mnemonic, "BNZ")) {
            //BNZ operation
            printf("BNZ operation*\n");
        } else if (!strcmp(stage_mem.input_buffer.mnemonic, "BZ")) {
            //BZ operation
            printf("BZ operation\n");
        } else if (!strcmp(stage_mem.input_buffer.mnemonic, "OR")) {
            // NO ACTION
            printf("***NO ACTION***\n");
        } else if (!strcmp(stage_mem.input_buffer.mnemonic, "AND")) {
            // NO ACTION
            printf("***NO ACTION***\n");
        } else if (!strcmp(stage_mem.input_buffer.mnemonic, "EX-OR")) {
            // NO ACTION
            printf("***NO ACTION***\n");
        } else if (!strcmp(stage_mem.input_buffer.mnemonic, "JUMP")) {
            printf("JUMP operation\n");
        } else if (!strcmp(stage_mem.input_buffer.mnemonic, "JAL")) {
            printf("JAL operation\n");
        } else if (!strcmp(stage_mem.input_buffer.mnemonic, "HALT")) {
            printf("HALT operation\n");
        }
        output_buffer_memory();
    }

}

void writeback(void) {
    if (stage_mem.output_buffer.PC != 0) //Check if output buffer is allocated
    {
        //write the result in the dest register
        printf("\n***************************Write Back Stage*********************************\n");
        input_buffer_writeback();
        if (!strcmp(stage_wb.input_buffer.mnemonic, "NOP")) {
            printf("\nThe NOP instruction is being processed\n");
            return;
        } else if (!strcmp(stage_wb.input_buffer.mnemonic, "STORE")) {
            flags1.zero = FALSE;
            flags1.nonzero = TRUE;
            return;
        } else if (!strcmp(stage_wb.input_buffer.mnemonic, "MOVC")) {
            //stage_wb.input_buffer.dest_register.value=stage_wb.input_buffer.literal;
            register_file1.reg[stage_wb.input_buffer.dest_register.address].value = stage_wb.input_buffer.result;
            register_file1.reg[stage_wb.input_buffer.dest_register.address].address = stage_wb.input_buffer.dest_register.address;
            register_file1.reg[stage_wb.input_buffer.dest_register.address].status = TRUE;
            register_status = TRUE;
            if (stage_wb.input_buffer.result == 0) {
                flags1.zero = TRUE;
                flags1.nonzero = FALSE;
            } else
                flags1.zero = FALSE;
            flags1.nonzero = TRUE;
            //print data
            printf("R[%d]=%d\n", stage_wb.input_buffer.dest_register.address,
                   register_file1.reg[stage_wb.input_buffer.dest_register.address].value);
            printf("Register Status=%d\n", register_file1.reg[stage_wb.input_buffer.dest_register.address].status);

        } else if (!strcmp(stage_wb.input_buffer.mnemonic, "LOAD")) {
            //stage_wb.input_buffer.dest_register.value=stage_wb.input_buffer.target_memory_data;
            register_file1.reg[stage_wb.input_buffer.dest_register.address].value = stage_wb.input_buffer.target_memory_data;
            register_file1.reg[stage_wb.input_buffer.dest_register.address].address = stage_wb.input_buffer.dest_register.address;
            register_file1.reg[stage_wb.input_buffer.dest_register.address].status = TRUE;
            register_status = TRUE;
            if (stage_wb.input_buffer.target_memory_data == 0) {
                flags1.zero = TRUE;
            } else
                flags1.zero = FALSE;

            //print data
            printf("R[%d]=%d\n", stage_wb.input_buffer.dest_register.address,
                   register_file1.reg[stage_wb.input_buffer.dest_register.address].value);
            printf("Register Status=%d\n", register_file1.reg[stage_wb.input_buffer.dest_register.address].status);

        } else if (!strcmp(stage_wb.input_buffer.mnemonic, "MUL") || !strcmp(stage_wb.input_buffer.mnemonic, "DIV") ||
                   !strcmp(stage_wb.input_buffer.mnemonic, "ADD") ||
                   !strcmp(stage_wb.input_buffer.mnemonic, "SUB") || !strcmp(stage_wb.input_buffer.mnemonic, "OR") ||
                   !strcmp(stage_wb.input_buffer.mnemonic, "AND") || !strcmp(stage_wb.input_buffer.mnemonic, "EX-OR")) {
            //update the dest_register by storing the value of result in dest_register
            stage_wb.input_buffer.dest_register.value = stage_wb.input_buffer.result;
            register_file1.reg[stage_wb.input_buffer.dest_register.address].value = stage_wb.input_buffer.result;
            register_file1.reg[stage_wb.input_buffer.dest_register.address].address = stage_wb.input_buffer.dest_register.address;
            register_file1.reg[stage_wb.input_buffer.dest_register.address].status = TRUE;
            register_status = TRUE;
            if (stage_wb.input_buffer.result == 0) {
                flags1.zero = TRUE;
            } else
                flags1.zero = FALSE;
            //print data
            printf("R[%d]=%d\n", stage_wb.input_buffer.dest_register.address,
                   register_file1.reg[stage_wb.input_buffer.dest_register.address].value);
            printf("Register Status=%d\n", register_file1.reg[stage_wb.input_buffer.dest_register.address].status);
        } else if (!strcmp(stage_wb.input_buffer.mnemonic, "JUMP")) {
            printf("\n Do nothing\n");
        } else if (!strcmp(stage_wb.input_buffer.mnemonic, "JAL")) {
            printf("\n JAL Operation\n");
            stage_wb.input_buffer.dest_register.value = stage_wb.input_buffer.result;
            register_file1.reg[stage_wb.input_buffer.dest_register.address].value = (stage_wb.input_buffer.PC + 4);
            //((code_memory1.arr_code_line[stage_wb.input_buffer.dest_register.address].address) + 4);
            register_file1.reg[stage_wb.input_buffer.dest_register.address].address = stage_wb.input_buffer.dest_register.address;
            register_file1.reg[stage_wb.input_buffer.dest_register.address].status = TRUE;
            printf("R[%d]=%d\n", stage_wb.input_buffer.dest_register.address,
                   register_file1.reg[stage_wb.input_buffer.dest_register.address].value);
            printf("Register Status=%d\n", register_file1.reg[stage_wb.input_buffer.dest_register.address].status);
        }

        //display_values();
    }
}

void display_values(void) {
    //Register file
    printf("\n The values of the 16 registers are :\n");
    for (int i = 0; i < 16; ++i) {
        printf("R[%d]= %d \n", i, register_file1.reg[i].value);
        printf("STATUS = %d \n", register_file1.reg[i].status);
    }

}

void initialization(void) {
    //initializing the data_memory

    data_memory1.base_address = 0;
    memset(data_memory1.data_array, 0,
           sizeof(data_memory1.data_array)); // initialize all the location in memory to zero
    register_file1.dummy.value=-1000;
    //initializing register file
    for (int i = 0; i < 16; ++i) {
        register_file1.reg[i].value = 0;
        register_file1.reg[i].status = 1;
    }

    //RAT initialization
    for (int j = 0; j < 16; ++j) {
        RAT1[j].RorP = R;
        RAT1[j].index_R = j;
        RAT1[j].index_P=-1;
    }

    //initializing physical register file
    for (int k = 0; k < 32; ++k) {
        physical_RF.prf[k].address = k;
        physical_RF.prf[k].value = 0;
        // physical_RF.prf[k].status=VALID;
        physical_RF.prf[k].status = FREE;
    }
    //initializing the cycle
    stats1.cycle = 1;

    for (int l = 0; l < 32; ++l) {
        issue_queue_struct[l].clock_cycle=1000;
        issue_queue_struct[l].dest_index=0;
        issue_queue_struct[l].dest_RorP=-1;
        memset(issue_queue_struct[l].inst_opcode,0, sizeof(issue_queue_struct[l].inst_opcode));
        issue_queue_struct[l].LSQ_index=-1;
        issue_queue_struct[l].ROB_index=-1;
        memset(issue_queue_struct[l].original_instruction_string,0, sizeof(issue_queue_struct[l].original_instruction_string));
        issue_queue_struct[l].src1_index=-1;
        issue_queue_struct[l].src1_RorP=-1;
        issue_queue_struct[l].src1_status=-1;
        issue_queue_struct[l].src1_value=-1;
        issue_queue_struct[l].src2_index=-1;
        issue_queue_struct[l].src2_RorP=-1;
        issue_queue_struct[l].src2_status=-1;
        issue_queue_struct[l].src2_value=-1;
        issue_queue_struct[l].status_of_allocation=FREE;
        memset(issue_queue_struct[l].translated_instruction_string,0, sizeof(issue_queue_struct[l].translated_instruction_string));
        issue_queue_struct[l].wakeup=NO;

    }
    for (int m = 0; m <32 ; ++m) {
        issue_queue_selection.selection_logic[m]=issue_queue_struct[m];
        issue_queue_FU_selection.selection_FU[m]=issue_queue_struct[m];
    }
    //Initialize the status of stalling
    common.stalled = FALSE;
    stage_fetch.stalled = FALSE;
    stage_decode.stalled = FALSE;

}

void output_buffer_fetch(void) {
    if (flags1.BZ == TRUE) {
        strcpy(stage_fetch.output_buffer.instruction_string,
               code_memory1.arr_code_line[new_base_address].instruction_string);
        stage_fetch.output_buffer.PC = code_memory1.arr_code_line[new_base_address].address;
        printf("\nInstruction Address: %d\tCurrent Instruction: %s \n", stage_fetch.output_buffer.PC,
               stage_fetch.output_buffer.instruction_string);
    } else {
        //instruction string
        strcpy(stage_fetch.output_buffer.instruction_string,
               code_memory1.arr_code_line[counter_fetch].instruction_string);
        //instruction address
        stage_fetch.output_buffer.PC = code_memory1.arr_code_line[counter_fetch].address;
        counter_fetch++;
        printf("\nInstruction Address: %d\tCurrent Instruction: %s \n", stage_fetch.output_buffer.PC,
               stage_fetch.output_buffer.instruction_string);
        //display_values();
    }

}

void input_buffer_decode(void) {
    //input buffer of decode stage
    //copy instruction string into the input buffer of decode
    strcpy(stage_decode.input_buffer.instruction_string, stage_fetch.output_buffer.instruction_string);
    //copy instruction address into the input buffer of decode
    stage_decode.input_buffer.PC = stage_fetch.output_buffer.PC;
}

void output_buffer_decode(void) {
    //output buffer of decode stage
    //Copy the instruction information from input buffer of decode to output buffer of decode
    stage_decode.output_buffer.PC = stage_decode.input_buffer.PC;
    strcpy(stage_decode.output_buffer.instruction_string, stage_decode.input_buffer.instruction_string);
    strcpy(stage_decode.output_buffer.mnemonic, stage_decode.input_buffer.mnemonic);
    stage_decode.output_buffer.dest_register = stage_decode.input_buffer.dest_register;
    stage_decode.output_buffer.source_register1 = stage_decode.input_buffer.source_register1;
    stage_decode.output_buffer.source_register2 = stage_decode.input_buffer.source_register2;
    stage_decode.output_buffer.literal = stage_decode.input_buffer.literal;

    //delete data from the input buffer
    stage_decode.input_buffer.PC = 0;
    memset(stage_decode.input_buffer.instruction_string, 0, sizeof(stage_decode.input_buffer.instruction_string));
    memset(stage_decode.input_buffer.mnemonic, 0, sizeof(stage_decode.input_buffer.mnemonic));
    memset(&stage_decode.input_buffer.dest_register, 0, sizeof(stage_decode.input_buffer.dest_register));
    memset(&stage_decode.input_buffer.source_register1, 0, sizeof(stage_decode.input_buffer.source_register1));
    memset(&stage_decode.input_buffer.source_register2, 0, sizeof(stage_decode.input_buffer.source_register2));
    stage_decode.input_buffer.literal = 0;

}

void input_buffer_execute(void) {
    //input buffer of execute stage
    //Copy the instruction information from output buffer of decode to input buffer of execute
    stage_execute.input_buffer.selected_inst=stage_iq_intFU.output_buffer.selected_inst;
    stage_execute.input_buffer.PC = stage_iq_intFU.output_buffer.PC;
    strcpy(stage_execute.input_buffer.instruction_string, stage_iq_intFU.output_buffer.instruction_string);
    strcpy(stage_execute.input_buffer.mnemonic, stage_iq_intFU.output_buffer.mnemonic);
    stage_execute.input_buffer.dest_register = stage_iq_intFU.output_buffer.dest_register;
    stage_execute.input_buffer.source_register1 = stage_iq_intFU.output_buffer.source_register1;
    stage_execute.input_buffer.source_register2 = stage_iq_intFU.output_buffer.source_register2;
    stage_execute.input_buffer.literal = stage_iq_intFU.output_buffer.literal;
    stage_execute.input_buffer.result = stage_iq_intFU.output_buffer.result;
    stage_execute.input_buffer.target_memory_data = stage_iq_intFU.output_buffer.target_memory_data;
    stage_execute.input_buffer.target_memory_addr = stage_iq_intFU.output_buffer.target_memory_data;



    //empty the output buffer of decode stage
    stage_iq_intFU.output_buffer.PC = 0;
    memset(stage_iq_intFU.output_buffer.instruction_string, 0, sizeof(stage_iq_intFU.output_buffer.instruction_string));
    memset(stage_iq_intFU.output_buffer.mnemonic, 0, sizeof(stage_iq_intFU.output_buffer.mnemonic));
    memset(&stage_iq_intFU.output_buffer.dest_register, 0, sizeof(stage_iq_intFU.output_buffer.dest_register));
    memset(&stage_iq_intFU.output_buffer.source_register1, 0, sizeof(stage_iq_intFU.output_buffer.source_register1));
    memset(&stage_iq_intFU.output_buffer.source_register2, 0, sizeof(stage_iq_intFU.output_buffer.source_register2));
    stage_iq_intFU.output_buffer.literal = 0;
    stage_iq_intFU.output_buffer.result = 0;
    stage_iq_intFU.output_buffer.target_memory_data = 0;
    stage_iq_intFU.output_buffer.target_memory_addr = 0;
}

void output_buffer_execute(void) {
    //output buffer of execute stage
    //Copy the instruction information from input buffer of execute to output buffer of execute
    stage_execute.output_buffer.selected_inst=stage_execute.input_buffer.selected_inst;
    stage_execute.output_buffer.PC = stage_execute.input_buffer.PC;
    strcpy(stage_execute.output_buffer.instruction_string, stage_execute.input_buffer.instruction_string);
    strcpy(stage_execute.output_buffer.mnemonic, stage_execute.input_buffer.mnemonic);
    stage_execute.output_buffer.dest_register = stage_execute.input_buffer.dest_register;
    stage_execute.output_buffer.source_register1 = stage_execute.input_buffer.source_register1;
    stage_execute.output_buffer.source_register2 = stage_execute.input_buffer.source_register2;
    stage_execute.output_buffer.literal = stage_execute.input_buffer.literal;
    stage_execute.output_buffer.result = stage_execute.input_buffer.result;
    stage_execute.output_buffer.target_memory_addr = stage_execute.input_buffer.target_memory_addr;
    stage_execute.output_buffer.target_memory_data = stage_execute.input_buffer.target_memory_data;


    //delete data from the input buffer
    stage_execute.input_buffer.PC = 0;
    memset(stage_execute.input_buffer.instruction_string, 0, sizeof(stage_execute.input_buffer.instruction_string));
    memset(stage_execute.input_buffer.mnemonic, 0, sizeof(stage_execute.input_buffer.mnemonic));
    memset(&stage_execute.input_buffer.dest_register, 0, sizeof(stage_execute.input_buffer.dest_register));
    memset(&stage_execute.input_buffer.source_register1, 0, sizeof(stage_execute.input_buffer.source_register1));
    memset(&stage_execute.input_buffer.source_register2, 0, sizeof(stage_execute.input_buffer.source_register2));
    stage_execute.input_buffer.literal = 0;
    stage_execute.input_buffer.result = 0;
    stage_execute.input_buffer.target_memory_addr = 0;
    stage_execute.input_buffer.target_memory_data = 0;

}

void input_buffer_execute_multiplication1(void) {
    stage_mul1.input_buffer.selected_inst=stage_iq_MUL.output_buffer.selected_inst;
    stage_mul1.input_buffer.PC = stage_iq_MUL.output_buffer.PC;
    strcpy(stage_mul1.input_buffer.instruction_string, stage_iq_MUL.output_buffer.instruction_string);
    strcpy(stage_mul1.input_buffer.mnemonic, stage_iq_MUL.output_buffer.mnemonic);
    stage_mul1.input_buffer.dest_register = stage_iq_MUL.output_buffer.dest_register;
    stage_mul1.input_buffer.source_register1 = stage_iq_MUL.output_buffer.source_register1;
    stage_mul1.input_buffer.source_register2 = stage_iq_MUL.output_buffer.source_register2;
    stage_mul1.input_buffer.literal = stage_iq_MUL.output_buffer.literal;
    stage_mul1.input_buffer.result = stage_iq_MUL.output_buffer.result;
    stage_mul1.input_buffer.target_memory_data = stage_iq_MUL.output_buffer.target_memory_data;
    stage_mul1.input_buffer.target_memory_addr = stage_iq_MUL.output_buffer.target_memory_data;

    //empty the output buffer of decode stage
    stage_iq_MUL.output_buffer.PC = 0;
    memset(stage_iq_MUL.output_buffer.instruction_string, 0, sizeof(stage_iq_MUL.output_buffer.instruction_string));
    memset(stage_iq_MUL.output_buffer.mnemonic, 0, sizeof(stage_iq_MUL.output_buffer.mnemonic));
    memset(&stage_iq_MUL.output_buffer.dest_register, 0, sizeof(stage_iq_MUL.output_buffer.dest_register));
    memset(&stage_iq_MUL.output_buffer.source_register1, 0, sizeof(stage_iq_MUL.output_buffer.source_register1));
    memset(&stage_iq_MUL.output_buffer.source_register2, 0, sizeof(stage_iq_MUL.output_buffer.source_register2));
    stage_iq_MUL.output_buffer.literal = 0;
    stage_iq_MUL.output_buffer.result = 0;
    stage_iq_MUL.output_buffer.target_memory_data = 0;
    stage_iq_MUL.output_buffer.target_memory_addr = 0;
}

void input_buffer_execute_div1(void) {
    stage_div1.input_buffer.selected_inst=stage_iq_DIV.output_buffer.selected_inst;
    stage_div1.input_buffer.PC = stage_iq_DIV.output_buffer.PC;
    strcpy(stage_div1.input_buffer.instruction_string, stage_iq_DIV.output_buffer.instruction_string);
    strcpy(stage_div1.input_buffer.mnemonic, stage_iq_DIV.output_buffer.mnemonic);
    stage_div1.input_buffer.dest_register = stage_iq_DIV.output_buffer.dest_register;
    stage_div1.input_buffer.source_register1 = stage_iq_DIV.output_buffer.source_register1;
    stage_div1.input_buffer.source_register2 = stage_iq_DIV.output_buffer.source_register2;
    stage_div1.input_buffer.literal = stage_iq_DIV.output_buffer.literal;
    stage_div1.input_buffer.result = stage_iq_DIV.output_buffer.result;
    stage_div1.input_buffer.target_memory_data = stage_iq_DIV.output_buffer.target_memory_data;
    stage_div1.input_buffer.target_memory_addr = stage_iq_DIV.output_buffer.target_memory_data;

    //empty the output buffer of decode stage
    stage_iq_DIV.output_buffer.PC = 0;
    memset(stage_iq_DIV.output_buffer.instruction_string, 0, sizeof(stage_iq_DIV.output_buffer.instruction_string));
    memset(stage_iq_DIV.output_buffer.mnemonic, 0, sizeof(stage_iq_DIV.output_buffer.mnemonic));
    memset(&stage_iq_DIV.output_buffer.dest_register, 0, sizeof(stage_iq_DIV.output_buffer.dest_register));
    memset(&stage_iq_DIV.output_buffer.source_register1, 0, sizeof(stage_iq_DIV.output_buffer.source_register1));
    memset(&stage_iq_DIV.output_buffer.source_register2, 0, sizeof(stage_iq_DIV.output_buffer.source_register2));
    stage_iq_DIV.output_buffer.literal = 0;
    stage_iq_DIV.output_buffer.result = 0;
    stage_iq_DIV.output_buffer.target_memory_data = 0;
    stage_iq_DIV.output_buffer.target_memory_addr = 0;
}

void output_buffer_execute_div1(void) {
    stage_div1.output_buffer.selected_inst=stage_div1.input_buffer.selected_inst;
    stage_div1.output_buffer.PC = stage_div1.input_buffer.PC;
    strcpy(stage_div1.output_buffer.instruction_string, stage_div1.input_buffer.instruction_string);
    strcpy(stage_div1.output_buffer.mnemonic, stage_div1.input_buffer.mnemonic);
    stage_div1.output_buffer.dest_register = stage_div1.input_buffer.dest_register;
    stage_div1.output_buffer.source_register1 = stage_div1.input_buffer.source_register1;
    stage_div1.output_buffer.source_register2 = stage_div1.input_buffer.source_register2;
    stage_div1.output_buffer.literal = stage_div1.input_buffer.literal;
    stage_div1.output_buffer.result = stage_div1.input_buffer.result;
    stage_div1.output_buffer.target_memory_addr = stage_div1.input_buffer.target_memory_addr;
    stage_div1.output_buffer.target_memory_data = stage_div1.input_buffer.target_memory_data;



    //delete data from the input buffer
    stage_div1.input_buffer.PC = 0;
    memset(stage_div1.input_buffer.instruction_string, 0, sizeof(stage_div1.input_buffer.instruction_string));
    memset(stage_div1.input_buffer.mnemonic, 0, sizeof(stage_div1.input_buffer.mnemonic));
    memset(&stage_div1.input_buffer.dest_register, 0, sizeof(stage_div1.input_buffer.dest_register));
    memset(&stage_div1.input_buffer.source_register1, 0, sizeof(stage_div1.input_buffer.source_register1));
    memset(&stage_div1.input_buffer.source_register2, 0, sizeof(stage_div1.input_buffer.source_register2));
    stage_div1.input_buffer.literal = 0;
    stage_div1.input_buffer.result = 0;
    stage_div1.input_buffer.target_memory_addr = 0;
    stage_div1.input_buffer.target_memory_data = 0;
}

void input_buffer_execute_div2(void) {
    stage_div2.input_buffer.selected_inst=stage_div1.output_buffer.selected_inst;
    stage_div2.input_buffer.PC = stage_div1.output_buffer.PC;
    strcpy(stage_div2.input_buffer.instruction_string, stage_div1.output_buffer.instruction_string);
    strcpy(stage_div2.input_buffer.mnemonic, stage_div1.output_buffer.mnemonic);
    stage_div2.input_buffer.dest_register = stage_div1.output_buffer.dest_register;
    stage_div2.input_buffer.source_register1 = stage_div1.output_buffer.source_register1;
    stage_div2.input_buffer.source_register2 = stage_div1.output_buffer.source_register2;
    stage_div2.input_buffer.literal = stage_div1.output_buffer.literal;
    stage_div2.input_buffer.result = stage_div1.output_buffer.result;
    stage_div2.input_buffer.target_memory_data = stage_div1.output_buffer.target_memory_data;
    stage_div2.input_buffer.target_memory_addr = stage_div1.output_buffer.target_memory_data;

    //empty the output buffer of decode stage
    stage_div1.output_buffer.PC = 0;
    memset(stage_div1.output_buffer.instruction_string, 0, sizeof(stage_div1.output_buffer.instruction_string));
    memset(stage_div1.output_buffer.mnemonic, 0, sizeof(stage_div1.output_buffer.mnemonic));
    memset(&stage_div1.output_buffer.dest_register, 0, sizeof(stage_div1.output_buffer.dest_register));
    memset(&stage_div1.output_buffer.source_register1, 0, sizeof(stage_div1.output_buffer.source_register1));
    memset(&stage_div1.output_buffer.source_register2, 0, sizeof(stage_div1.output_buffer.source_register2));
    stage_div1.output_buffer.literal = 0;
    stage_div1.output_buffer.result = 0;
    stage_div1.output_buffer.target_memory_data = 0;
    stage_div1.output_buffer.target_memory_addr = 0;
}

void output_buffer_execute_div2(void) {
    stage_div2.output_buffer.selected_inst=stage_div2.input_buffer.selected_inst;
    stage_div2.output_buffer.PC = stage_div2.input_buffer.PC;
    strcpy(stage_div2.output_buffer.instruction_string, stage_div2.input_buffer.instruction_string);
    strcpy(stage_div2.output_buffer.mnemonic, stage_div2.input_buffer.mnemonic);
    stage_div2.output_buffer.dest_register = stage_div2.input_buffer.dest_register;
    stage_div2.output_buffer.source_register1 = stage_div2.input_buffer.source_register1;
    stage_div2.output_buffer.source_register2 = stage_div2.input_buffer.source_register2;
    stage_div2.output_buffer.literal = stage_div2.input_buffer.literal;
    stage_div2.output_buffer.result = stage_div2.input_buffer.result;
    stage_div2.output_buffer.target_memory_addr = stage_div2.input_buffer.target_memory_addr;
    stage_div2.output_buffer.target_memory_data = stage_div2.input_buffer.target_memory_data;



    //delete data from the input buffer
    stage_div2.input_buffer.PC = 0;
    memset(stage_div2.input_buffer.instruction_string, 0, sizeof(stage_div2.input_buffer.instruction_string));
    memset(stage_div2.input_buffer.mnemonic, 0, sizeof(stage_div2.input_buffer.mnemonic));
    memset(&stage_div2.input_buffer.dest_register, 0, sizeof(stage_div2.input_buffer.dest_register));
    memset(&stage_div2.input_buffer.source_register1, 0, sizeof(stage_div2.input_buffer.source_register1));
    memset(&stage_div2.input_buffer.source_register2, 0, sizeof(stage_div2.input_buffer.source_register2));
    stage_div2.input_buffer.literal = 0;
    stage_div2.input_buffer.result = 0;
    stage_div2.input_buffer.target_memory_addr = 0;
    stage_div2.input_buffer.target_memory_data = 0;
}

void input_buffer_execute_div3(void) {
    stage_div3.input_buffer.selected_inst=stage_div2.output_buffer.selected_inst;
    stage_div3.input_buffer.PC = stage_div2.output_buffer.PC;
    strcpy(stage_div3.input_buffer.instruction_string, stage_div2.output_buffer.instruction_string);
    strcpy(stage_div3.input_buffer.mnemonic, stage_div2.output_buffer.mnemonic);
    stage_div3.input_buffer.dest_register = stage_div2.output_buffer.dest_register;
    stage_div3.input_buffer.source_register1 = stage_div2.output_buffer.source_register1;
    stage_div3.input_buffer.source_register2 = stage_div2.output_buffer.source_register2;
    stage_div3.input_buffer.literal = stage_div2.output_buffer.literal;
    stage_div3.input_buffer.result = stage_div2.output_buffer.result;
    stage_div3.input_buffer.target_memory_data = stage_div2.output_buffer.target_memory_data;
    stage_div3.input_buffer.target_memory_addr = stage_div2.output_buffer.target_memory_data;

    //empty the output buffer of decode stage
    stage_div2.output_buffer.PC = 0;
    memset(stage_div2.output_buffer.instruction_string, 0, sizeof(stage_div2.output_buffer.instruction_string));
    memset(stage_div2.output_buffer.mnemonic, 0, sizeof(stage_div2.output_buffer.mnemonic));
    memset(&stage_div2.output_buffer.dest_register, 0, sizeof(stage_div2.output_buffer.dest_register));
    memset(&stage_div2.output_buffer.source_register1, 0, sizeof(stage_div2.output_buffer.source_register1));
    memset(&stage_div2.output_buffer.source_register2, 0, sizeof(stage_div2.output_buffer.source_register2));
    stage_div2.output_buffer.literal = 0;
    stage_div2.output_buffer.result = 0;
    stage_div2.output_buffer.target_memory_data = 0;
    stage_div2.output_buffer.target_memory_addr = 0;
}

void output_buffer_execute_div3(void) {
    stage_div3.output_buffer.selected_inst=stage_div3.input_buffer.selected_inst;
    stage_div3.output_buffer.PC = stage_div3.input_buffer.PC;
    strcpy(stage_div3.output_buffer.instruction_string, stage_div3.input_buffer.instruction_string);
    strcpy(stage_div3.output_buffer.mnemonic, stage_div3.input_buffer.mnemonic);
    stage_div3.output_buffer.dest_register = stage_div3.input_buffer.dest_register;
    stage_div3.output_buffer.source_register1 = stage_div3.input_buffer.source_register1;
    stage_div3.output_buffer.source_register2 = stage_div3.input_buffer.source_register2;
    stage_div3.output_buffer.literal = stage_div3.input_buffer.literal;
    stage_div3.output_buffer.result = stage_div3.input_buffer.result;
    stage_div3.output_buffer.target_memory_addr = stage_div3.input_buffer.target_memory_addr;
    stage_div3.output_buffer.target_memory_data = stage_div3.input_buffer.target_memory_data;

    //delete data from the input buffer
    stage_div3.input_buffer.PC = 0;
    memset(stage_div3.input_buffer.instruction_string, 0, sizeof(stage_div3.input_buffer.instruction_string));
    memset(stage_div3.input_buffer.mnemonic, 0, sizeof(stage_div3.input_buffer.mnemonic));
    memset(&stage_div3.input_buffer.dest_register, 0, sizeof(stage_div3.input_buffer.dest_register));
    memset(&stage_div3.input_buffer.source_register1, 0, sizeof(stage_div3.input_buffer.source_register1));
    memset(&stage_div3.input_buffer.source_register2, 0, sizeof(stage_div3.input_buffer.source_register2));
    stage_div3.input_buffer.literal = 0;
    stage_div3.input_buffer.result = 0;
    stage_div3.input_buffer.target_memory_addr = 0;
    stage_div3.input_buffer.target_memory_data = 0;
}

void input_buffer_execute_div4(void) {
    stage_div4.input_buffer.selected_inst=stage_div3.output_buffer.selected_inst;
    stage_div4.input_buffer.PC = stage_div3.output_buffer.PC;
    strcpy(stage_div4.input_buffer.instruction_string, stage_div3.output_buffer.instruction_string);
    strcpy(stage_div4.input_buffer.mnemonic, stage_div3.output_buffer.mnemonic);
    stage_div4.input_buffer.dest_register = stage_div3.output_buffer.dest_register;
    stage_div4.input_buffer.source_register1 = stage_div3.output_buffer.source_register1;
    stage_div4.input_buffer.source_register2 = stage_div3.output_buffer.source_register2;
    stage_div4.input_buffer.literal = stage_div3.output_buffer.literal;
    stage_div4.input_buffer.result = stage_div3.output_buffer.result;
    stage_div4.input_buffer.target_memory_data = stage_div3.output_buffer.target_memory_data;
    stage_div4.input_buffer.target_memory_addr = stage_div3.output_buffer.target_memory_data;

    //empty the output buffer of decode stage
    stage_div3.output_buffer.PC = 0;
    memset(stage_div3.output_buffer.instruction_string, 0, sizeof(stage_div3.output_buffer.instruction_string));
    memset(stage_div3.output_buffer.mnemonic, 0, sizeof(stage_div3.output_buffer.mnemonic));
    memset(&stage_div3.output_buffer.dest_register, 0, sizeof(stage_div3.output_buffer.dest_register));
    memset(&stage_div3.output_buffer.source_register1, 0, sizeof(stage_div3.output_buffer.source_register1));
    memset(&stage_div3.output_buffer.source_register2, 0, sizeof(stage_div3.output_buffer.source_register2));
    stage_div3.output_buffer.literal = 0;
    stage_div3.output_buffer.result = 0;
    stage_div3.output_buffer.target_memory_data = 0;
    stage_div3.output_buffer.target_memory_addr = 0;
}

void output_buffer_execute_div4(void) {
    stage_div4.output_buffer.selected_inst=stage_div4.input_buffer.selected_inst;
    stage_div4.output_buffer.PC = stage_div4.input_buffer.PC;
    strcpy(stage_div4.output_buffer.instruction_string, stage_div4.input_buffer.instruction_string);
    strcpy(stage_div4.output_buffer.mnemonic, stage_div4.input_buffer.mnemonic);
    stage_div4.output_buffer.dest_register = stage_div4.input_buffer.dest_register;
    stage_div4.output_buffer.source_register1 = stage_div4.input_buffer.source_register1;
    stage_div4.output_buffer.source_register2 = stage_div4.input_buffer.source_register2;
    stage_div4.output_buffer.literal = stage_div4.input_buffer.literal;
    stage_div4.output_buffer.result = stage_div4.input_buffer.result;
    stage_div4.output_buffer.target_memory_addr = stage_div4.input_buffer.target_memory_addr;
    stage_div4.output_buffer.target_memory_data = stage_div4.input_buffer.target_memory_data;

    //delete data from the input buffer
    stage_div4.input_buffer.PC = 0;
    memset(stage_div4.input_buffer.instruction_string, 0, sizeof(stage_div4.input_buffer.instruction_string));
    memset(stage_div4.input_buffer.mnemonic, 0, sizeof(stage_div4.input_buffer.mnemonic));
    memset(&stage_div4.input_buffer.dest_register, 0, sizeof(stage_div4.input_buffer.dest_register));
    memset(&stage_div4.input_buffer.source_register1, 0, sizeof(stage_div4.input_buffer.source_register1));
    memset(&stage_div4.input_buffer.source_register2, 0, sizeof(stage_div4.input_buffer.source_register2));
    stage_div4.input_buffer.literal = 0;
    stage_div4.input_buffer.result = 0;
    stage_div4.input_buffer.target_memory_addr = 0;
    stage_div4.input_buffer.target_memory_data = 0;
}

void input_buffer_memory_div(void) {
    stage_div1.output_buffer.selected_inst=stage_div1.input_buffer.selected_inst;
    stage_mem.input_buffer.PC = stage_div4.output_buffer.PC;
    strcpy(stage_mem.input_buffer.instruction_string, stage_div4.output_buffer.instruction_string);
    strcpy(stage_mem.input_buffer.mnemonic, stage_div4.output_buffer.mnemonic);
    stage_mem.input_buffer.dest_register = stage_div4.output_buffer.dest_register;
    stage_mem.input_buffer.source_register1 = stage_div4.output_buffer.source_register1;
    stage_mem.input_buffer.source_register2 = stage_div4.output_buffer.source_register2;
    stage_mem.input_buffer.literal = stage_div4.output_buffer.literal;
    stage_mem.input_buffer.target_memory_data = stage_div4.output_buffer.target_memory_data;
    stage_mem.input_buffer.result = stage_div4.output_buffer.result;
    stage_mem.input_buffer.target_memory_addr = stage_div4.output_buffer.target_memory_addr;

    //empty the output buffer of execute stage
    stage_div4.output_buffer.PC = 0;
    memset(stage_div4.output_buffer.instruction_string, 0, sizeof(stage_div4.output_buffer.instruction_string));
    memset(stage_div4.output_buffer.mnemonic, 0, sizeof(stage_div4.output_buffer.mnemonic));
    memset(&stage_div4.output_buffer.dest_register, 0, sizeof(stage_div4.output_buffer.dest_register));
    memset(&stage_div4.output_buffer.source_register1, 0, sizeof(stage_div4.output_buffer.source_register1));
    memset(&stage_div4.output_buffer.source_register2, 0, sizeof(stage_div4.output_buffer.source_register2));
    stage_div4.output_buffer.literal = 0;
    stage_div4.output_buffer.result = 0;
    stage_div4.output_buffer.target_memory_addr = 0;
    stage_div4.output_buffer.target_memory_data = 0;
}

void output_buffer_execute_multiplication1(void) {
    stage_mul1.output_buffer.PC = stage_mul1.input_buffer.PC;
    strcpy(stage_mul1.output_buffer.instruction_string, stage_mul1.input_buffer.instruction_string);
    strcpy(stage_mul1.output_buffer.mnemonic, stage_mul1.input_buffer.mnemonic);
    stage_mul1.output_buffer.dest_register = stage_mul1.input_buffer.dest_register;
    stage_mul1.output_buffer.source_register1 = stage_mul1.input_buffer.source_register1;
    stage_mul1.output_buffer.source_register2 = stage_mul1.input_buffer.source_register2;
    stage_mul1.output_buffer.literal = stage_mul1.input_buffer.literal;
    stage_mul1.output_buffer.result = stage_mul1.input_buffer.result;
    stage_mul1.output_buffer.target_memory_addr = stage_mul1.input_buffer.target_memory_addr;
    stage_mul1.output_buffer.target_memory_data = stage_mul1.input_buffer.target_memory_data;



    //delete data from the input buffer
    stage_mul1.input_buffer.PC = 0;
    memset(stage_mul1.input_buffer.instruction_string, 0, sizeof(stage_mul1.input_buffer.instruction_string));
    memset(stage_mul1.input_buffer.mnemonic, 0, sizeof(stage_mul1.input_buffer.mnemonic));
    memset(&stage_mul1.input_buffer.dest_register, 0, sizeof(stage_mul1.input_buffer.dest_register));
    memset(&stage_mul1.input_buffer.source_register1, 0, sizeof(stage_mul1.input_buffer.source_register1));
    memset(&stage_mul1.input_buffer.source_register2, 0, sizeof(stage_mul1.input_buffer.source_register2));
    stage_mul1.input_buffer.literal = 0;
    stage_mul1.input_buffer.result = 0;
    stage_mul1.input_buffer.target_memory_addr = 0;
    stage_mul1.input_buffer.target_memory_data = 0;
}

void input_buffer_execute_multiplication2(void) {
    stage_mul2.input_buffer.PC = stage_mul1.output_buffer.PC;
    strcpy(stage_mul2.input_buffer.instruction_string, stage_mul1.output_buffer.instruction_string);
    strcpy(stage_mul2.input_buffer.mnemonic, stage_mul1.output_buffer.mnemonic);
    stage_mul2.input_buffer.dest_register = stage_mul1.output_buffer.dest_register;
    stage_mul2.input_buffer.source_register1 = stage_mul1.output_buffer.source_register1;
    stage_mul2.input_buffer.source_register2 = stage_mul1.output_buffer.source_register2;
    stage_mul2.input_buffer.literal = stage_mul1.output_buffer.literal;
    stage_mul2.input_buffer.result = stage_mul1.output_buffer.result;
    stage_mul2.input_buffer.target_memory_data = stage_mul1.output_buffer.target_memory_data;
    stage_mul2.input_buffer.target_memory_addr = stage_mul1.output_buffer.target_memory_data;

    //empty the output buffer of decode stage
    stage_mul1.output_buffer.PC = 0;
    memset(stage_mul1.output_buffer.instruction_string, 0, sizeof(stage_mul1.output_buffer.instruction_string));
    memset(stage_mul1.output_buffer.mnemonic, 0, sizeof(stage_mul1.output_buffer.mnemonic));
    memset(&stage_mul1.output_buffer.dest_register, 0, sizeof(stage_mul1.output_buffer.dest_register));
    memset(&stage_mul1.output_buffer.source_register1, 0, sizeof(stage_mul1.output_buffer.source_register1));
    memset(&stage_mul1.output_buffer.source_register2, 0, sizeof(stage_mul1.output_buffer.source_register2));
    stage_mul1.output_buffer.literal = 0;
    stage_mul1.output_buffer.result = 0;
    stage_mul1.output_buffer.target_memory_data = 0;
    stage_mul1.output_buffer.target_memory_addr = 0;
}

void output_buffer_execute_multiplication2(void) {
    stage_mul2.output_buffer.PC = stage_mul2.input_buffer.PC;
    strcpy(stage_mul2.output_buffer.instruction_string, stage_mul2.input_buffer.instruction_string);
    strcpy(stage_mul2.output_buffer.mnemonic, stage_mul2.input_buffer.mnemonic);
    stage_mul2.output_buffer.dest_register = stage_mul2.input_buffer.dest_register;
    stage_mul2.output_buffer.source_register1 = stage_mul2.input_buffer.source_register1;
    stage_mul2.output_buffer.source_register2 = stage_mul2.input_buffer.source_register2;
    stage_mul2.output_buffer.literal = stage_mul2.input_buffer.literal;
    stage_mul2.output_buffer.result = stage_mul2.input_buffer.result;
    stage_mul2.output_buffer.target_memory_addr = stage_mul2.input_buffer.target_memory_addr;
    stage_mul2.output_buffer.target_memory_data = stage_mul2.input_buffer.target_memory_data;

    //delete data from the input buffer
    stage_mul2.input_buffer.PC = 0;
    memset(stage_mul2.input_buffer.instruction_string, 0, sizeof(stage_mul2.input_buffer.instruction_string));
    memset(stage_mul2.input_buffer.mnemonic, 0, sizeof(stage_mul2.input_buffer.mnemonic));
    memset(&stage_mul2.input_buffer.dest_register, 0, sizeof(stage_mul2.input_buffer.dest_register));
    memset(&stage_mul2.input_buffer.source_register1, 0, sizeof(stage_mul2.input_buffer.source_register1));
    memset(&stage_mul2.input_buffer.source_register2, 0, sizeof(stage_mul2.input_buffer.source_register2));
    stage_mul2.input_buffer.literal = 0;
    stage_mul2.input_buffer.result = 0;
    stage_mul2.input_buffer.target_memory_addr = 0;
    stage_mul2.input_buffer.target_memory_data = 0;
}

void input_buffer_memory_mul(void) {
    stage_mem.input_buffer.PC = stage_mul2.output_buffer.PC;
    strcpy(stage_mem.input_buffer.instruction_string, stage_mul2.output_buffer.instruction_string);
    strcpy(stage_mem.input_buffer.mnemonic, stage_mul2.output_buffer.mnemonic);
    stage_mem.input_buffer.dest_register = stage_mul2.output_buffer.dest_register;
    stage_mem.input_buffer.source_register1 = stage_mul2.output_buffer.source_register1;
    stage_mem.input_buffer.source_register2 = stage_mul2.output_buffer.source_register2;
    stage_mem.input_buffer.literal = stage_mul2.output_buffer.literal;
    stage_mem.input_buffer.target_memory_data = stage_mul2.output_buffer.target_memory_data;
    stage_mem.input_buffer.result = stage_mul2.output_buffer.result;
    stage_mem.input_buffer.target_memory_addr = stage_mul2.output_buffer.target_memory_addr;

    //empty the output buffer of execute stage
    stage_mul2.output_buffer.PC = 0;
    memset(stage_mul2.output_buffer.instruction_string, 0, sizeof(stage_mul2.output_buffer.instruction_string));
    memset(stage_mul2.output_buffer.mnemonic, 0, sizeof(stage_mul2.output_buffer.mnemonic));
    memset(&stage_mul2.output_buffer.dest_register, 0, sizeof(stage_mul2.output_buffer.dest_register));
    memset(&stage_mul2.output_buffer.source_register1, 0, sizeof(stage_mul2.output_buffer.source_register1));
    memset(&stage_mul2.output_buffer.source_register2, 0, sizeof(stage_mul2.output_buffer.source_register2));
    stage_mul2.output_buffer.literal = 0;
    stage_mul2.output_buffer.result = 0;
    stage_mul2.output_buffer.target_memory_addr = 0;
    stage_mul2.output_buffer.target_memory_data = 0;
}

void input_buffer_memory_exec(void) {
    //input buffer of memory stage
    //Copy the instruction information from output buffer of execute to input buffer of memory
    stage_mem.input_buffer.PC = stage_execute.output_buffer.PC;
    strcpy(stage_mem.input_buffer.instruction_string, stage_execute.output_buffer.instruction_string);
    strcpy(stage_mem.input_buffer.mnemonic, stage_execute.output_buffer.mnemonic);
    stage_mem.input_buffer.dest_register = stage_execute.output_buffer.dest_register;
    stage_mem.input_buffer.source_register1 = stage_execute.output_buffer.source_register1;
    stage_mem.input_buffer.source_register2 = stage_execute.output_buffer.source_register2;
    stage_mem.input_buffer.literal = stage_execute.output_buffer.literal;
    stage_mem.input_buffer.target_memory_data = stage_execute.output_buffer.target_memory_data;
    stage_mem.input_buffer.result = stage_execute.output_buffer.result;
    stage_mem.input_buffer.target_memory_addr = stage_execute.output_buffer.target_memory_addr;

    //empty the output buffer of execute stage
    stage_execute.output_buffer.PC = 0;
    memset(stage_execute.output_buffer.instruction_string, 0, sizeof(stage_execute.output_buffer.instruction_string));
    memset(stage_execute.output_buffer.mnemonic, 0, sizeof(stage_execute.output_buffer.mnemonic));
    memset(&stage_execute.output_buffer.dest_register, 0, sizeof(stage_execute.output_buffer.dest_register));
    memset(&stage_execute.output_buffer.source_register1, 0, sizeof(stage_execute.output_buffer.source_register1));
    memset(&stage_execute.output_buffer.source_register2, 0, sizeof(stage_execute.output_buffer.source_register2));
    stage_execute.output_buffer.literal = 0;
    stage_execute.output_buffer.result = 0;
    stage_execute.output_buffer.target_memory_addr = 0;
    stage_execute.output_buffer.target_memory_data = 0;

}

void output_buffer_memory(void) {
    //output buffer of memory stage
    //Copy the instruction information from input buffer of memory to output buffer of memory
    stage_mem.output_buffer.PC = stage_mem.input_buffer.PC;
    strcpy(stage_mem.output_buffer.instruction_string, stage_mem.input_buffer.instruction_string);
    strcpy(stage_mem.output_buffer.mnemonic, stage_mem.input_buffer.mnemonic);
    stage_mem.output_buffer.dest_register = stage_mem.input_buffer.dest_register;
    stage_mem.output_buffer.source_register1 = stage_mem.input_buffer.source_register1;
    stage_mem.output_buffer.source_register2 = stage_mem.input_buffer.source_register2;
    stage_mem.output_buffer.literal = stage_mem.input_buffer.literal;
    stage_mem.output_buffer.result = stage_mem.input_buffer.result;
    stage_mem.output_buffer.target_memory_addr = stage_mem.input_buffer.target_memory_addr;
    stage_mem.output_buffer.target_memory_data = stage_mem.input_buffer.target_memory_data;


    //empty the input buffer of memory stage
    stage_mem.input_buffer.PC = 0;
    memset(stage_mem.input_buffer.instruction_string, 0, sizeof(stage_mem.input_buffer.instruction_string));
    memset(stage_mem.input_buffer.mnemonic, 0, sizeof(stage_mem.input_buffer.mnemonic));
    memset(&stage_mem.input_buffer.dest_register, 0, sizeof(stage_mem.input_buffer.dest_register));
    memset(&stage_mem.input_buffer.source_register1, 0, sizeof(stage_mem.input_buffer.source_register1));
    memset(&stage_mem.input_buffer.source_register2, 0, sizeof(stage_mem.input_buffer.source_register2));
    stage_mem.input_buffer.target_memory_addr = 0;
    stage_mem.input_buffer.target_memory_data = 0;
    stage_mem.input_buffer.literal = 0;
    stage_mem.input_buffer.result = 0;

}

void input_buffer_writeback(void) {
    //input buffer of write back stage
    //Copy the instruction information from input buffer of memory to output buffer of memory
    stage_wb.input_buffer.PC = stage_mem.output_buffer.PC;
    strcpy(stage_wb.input_buffer.instruction_string, stage_mem.output_buffer.instruction_string);
    strcpy(stage_wb.input_buffer.mnemonic, stage_mem.output_buffer.mnemonic);
    stage_wb.input_buffer.dest_register = stage_mem.output_buffer.dest_register;
    stage_wb.input_buffer.source_register1 = stage_mem.output_buffer.source_register1;
    stage_wb.input_buffer.source_register2 = stage_mem.output_buffer.source_register2;
    stage_wb.input_buffer.literal = stage_mem.output_buffer.literal;
    stage_wb.input_buffer.result = stage_mem.output_buffer.result;
    stage_wb.input_buffer.target_memory_addr = stage_mem.output_buffer.target_memory_addr;
    stage_wb.input_buffer.target_memory_data = stage_mem.output_buffer.target_memory_data;

    //empty the input buffer of memory stage
    stage_mem.output_buffer.PC = 0;
    memset(stage_mem.output_buffer.instruction_string, 0, sizeof(stage_mem.output_buffer.instruction_string));
    memset(stage_mem.output_buffer.mnemonic, 0, sizeof(stage_mem.output_buffer.mnemonic));
    memset(&stage_mem.output_buffer.dest_register, 0, sizeof(stage_mem.output_buffer.dest_register));
    memset(&stage_mem.output_buffer.source_register1, 0, sizeof(stage_mem.output_buffer.source_register1));
    memset(&stage_mem.output_buffer.source_register2, 0, sizeof(stage_mem.output_buffer.source_register2));
    stage_mem.output_buffer.target_memory_addr = 0;
    stage_mem.output_buffer.target_memory_data = 0;
    stage_mem.output_buffer.literal = 0;
    stage_mem.output_buffer.result = 0;

}

void decode_stage_operations(void) {
    int reg_address1 = 0;
    int reg_address2 = 0;
    int reg_address3 = 0;
    int literal = 0;
    int a = 0;

    char *str1, *str2, *str3, *str4, *str5;
    int n = 0;
    char string_copy1[2], string_copy2[2];
    memset(string_copy1, 0, sizeof(string_copy1));
    memset(string_copy2, 0, sizeof(string_copy2));
    char string[25];
    strcpy(string, stage_decode.input_buffer.instruction_string);
    while (string[a] != 0) {
        if (string[a] == ',')
            string[a] = ' ';
        a++;
    }
    str1 = strtok(string, " ");
    n++;
    //printf("the string1 is%s\n", str1);

    while (n < 5 && string[n] != 0) {
        str2 = strtok(NULL, " ");
        if (str2 == NULL)
            break;
        else n++;
        str3 = strtok(NULL, " ");
        if (str3 == NULL)
            break;
        else n++;
        str4 = strtok(NULL, " ");
        if (str4 == NULL)
            break;
        else n++;
        str5 = strtok(NULL, " ");
        if (str5 == NULL)
            break;
        else n++;
    }
    strcpy(stage_decode.input_buffer.mnemonic, str1);
    printf("\n The mnemonic is  : %s\n", stage_decode.input_buffer.mnemonic);


    //compare the string to get the exact inst
    if (!strcmp(stage_decode.input_buffer.mnemonic, "LOAD") || !strcmp(stage_decode.input_buffer.mnemonic, "ADD") ||
        !strcmp(stage_decode.input_buffer.mnemonic, "SUB")) {
        //Source Register 1
        strncpy(string_copy1, str3, 1);
        if (string_copy1[0] == 'R') {
            sscanf(str3, "R%d", &reg_address2); // The register number is stored in the variable reg_number

            printf("The source register is : R%d\n", reg_address2);
            //fetch_stage.input_buffer.source_register1=register_file1.reg[greg_number];
            //update the register in

            for (int i = 0; i < 16; ++i) {
                if (RAT1[i].index_R == reg_address2 && RAT1[i].RorP == R) {
                    stage_decode.input_buffer.source_register1 = register_file1.reg[reg_address2];
                    stage_decode.input_buffer.source_register1.address = reg_address2;
                    stage_decode.input_buffer.validity = TRUE;
                    stage_decode.input_buffer.src1_RorP = R;
                    break;
                } else if (RAT1[i].index_R == reg_address2 && RAT1[i].RorP == P) {
                    stage_decode.input_buffer.source_register1 = physical_RF.prf[RAT1[i].index_P];
                    stage_decode.input_buffer.source_register1.address = RAT1[i].index_P;
                    stage_decode.input_buffer.validity = TRUE;
                    stage_decode.input_buffer.src1_RorP = P;
                    break;
                }
            }
            if(stage_decode.input_buffer.validity!=TRUE) {
                if (bus1.register_address == reg_address2) {
                    stage_decode.input_buffer.source_register1.value = bus1.result;
                    stage_decode.input_buffer.source_register1.address = reg_address2;
                    stage_decode.input_buffer.validity=TRUE;
                    stage_decode.input_buffer.src1_RorP=P;
                    printf("Result copied through forwarding bus\n");
                    //free forward bus
                    bus1.result = -1;
                    bus1.register_address = -1;
                } else if (bus2.register_address == reg_address2) {
                    stage_decode.input_buffer.source_register1.value = bus2.result;
                    stage_decode.input_buffer.source_register1.address = reg_address2;
                    stage_decode.input_buffer.validity=TRUE;
                    stage_decode.input_buffer.src1_RorP=P;
                    printf("Result copied through forwarding bus\n");
                    //free forward bus
                    bus2.result = -1;
                    bus2.register_address = -1;
                } else if (bus3.register_address == reg_address2) {
                    stage_decode.input_buffer.source_register1.value = bus3.result;
                    stage_decode.input_buffer.source_register1.address = reg_address2;
                    stage_decode.input_buffer.validity=TRUE;
                    stage_decode.input_buffer.src1_RorP=P;
                    printf("Result copied through forwarding bus\n");
                    //free forward bus
                    bus3.result = -1;
                    bus3.register_address = -1;
                } else {
                    stage_decode.input_buffer.source_register1 = register_file1.dummy;
                    stage_decode.input_buffer.validity=FALSE;
                    stage_decode.input_buffer.src1_RorP=-1;
                    stage_decode.input_buffer.source_register1.address = reg_address2;
                }
            }




//           // stage_decode.input_buffer.source_register1 = register_file1.reg[reg_address2];
//            //stage_decode.input_buffer.source_register1.address = reg_address2;
//           // register_file1.reg[reg_address2].address = reg_address2;
//            if (register_file1.reg[reg_address2].status == FALSE) {
//                if (bus1.register_address == register_file1.reg[reg_address2].address) {
//                    stage_decode.input_buffer.source_register1.value = bus1.result;
//                    printf("Result copied through forwarding bus\n");
//                    //free forward bus
//                    bus1.result = -1;
//                    bus1.register_address = -1;
//                } else if (bus2.register_address == register_file1.reg[reg_address2].address) {
//                    stage_decode.input_buffer.source_register1.value = bus2.result;
//                    printf("Result copied through forwarding bus\n");
//                    //free forward bus
//                    bus2.result = -1;
//                    bus2.register_address = -1;
//                } else if (bus3.register_address == register_file1.reg[reg_address2].address) {
//                    stage_decode.input_buffer.source_register1.value = bus3.result;
//                    printf("Result copied through forwarding bus\n");
//                    //free forward bus
//                    bus3.result = -1;
//                    bus3.register_address = -1;
//                } else {
//                    //update the stall flag
//                    common.stalled = TRUE;
//                    printf("\nStall is triggered in decode stage for register: R%d\n", reg_address2);
//                    // Push NOP instruction
//                    stage_decodeA.output_buffer.PC = 1;
//                    strcpy(stage_decodeA.output_buffer.mnemonic, "NOP");
//                    return;
//                }

        }
   // }
        else if (string_copy1[0] == '#') {
            sscanf(str3, "#%d", &literal);
            printf("The literal value is : %d\n", literal);
            stage_decode.input_buffer.literal = literal;
        }

    //Source Register 2
    strncpy(string_copy2, str4, 1);
    if (string_copy2[0] == 'R') {
        sscanf(str4, "R%d", &reg_address3); // The register number is stored in the variable reg_number
        printf("The source register is : R%d\n", reg_address3);
        //fetch_stage.input_buffer.source_register1=register_file1.reg[greg_number];
        //update the register in

        for (int i = 0; i < 16; ++i) {
            if (RAT1[i].index_R == reg_address3 && RAT1[i].RorP == R) {
                stage_decode.input_buffer.source_register2 = register_file1.reg[reg_address3];
                stage_decode.input_buffer.source_register2.address = reg_address3;
                stage_decode.input_buffer.validity = TRUE;
                stage_decode.input_buffer.src2_RorP = R;
                break;
            } else if (RAT1[i].index_R == reg_address3 && RAT1[i].RorP == P) {
                stage_decode.input_buffer.source_register2 = physical_RF.prf[RAT1[i].index_P];
                stage_decode.input_buffer.source_register2.address = RAT1[i].index_P;
                stage_decode.input_buffer.validity = TRUE;
                stage_decode.input_buffer.src2_RorP = P;
                break;
            }
        }
        if(stage_decode.input_buffer.validity!=TRUE) {
            if (bus1.register_address == reg_address3) {
                stage_decode.input_buffer.source_register2.value = bus1.result;
                stage_decode.input_buffer.source_register2.address = reg_address3;
                stage_decode.input_buffer.validity=TRUE;
                stage_decode.input_buffer.src2_RorP=P;
                printf("Result copied through forwarding bus\n");
                //free forward bus
                bus1.result = -1;
                bus1.register_address = -1;
            } else if (bus2.register_address == reg_address3) {
                stage_decode.input_buffer.source_register2.value = bus2.result;
                stage_decode.input_buffer.source_register2.address = reg_address3;
                stage_decode.input_buffer.validity=TRUE;
                stage_decode.input_buffer.src2_RorP=P;
                printf("Result copied through forwarding bus\n");
                //free forward bus
                bus2.result = -1;
                bus2.register_address = -1;
            } else if (bus3.register_address == reg_address3) {
                stage_decode.input_buffer.source_register2.value = bus3.result;
                stage_decode.input_buffer.source_register2.address = reg_address3;
                stage_decode.input_buffer.validity=TRUE;
                stage_decode.input_buffer.src2_RorP=P;
                printf("Result copied through forwarding bus\n");
                //free forward bus
                bus3.result = -1;
                bus3.register_address = -1;
            } else {
                stage_decode.input_buffer.source_register2 = register_file1.dummy;
                stage_decode.input_buffer.validity=FALSE;
                stage_decode.input_buffer.src2_RorP=-1;
                stage_decode.input_buffer.source_register2.address = reg_address3;
            }
        }

//            printf("The source register is : R%d\n", reg_address3);
//            stage_decode.input_buffer.source_register2 = register_file1.reg[reg_address3];
//            stage_decode.input_buffer.source_register2.address = reg_address3;
//            register_file1.reg[reg_address3].address = reg_address3;
//            if (register_file1.reg[reg_address3].status == FALSE) {
//                if (bus1.register_address == register_file1.reg[reg_address2].address) {
//                    stage_decode.input_buffer.source_register1.value = bus1.result;
//                    printf("Result copied through forwarding bus\n");
//                    //free forward bus
//                    bus1.result = -1;
//                    bus1.register_address = -1;
//                } else if (bus2.register_address == register_file1.reg[reg_address2].address) {
//                    stage_decode.input_buffer.source_register1.value = bus2.result;
//                    printf("Result copied through forwarding bus\n");
//                    //free forward bus
//                    bus2.result = -1;
//                    bus2.register_address = -1;
//                } else if (bus3.register_address == register_file1.reg[reg_address2].address) {
//                    stage_decode.input_buffer.source_register1.value = bus3.result;
//                    printf("Result copied through forwarding bus\n");
//                    //free forward bus
//                    bus3.result = -1;
//                    bus3.register_address = -1;
//                } else {
//                    //update the stall flag
//                    common.stalled = TRUE;
//                    printf("\nStall is triggered in decode stage for register: R%d\n", reg_address3);
//                    // Push NOP instruction
//                    stage_decodeA.output_buffer.PC = 1;
//                    strcpy(stage_decodeA.output_buffer.mnemonic, "NOP");
//                    return;
//                }

    } else if ((string_copy2[0]) == '#') {
        sscanf(str4, "#%d", &literal);
        printf("The literal value is : %d\n", literal);
        stage_decode.input_buffer.literal = literal;
    } else
        printf("Error in Arithmetic instruction\n");

    //Destination Register
    sscanf(str2, "R%d", &reg_address1); // The register number is stored in the variable reg_number

    printf("The destination register is : R%d\n", reg_address1);

    // stage_decode.input_buffer.dest_register = register_file1.reg[reg_address1];
    //  stage_decode.input_buffer.dest_register.address = reg_address1;
    register_file1.reg[reg_address1].address = reg_address1;
    for (int i = 0; i < 32; ++i) {
        if (physical_RF.prf[i].status == FREE) {
            physical_index = i;
            printf("the register R[%d] is converted to P[%d]\n", reg_address1, i);
            stage_decode.input_buffer.dest_register = physical_RF.prf[i];
            stage_decode.input_buffer.dest_register.address = i;
            stage_decode.input_buffer.dest_RorP=P;
            RAT1[reg_address1].RorP = P;
            RAT1[reg_address1].index_P = i;
            physical_RF.prf[i].status=ALLOCATED;
            break;
        }
    }
        register_file1.reg[reg_address1].status = FALSE;
//        if (register_file1.reg[reg_address2].status == FALSE) {
//
//            if (bus1.register_address == register_file1.reg[reg_address2].address) {
//                stage_decode.input_buffer.source_register1.value = bus1.result;
//                printf("Result copied through forwarding bus\n");
//                //free forward bus
//                bus1.result = -1;
//                bus1.register_address = -1;
//            } else if (bus2.register_address == register_file1.reg[reg_address2].address) {
//                stage_decode.input_buffer.source_register1.value = bus2.result;
//                printf("Result copied through forwarding bus\n");
//                //free forward bus
//                bus2.result = -1;
//                bus2.register_address = -1;
//            } else if (bus3.register_address == register_file1.reg[reg_address2].address) {
//                stage_decode.input_buffer.source_register1.value = bus3.result;
//                printf("Result copied through forwarding bus\n");
//                //free forward bus
//                bus3.result = -1;
//                bus3.register_address = -1;
//            } else {
//                //update the stall flag
//                common.stalled = TRUE;
//                printf("\nStall is triggered in decode stage for register: R%d\n", reg_address2);
//                stage_decodeA.output_buffer.PC = 1;
//                strcpy(stage_decodeA.output_buffer.mnemonic, "NOP");
//                return;
//
//
//            }
 //           }
           // else
            // set the register invalid flag so that the other instructions cannot access the register until set in WB stage
         //   register_file1.reg[reg_address1].status = FALSE; //0=INVALID

//        if (execute_int_free == FALSE)
//            decode_stalled_flag = TRUE;
//
//        output_buffer_decodeA();
    }
    else if (!strcmp(stage_decode.input_buffer.mnemonic, "MUL")) {
        //Source Register 1
        strncpy(string_copy1, str3, 1);
        if (string_copy1[0] == 'R') {
            sscanf(str3, "R%d", &reg_address2); // The register number is stored in the variable reg_number

            printf("The source register is : R%d\n", reg_address2);
            //fetch_stage.input_buffer.source_register1=register_file1.reg[greg_number];
            //update the register in

            for (int i = 0; i < 16; ++i) {
                if (RAT1[i].index_R == reg_address2 && RAT1[i].RorP == R) {
                    stage_decode.input_buffer.source_register1 = register_file1.reg[reg_address2];
                    stage_decode.input_buffer.source_register1.address = reg_address2;
                    stage_decode.input_buffer.validity = TRUE;
                    stage_decode.input_buffer.src1_RorP = R;
                    break;
                } else if (RAT1[i].index_R == reg_address2 && RAT1[i].RorP == P) {
                    stage_decode.input_buffer.source_register1 = physical_RF.prf[RAT1[i].index_P];
                    stage_decode.input_buffer.source_register1.address = RAT1[i].index_P;
                    stage_decode.input_buffer.validity = TRUE;
                    stage_decode.input_buffer.src1_RorP = P;
                    break;
                }
            }
            if(stage_decode.input_buffer.validity!=TRUE) {
                if (bus1.register_address == reg_address2) {
                    stage_decode.input_buffer.source_register1.value = bus1.result;
                    stage_decode.input_buffer.source_register1.address = reg_address2;
                    stage_decode.input_buffer.validity=TRUE;
                    stage_decode.input_buffer.src1_RorP=P;
                    printf("Result copied through forwarding bus\n");
                    //free forward bus
                    bus1.result = -1;
                    bus1.register_address = -1;
                } else if (bus2.register_address == reg_address2) {
                    stage_decode.input_buffer.source_register1.value = bus2.result;
                    stage_decode.input_buffer.source_register1.address = reg_address2;
                    stage_decode.input_buffer.validity=TRUE;
                    stage_decode.input_buffer.src1_RorP=P;
                    printf("Result copied through forwarding bus\n");
                    //free forward bus
                    bus2.result = -1;
                    bus2.register_address = -1;
                } else if (bus3.register_address == reg_address2) {
                    stage_decode.input_buffer.source_register1.value = bus3.result;
                    stage_decode.input_buffer.source_register1.address = reg_address2;
                    stage_decode.input_buffer.validity=TRUE;
                    stage_decode.input_buffer.src1_RorP=P;
                    printf("Result copied through forwarding bus\n");
                    //free forward bus
                    bus3.result = -1;
                    bus3.register_address = -1;
                } else {
                    stage_decode.input_buffer.source_register1 = register_file1.dummy;
                    stage_decode.input_buffer.validity=FALSE;
                    stage_decode.input_buffer.src1_RorP=-1;
                    stage_decode.input_buffer.source_register1.address = reg_address2;
                }
            }



//            stage_decode.input_buffer.source_register1 = register_file1.reg[reg_address2];
//            stage_decode.input_buffer.source_register1.address = reg_address2;
//            register_file1.reg[reg_address2].address = reg_address2;
//            if (register_file1.reg[reg_address2].status == FALSE) {
//                if (bus1.register_address == register_file1.reg[reg_address2].address) {
//                    stage_decode.input_buffer.source_register1.value = bus1.result;
//                    printf("Result copied through forwarding bus\n");
//                    //free forward bus
//                    bus1.result = -1;
//                    bus1.register_address = -1;
//                } else if (bus2.register_address == register_file1.reg[reg_address2].address) {
//                    stage_decode.input_buffer.source_register1.value = bus2.result;
//                    printf("Result copied through forwarding bus\n");
//                    //free forward bus
//                    bus2.result = -1;
//                    bus2.register_address = -1;
//                } else if (bus3.register_address == register_file1.reg[reg_address2].address) {
//                    stage_decode.input_buffer.source_register1.value = bus3.result;
//                    printf("Result copied through forwarding bus\n");
//                    //free forward bus
//                    bus3.result = -1;
//                    bus3.register_address = -1;
//                } else {
//                    //update the stall flag
//                    common.stalled = TRUE;
//                    printf("\nStall is triggered in decode stage for register: R%d\n", reg_address2);
//                    stage_decodeA.output_buffer.PC = 1;
//                    strcpy(stage_decodeA.output_buffer.mnemonic, "NOP");
//                    return;
//                }
//            }
        } else if (string_copy1[0] == '#') {
            sscanf(str3, "#%d", &literal);
            printf("The literal value is : %d\n", literal);
            stage_decode.input_buffer.literal = literal;
        }

        //Source Register 2
        strncpy(string_copy2, str4, 1);
        if (string_copy2[0] == 'R') {
            sscanf(str4, "R%d", &reg_address3); // The register number is stored in the variable reg_number

            printf("The source register is : R%d\n", reg_address3);

            for (int i = 0; i < 16; ++i) {
                if (RAT1[i].index_R == reg_address3 && RAT1[i].RorP == R) {
                    stage_decode.input_buffer.source_register2 = register_file1.reg[reg_address3];
                    stage_decode.input_buffer.source_register2.address = reg_address3;
                    stage_decode.input_buffer.validity = TRUE;
                    stage_decode.input_buffer.src2_RorP = R;
                    break;
                } else if (RAT1[i].index_R == reg_address3 && RAT1[i].RorP == P) {
                    stage_decode.input_buffer.source_register2 = physical_RF.prf[RAT1[i].index_P];
                    stage_decode.input_buffer.source_register2.address = RAT1[i].index_P;
                    stage_decode.input_buffer.validity = TRUE;
                    stage_decode.input_buffer.src2_RorP = P;
                    break;
                }
            }
            if(stage_decode.input_buffer.validity!=TRUE) {
                if (bus1.register_address == reg_address3) {
                    stage_decode.input_buffer.source_register2.value = bus1.result;
                    stage_decode.input_buffer.source_register2.address = reg_address3;
                    stage_decode.input_buffer.validity=TRUE;
                    stage_decode.input_buffer.src2_RorP=P;
                    printf("Result copied through forwarding bus\n");
                    //free forward bus
                    bus1.result = -1;
                    bus1.register_address = -1;
                } else if (bus2.register_address == reg_address3) {
                    stage_decode.input_buffer.source_register2.value = bus2.result;
                    stage_decode.input_buffer.source_register2.address = reg_address3;
                    stage_decode.input_buffer.validity=TRUE;
                    stage_decode.input_buffer.src2_RorP=P;
                    printf("Result copied through forwarding bus\n");
                    //free forward bus
                    bus2.result = -1;
                    bus2.register_address = -1;
                } else if (bus3.register_address == reg_address3) {
                    stage_decode.input_buffer.source_register2.value = bus3.result;
                    stage_decode.input_buffer.source_register2.address = reg_address3;
                    stage_decode.input_buffer.validity=TRUE;
                    stage_decode.input_buffer.src2_RorP=P;
                    printf("Result copied through forwarding bus\n");
                    //free forward bus
                    bus3.result = -1;
                    bus3.register_address = -1;
                } else {
                    stage_decode.input_buffer.source_register2 = register_file1.dummy;
                    stage_decode.input_buffer.validity=FALSE;
                    stage_decode.input_buffer.src2_RorP=-1;
                    stage_decode.input_buffer.source_register2.address = reg_address3;
                }
            }
//            stage_decode.input_buffer.source_register2 = register_file1.reg[reg_address3];
//            stage_decode.input_buffer.source_register2.address = reg_address3;
//            register_file1.reg[reg_address3].address = reg_address3;
//            if (register_file1.reg[reg_address3].status == FALSE) {
//                if (bus1.register_address == register_file1.reg[reg_address2].address) {
//                    stage_decode.input_buffer.source_register1.value = bus1.result;
//                    printf("Result copied through forwarding bus\n");
//                    //free forward bus
//                    bus1.result = -1;
//                    bus1.register_address = -1;
//                } else if (bus2.register_address == register_file1.reg[reg_address2].address) {
//                    stage_decode.input_buffer.source_register1.value = bus2.result;
//                    printf("Result copied through forwarding bus\n");
//                    //free forward bus
//                    bus2.result = -1;
//                    bus2.register_address = -1;
//                } else if (bus3.register_address == register_file1.reg[reg_address2].address) {
//                    stage_decode.input_buffer.source_register1.value = bus3.result;
//                    printf("Result copied through forwarding bus\n");
//                    //free forward bus
//                    bus3.result = -1;
//                    bus3.register_address = -1;
//                } else {
//                    //update the stall flag
//                    common.stalled = TRUE;
//                    printf("\nStall is triggered in decode stage for register: R%d\n", reg_address3);
//                    // Push NOP instruction
//                    stage_decodeA.output_buffer.PC = 1;
//                    strcpy(stage_decodeA.output_buffer.mnemonic, "NOP");
//                    return;
//                }
//
//            }
        } else if ((string_copy2[0]) == '#') {
            sscanf(str4, "#%d", &literal);
            printf("The literal value is : %d\n", literal);
            stage_decode.input_buffer.literal = literal;
        } else
            printf("Error in Arithmetic instruction\n");

        //Destination Register
        sscanf(str2, "R%d", &reg_address1); // The register number is stored in the variable reg_number

        printf("The destination register is : R%d\n", reg_address1);

        register_file1.reg[reg_address1].address = reg_address1;
        for (int i = 0; i < 32; ++i) {
            if (physical_RF.prf[i].status == FREE) {
                physical_index = i;
                printf("the register R[%d] is converted to P[%d]\n", reg_address1, i);
                stage_decode.input_buffer.dest_register = physical_RF.prf[i];
                stage_decode.input_buffer.dest_register.address = i;
                stage_decode.input_buffer.dest_RorP=P;
                RAT1[reg_address1].RorP = P;
                RAT1[reg_address1].index_P = i;
                physical_RF.prf[i].status=ALLOCATED;

                break;
            }
        }
        register_file1.reg[reg_address1].status = FALSE;


//        stage_decode.input_buffer.dest_register = register_file1.reg[reg_address1];
//        stage_decode.input_buffer.dest_register.address = reg_address1;
//        register_file1.reg[reg_address1].address = reg_address1;
//        if (register_file1.reg[reg_address2].status == FALSE) {
//            if (bus1.register_address == register_file1.reg[reg_address2].address) {
//                stage_decode.input_buffer.source_register1.value = bus1.result;
//                printf("Result copied through forwarding bus\n");
//                //free forward bus
//                bus1.result = -1;
//                bus1.register_address = -1;
//            } else if (bus2.register_address == register_file1.reg[reg_address2].address) {
//                stage_decode.input_buffer.source_register1.value = bus2.result;
//                printf("Result copied through forwarding bus\n");
//                //free forward bus
//                bus2.result = -1;
//                bus2.register_address = -1;
//            } else if (bus3.register_address == register_file1.reg[reg_address2].address) {
//                stage_decode.input_buffer.source_register1.value = bus3.result;
//                printf("Result copied through forwarding bus\n");
//                //free forward bus
//                bus3.result = -1;
//                bus3.register_address = -1;
//            } else {
//                //update the stall flag
//                common.stalled = TRUE;
//                printf("\nStall is triggered in decode stage for register: R%d\n", reg_address2);
//                stage_decodeA.output_buffer.PC = 1;
//                strcpy(stage_decodeA.output_buffer.mnemonic, "NOP");
//                return;
//           }
       // }
       // else
            // set the register invalid flag so that the other instructions cannot access the register until set in WB stage
//            register_file1.reg[reg_address1].status = FALSE; //0=INVALID
//
//        if (execute_mul2_free == FALSE)
//            decode_stalled_flag = TRUE;
//        output_buffer_decodeB();

    } else if (!strcmp(stage_decode.input_buffer.mnemonic, "DIV")) {
        //Source Register 1
        strncpy(string_copy1, str3, 1);
        if (string_copy1[0] == 'R') {
            sscanf(str3, "R%d", &reg_address2); // The register number is stored in the variable reg_number

            printf("The source register is : R%d\n", reg_address2);
            //fetch_stage.input_buffer.source_register1=register_file1.reg[greg_number];
            //update the register in
            for (int i = 0; i < 16; ++i) {
                if (RAT1[i].index_R == reg_address2 && RAT1[i].RorP == R) {
                    stage_decode.input_buffer.source_register1 = register_file1.reg[reg_address2];
                    stage_decode.input_buffer.source_register1.address = reg_address2;
                    stage_decode.input_buffer.validity = TRUE;
                    stage_decode.input_buffer.src1_RorP = R;
                    break;
                } else if (RAT1[i].index_R == reg_address2 && RAT1[i].RorP == P) {
                    stage_decode.input_buffer.source_register1 = physical_RF.prf[RAT1[i].index_P];
                    stage_decode.input_buffer.source_register1.address = RAT1[i].index_P;
                    stage_decode.input_buffer.validity = TRUE;
                    stage_decode.input_buffer.src1_RorP = P;
                    break;
                }
            }
            if(stage_decode.input_buffer.validity!=TRUE) {
                if (bus1.register_address == reg_address2) {
                    stage_decode.input_buffer.source_register1.value = bus1.result;
                    stage_decode.input_buffer.source_register1.address = reg_address2;
                    stage_decode.input_buffer.validity=TRUE;
                    stage_decode.input_buffer.src1_RorP=P;
                    printf("Result copied through forwarding bus\n");
                    //free forward bus
                    bus1.result = -1;
                    bus1.register_address = -1;
                } else if (bus2.register_address == reg_address2) {
                    stage_decode.input_buffer.source_register1.value = bus2.result;
                    stage_decode.input_buffer.source_register1.address = reg_address2;
                    stage_decode.input_buffer.validity=TRUE;
                    stage_decode.input_buffer.src1_RorP=P;
                    printf("Result copied through forwarding bus\n");
                    //free forward bus
                    bus2.result = -1;
                    bus2.register_address = -1;
                } else if (bus3.register_address == reg_address2) {
                    stage_decode.input_buffer.source_register1.value = bus3.result;
                    stage_decode.input_buffer.source_register1.address = reg_address2;
                    stage_decode.input_buffer.validity=TRUE;
                    stage_decode.input_buffer.src1_RorP=P;
                    printf("Result copied through forwarding bus\n");
                    //free forward bus
                    bus3.result = -1;
                    bus3.register_address = -1;
                } else {
                    stage_decode.input_buffer.source_register1 = register_file1.dummy;
                    stage_decode.input_buffer.validity=FALSE;
                    stage_decode.input_buffer.src1_RorP=-1;
                    stage_decode.input_buffer.source_register1.address = reg_address2;
                }
            }
        } else if (string_copy1[0] == '#') {
            sscanf(str3, "#%d", &literal);
            printf("The literal value is : %d\n", literal);
            stage_decode.input_buffer.literal = literal;
        }

        //Source Register 2
        strncpy(string_copy2, str4, 1);
        if (string_copy2[0] == 'R') {
            sscanf(str4, "R%d", &reg_address3); // The register number is stored in the variable reg_number

            printf("The source register is : R%d\n", reg_address3);

            for (int i = 0; i < 16; ++i) {
                if (RAT1[i].index_R == reg_address3 && RAT1[i].RorP == R) {
                    stage_decode.input_buffer.source_register2 = register_file1.reg[reg_address3];
                    stage_decode.input_buffer.source_register2.address = reg_address3;
                    stage_decode.input_buffer.validity = TRUE;
                    stage_decode.input_buffer.src2_RorP = R;
                    break;
                } else if (RAT1[i].index_R == reg_address3 && RAT1[i].RorP == P) {
                    stage_decode.input_buffer.source_register2 = physical_RF.prf[RAT1[i].index_P];
                    stage_decode.input_buffer.source_register2.address = RAT1[i].index_P;
                    stage_decode.input_buffer.validity = TRUE;
                    stage_decode.input_buffer.src2_RorP = P;
                    break;
                }
            }
            if(stage_decode.input_buffer.validity!=TRUE) {
                if (bus1.register_address == reg_address3) {
                    stage_decode.input_buffer.source_register2.value = bus1.result;
                    stage_decode.input_buffer.source_register2.address = reg_address3;
                    stage_decode.input_buffer.validity=TRUE;
                    stage_decode.input_buffer.src2_RorP=P;
                    printf("Result copied through forwarding bus\n");
                    //free forward bus
                    bus1.result = -1;
                    bus1.register_address = -1;
                } else if (bus2.register_address == reg_address3) {
                    stage_decode.input_buffer.source_register2.value = bus2.result;
                    stage_decode.input_buffer.source_register2.address = reg_address3;
                    stage_decode.input_buffer.validity=TRUE;
                    stage_decode.input_buffer.src2_RorP=P;
                    printf("Result copied through forwarding bus\n");
                    //free forward bus
                    bus2.result = -1;
                    bus2.register_address = -1;
                } else if (bus3.register_address == reg_address3) {
                    stage_decode.input_buffer.source_register2.value = bus3.result;
                    stage_decode.input_buffer.source_register2.address = reg_address3;
                    stage_decode.input_buffer.validity=TRUE;
                    stage_decode.input_buffer.src2_RorP=P;
                    printf("Result copied through forwarding bus\n");
                    //free forward bus
                    bus3.result = -1;
                    bus3.register_address = -1;
                } else {
                    stage_decode.input_buffer.source_register2 = register_file1.dummy;
                    stage_decode.input_buffer.validity=FALSE;
                    stage_decode.input_buffer.src2_RorP=-1;
                    stage_decode.input_buffer.source_register2.address = reg_address3;
                }
            }
//            stage_decode.input_buffer.source_register2 = register_file1.reg[reg_address3];
//            stage_decode.input_buffer.source_register2.address = reg_address3;
//            register_file1.reg[reg_address3].address = reg_address3;
//            if (register_file1.reg[reg_address3].status == FALSE) {
//                if (bus1.register_address == register_file1.reg[reg_address2].address) {
//                    stage_decode.input_buffer.source_register1.value = bus1.result;
//                    printf("Result copied through forwarding bus\n");
//                    //free forward bus
//                    bus1.result = -1;
//                    bus1.register_address = -1;
//                } else if (bus2.register_address == register_file1.reg[reg_address2].address) {
//                    stage_decode.input_buffer.source_register1.value = bus2.result;
//                    printf("Result copied through forwarding bus\n");
//                    //free forward bus
//                    bus2.result = -1;
//                    bus2.register_address = -1;
//                } else if (bus3.register_address == register_file1.reg[reg_address2].address) {
//                    stage_decode.input_buffer.source_register1.value = bus3.result;
//                    printf("Result copied through forwarding bus\n");
//                    //free forward bus
//                    bus3.result = -1;
//                    bus3.register_address = -1;
//                } else {
//                    //update the stall flag
//                    common.stalled = TRUE;
//                    printf("\nStall is triggered in decode stage for register: R%d\n", reg_address3);
//                    // Push NOP instruction
//                    stage_decodeA.output_buffer.PC = 1;
//                    strcpy(stage_decodeA.output_buffer.mnemonic, "NOP");
//                    return;
//                }

            //}
        } else if ((string_copy2[0]) == '#') {
            sscanf(str4, "#%d", &literal);
            printf("The literal value is : %d\n", literal);
            stage_decode.input_buffer.literal = literal;
        } else
            printf("Error in Arithmetic instruction\n");

        //Destination Register
        sscanf(str2, "R%d", &reg_address1); // The register number is stored in the variable reg_number

        printf("The destination register is : R%d\n", reg_address1);

        register_file1.reg[reg_address1].address = reg_address1;
        for (int i = 0; i < 32; ++i) {
            if (physical_RF.prf[i].status == FREE) {
                physical_index = i;
                printf("the register R[%d] is converted to P[%d]\n", reg_address1, i);
                stage_decode.input_buffer.dest_register = physical_RF.prf[i];
                stage_decode.input_buffer.dest_register.address = i;
                stage_decode.input_buffer.dest_RorP=P;
                RAT1[reg_address1].RorP = P;
                RAT1[reg_address1].index_P = i;
                physical_RF.prf[i].status=ALLOCATED;

                break;
            }
        }
        register_file1.reg[reg_address1].status = FALSE;

//        stage_decode.input_buffer.dest_register = register_file1.reg[reg_address1];
//        stage_decode.input_buffer.dest_register.address = reg_address1;
//        register_file1.reg[reg_address1].address = reg_address1;
//        if (register_file1.reg[reg_address2].status == FALSE) {
//            if (bus1.register_address == register_file1.reg[reg_address2].address) {
//                stage_decode.input_buffer.source_register1.value = bus1.result;
//                printf("Result copied through forwarding bus\n");
//                //free forward bus
//                bus1.result = -1;
//                bus1.register_address = -1;
//            } else if (bus2.register_address == register_file1.reg[reg_address2].address) {
//                stage_decode.input_buffer.source_register1.value = bus2.result;
//                printf("Result copied through forwarding bus\n");
//                //free forward bus
//                bus2.result = -1;
//                bus2.register_address = -1;
//            } else if (bus3.register_address == register_file1.reg[reg_address2].address) {
//                stage_decode.input_buffer.source_register1.value = bus3.result;
//                printf("Result copied through forwarding bus\n");
//                //free forward bus
//                bus3.result = -1;
//                bus3.register_address = -1;
//            } else {
//                //update the stall flag
//                common.stalled = TRUE;
//                printf("\nStall is triggered in decode stage for register: R%d\n", reg_address2);
//                stage_decodeA.output_buffer.PC = 1;
//                strcpy(stage_decodeA.output_buffer.mnemonic, "NOP");
//                return;
//            }
//        } else
            // set the register invalid flag so that the other instructions cannot access the register until set in WB stage
           // register_file1.reg[reg_address1].status = FALSE; //0=INVALID
//
//        if (execute_div4_free == FALSE)
//            decode_stalled_flag = TRUE;
//        output_buffer_decodeC();

    } else if (!strcmp(stage_decode.input_buffer.mnemonic, "MOVC")) {
        // Source Register 1
        strncpy(string_copy1, str3, 1);
        if (string_copy1[0] == 'R') {
            sscanf(str3, "R%d", &reg_address2); // The register number is stored in the variable reg_number

            printf("The source register is : R%d\n", reg_address2);

            for (int i = 0; i < 16; ++i) {
                if (RAT1[i].index_R == reg_address2 && RAT1[i].RorP == R) {
                    stage_decode.input_buffer.source_register1 = register_file1.reg[reg_address2];
                    stage_decode.input_buffer.source_register1.address = reg_address2;
                    stage_decode.input_buffer.validity = TRUE;
                    stage_decode.input_buffer.src1_RorP = R;
                    break;
                } else if (RAT1[i].index_R == reg_address2 && RAT1[i].RorP == P) {
                    stage_decode.input_buffer.source_register1 = physical_RF.prf[RAT1[i].index_P];
                    stage_decode.input_buffer.source_register1.address = RAT1[i].index_P;
                    stage_decode.input_buffer.validity = TRUE;
                    stage_decode.input_buffer.src1_RorP = P;
                    break;
                }
            }
            if(stage_decode.input_buffer.validity!=TRUE) {
                if (bus1.register_address == reg_address2) {
                    stage_decode.input_buffer.source_register1.value = bus1.result;
                    stage_decode.input_buffer.source_register1.address = reg_address2;
                    stage_decode.input_buffer.validity=TRUE;
                    stage_decode.input_buffer.src1_RorP=P;
                    printf("Result copied through forwarding bus\n");
                    //free forward bus
                    bus1.result = -1;
                    bus1.register_address = -1;
                } else if (bus2.register_address == reg_address2) {
                    stage_decode.input_buffer.source_register1.value = bus2.result;
                    stage_decode.input_buffer.source_register1.address = reg_address2;
                    stage_decode.input_buffer.validity=TRUE;
                    stage_decode.input_buffer.src1_RorP=P;
                    printf("Result copied through forwarding bus\n");
                    //free forward bus
                    bus2.result = -1;
                    bus2.register_address = -1;
                } else if (bus3.register_address == reg_address2) {
                    stage_decode.input_buffer.source_register1.value = bus3.result;
                    stage_decode.input_buffer.source_register1.address = reg_address2;
                    stage_decode.input_buffer.validity=TRUE;
                    stage_decode.input_buffer.src1_RorP=P;
                    printf("Result copied through forwarding bus\n");
                    //free forward bus
                    bus3.result = -1;
                    bus3.register_address = -1;
                } else {
                    stage_decode.input_buffer.source_register1 = register_file1.dummy;
                    stage_decode.input_buffer.validity=FALSE;
                    stage_decode.input_buffer.src1_RorP=-1;
                    stage_decode.input_buffer.source_register1.address = reg_address2;
                }
            }

//            stage_decode.input_buffer.source_register1 = register_file1.reg[reg_address2];
//            stage_decode.input_buffer.source_register1.address = reg_address2;
//            register_file1.reg[reg_address2].address = reg_address2;
//            if (register_file1.reg[reg_address2].status == FALSE) {
//                if (bus1.register_address == register_file1.reg[reg_address2].address) {
//                    stage_decode.input_buffer.source_register1.value = bus1.result;
//                    printf("Result copied through forwarding bus\n");
//                    //free forward bus
//                    bus1.result = -1;
//                    bus1.register_address = -1;
//                } else if (bus2.register_address == register_file1.reg[reg_address2].address) {
//                    stage_decode.input_buffer.source_register1.value = bus2.result;
//                    printf("Result copied through forwarding bus\n");
//                    //free forward bus
//                    bus2.result = -1;
//                    bus2.register_address = -1;
//                } else if (bus3.register_address == register_file1.reg[reg_address2].address) {
//                    stage_decode.input_buffer.source_register1.value = bus3.result;
//                    printf("Result copied through forwarding bus\n");
//                    //free forward bus
//                    bus3.result = -1;
//                    bus3.register_address = -1;
//                } else {
//                    //update the stall flag
//                    common.stalled = TRUE;
//                    printf("\nStall is triggered in decode stage for register: R%d\n", reg_address2);
//                    // Push NOP instruction
//                    stage_decodeA.output_buffer.PC = 1;
//                    strcpy(stage_decodeA.output_buffer.mnemonic, "NOP");
//                    return;
//                }
//            }
        } else if (string_copy1[0] == '#') {
            sscanf(str3, "#%d", &literal);
            printf("The literal value is : %d\n", literal);
            stage_decode.input_buffer.literal = literal;
        } else
            printf("Error in MOVC instruction\n");

        // Destination Register

        sscanf(str2, "R%d", &reg_address1); // The register number is stored in the variable reg_number

        printf("The destination register is : R%d\n", reg_address1);

        register_file1.reg[reg_address1].address = reg_address1;
        for (int i = 0; i < 32; ++i) {
            if (physical_RF.prf[i].status == FREE) {
                physical_index = i;
                printf("the register R[%d] is converted to P[%d]\n", reg_address1, i);
                stage_decode.input_buffer.dest_register = physical_RF.prf[i];
                stage_decode.input_buffer.dest_register.address = i;
                stage_decode.input_buffer.dest_RorP=P;
                RAT1[reg_address1].RorP = P;
                RAT1[reg_address1].index_P = i;
                physical_RF.prf[i].status=ALLOCATED;

                break;
            }
        }
        register_file1.reg[reg_address1].status = FALSE;

//
//        stage_decode.input_buffer.dest_register = register_file1.reg[reg_address1];
//        stage_decode.input_buffer.dest_register.address = reg_address1;
//        register_file1.reg[reg_address1].address = reg_address1;
//        if (register_file1.reg[reg_address2].status == FALSE) {
//            if (bus1.register_address == register_file1.reg[reg_address2].address) {
//                stage_decode.input_buffer.source_register1.value = bus1.result;
//                printf("Result copied through forwarding bus\n");
//                //free forward bus
//                bus1.result = -1;
//                bus1.register_address = -1;
//            } else if (bus2.register_address == register_file1.reg[reg_address2].address) {
//                stage_decode.input_buffer.source_register1.value = bus2.result;
//                printf("Result copied through forwarding bus\n");
//                //free forward bus
//                bus2.result = -1;
//                bus2.register_address = -1;
//            } else if (bus3.register_address == register_file1.reg[reg_address2].address) {
//                stage_decode.input_buffer.source_register1.value = bus3.result;
//                printf("Result copied through forwarding bus\n");
//                //free forward bus
//                bus3.result = -1;
//                bus3.register_address = -1;
//            } else {
//                //update the stall flag
//                common.stalled = TRUE;
//                printf("\nStall is triggered in decode stage for register: R%d\n", reg_address2);
//                stage_decodeA.output_buffer.PC = 1;
//                strcpy(stage_decodeA.output_buffer.mnemonic, "NOP");
//                return;
//            }
//        } else
            // set the register invalid flag so that the other instructions cannot access the register until set in WB stage
      //      register_file1.reg[reg_address1].status = FALSE; //0=INVALID

//        if (execute_int_free == FALSE)
//            decode_stalled_flag = TRUE;
//        output_buffer_decodeA();

    } else if (!strcmp(stage_decode.input_buffer.mnemonic, "HALT\n")) {
        printf("\n HALT INSTRUCTION IS TRIGGERED!!!\n");
        stage_decode.output_buffer.PC = 1;
        common.stalled = TRUE;
        flags1.HALT = TRUE;
        strcpy(stage_decode.output_buffer.mnemonic, "NOP");
        return;
    } else if (!strcmp(stage_decode.input_buffer.mnemonic, "STORE")) {
        printf("Store inst fetched \n");

        sscanf(str2, "R%d", &reg_address1); // The register number is stored in the variable reg_number

        printf("The source 1 register is : R%d\n", reg_address1);

        for (int i = 0; i < 16; ++i) {
            if (RAT1[i].index_R == reg_address2 && RAT1[i].RorP == R) {
                stage_decode.input_buffer.source_register1 = register_file1.reg[reg_address2];
                stage_decode.input_buffer.source_register1.address = reg_address2;
                stage_decode.input_buffer.validity = TRUE;
                stage_decode.input_buffer.src1_RorP = R;
                break;
            } else if (RAT1[i].index_R == reg_address2 && RAT1[i].RorP == P) {
                stage_decode.input_buffer.source_register1 = physical_RF.prf[RAT1[i].index_P];
                stage_decode.input_buffer.source_register1.address = RAT1[i].index_P;
                stage_decode.input_buffer.validity = TRUE;
                stage_decode.input_buffer.src1_RorP = P;
                break;
            }
        }
        if(stage_decode.input_buffer.validity!=TRUE) {
            if (bus1.register_address == reg_address2) {
                stage_decode.input_buffer.source_register1.value = bus1.result;
                stage_decode.input_buffer.source_register1.address = reg_address2;
                stage_decode.input_buffer.validity=TRUE;
                stage_decode.input_buffer.src1_RorP=P;
                printf("Result copied through forwarding bus\n");
                //free forward bus
                bus1.result = -1;
                bus1.register_address = -1;
            } else if (bus2.register_address == reg_address2) {
                stage_decode.input_buffer.source_register1.value = bus2.result;
                stage_decode.input_buffer.source_register1.address = reg_address2;
                stage_decode.input_buffer.validity=TRUE;
                stage_decode.input_buffer.src1_RorP=P;
                printf("Result copied through forwarding bus\n");
                //free forward bus
                bus2.result = -1;
                bus2.register_address = -1;
            } else if (bus3.register_address == reg_address2) {
                stage_decode.input_buffer.source_register1.value = bus3.result;
                stage_decode.input_buffer.source_register1.address = reg_address2;
                stage_decode.input_buffer.validity=TRUE;
                stage_decode.input_buffer.src1_RorP=P;
                printf("Result copied through forwarding bus\n");
                //free forward bus
                bus3.result = -1;
                bus3.register_address = -1;
            } else {
                stage_decode.input_buffer.source_register1 = register_file1.dummy;
                stage_decode.input_buffer.validity=FALSE;
                stage_decode.input_buffer.src1_RorP=-1;
                stage_decode.input_buffer.source_register1.address = reg_address2;
            }
        }

//        stage_decode.input_buffer.source_register1 = register_file1.reg[reg_address1];
//        stage_decode.input_buffer.source_register1.address = reg_address1;
//        register_file1.reg[reg_address1].address = reg_address1;
//        if (register_file1.reg[reg_address1].status == FALSE) {
//            if (bus1.register_address == register_file1.reg[reg_address2].address) {
//                stage_decode.input_buffer.source_register1.value = bus1.result;
//                printf("Result copied through forwarding bus\n");
//                //free forward bus
//                bus1.result = -1;
//                bus1.register_address = -1;
//            } else if (bus2.register_address == register_file1.reg[reg_address2].address) {
//                stage_decode.input_buffer.source_register1.value = bus2.result;
//                printf("Result copied through forwarding bus\n");
//                //free forward bus
//                bus2.result = -1;
//                bus2.register_address = -1;
//            } else if (bus3.register_address == register_file1.reg[reg_address2].address) {
//                stage_decode.input_buffer.source_register1.value = bus3.result;
//                printf("Result copied through forwarding bus\n");
//                //free forward bus
//                bus3.result = -1;
//                bus3.register_address = -1;
//            } else {
//                //update the stall flag
//                common.stalled = TRUE;
//                printf("\nStall is triggered in decode stage for register: R%d\n", reg_address1);
//                // Push NOP instruction
//                stage_decodeA.output_buffer.PC = 1;
//                strcpy(stage_decodeA.output_buffer.mnemonic, "NOP");
//                return;
//            }
//        }
        sscanf(str3, "R%d", &reg_address2); // The register number is stored in the variable reg_number

        printf("The source 2 register is : R%d\n", reg_address2);

        for (int i = 0; i < 16; ++i) {
            if (RAT1[i].index_R == reg_address3 && RAT1[i].RorP == R) {
                stage_decode.input_buffer.source_register2 = register_file1.reg[reg_address3];
                stage_decode.input_buffer.source_register2.address = reg_address3;
                stage_decode.input_buffer.validity = TRUE;
                stage_decode.input_buffer.src2_RorP = R;
                break;
            } else if (RAT1[i].index_R == reg_address3 && RAT1[i].RorP == P) {
                stage_decode.input_buffer.source_register2 = physical_RF.prf[RAT1[i].index_P];
                stage_decode.input_buffer.source_register2.address = RAT1[i].index_P;
                stage_decode.input_buffer.validity = TRUE;
                stage_decode.input_buffer.src2_RorP = P;
                break;
            }
        }
        if(stage_decode.input_buffer.validity!=TRUE) {
            if (bus1.register_address == reg_address3) {
                stage_decode.input_buffer.source_register2.value = bus1.result;
                stage_decode.input_buffer.source_register2.address = reg_address3;
                stage_decode.input_buffer.validity=TRUE;
                stage_decode.input_buffer.src2_RorP=P;
                printf("Result copied through forwarding bus\n");
                //free forward bus
                bus1.result = -1;
                bus1.register_address = -1;
            } else if (bus2.register_address == reg_address3) {
                stage_decode.input_buffer.source_register2.value = bus2.result;
                stage_decode.input_buffer.source_register2.address = reg_address3;
                stage_decode.input_buffer.validity=TRUE;
                stage_decode.input_buffer.src2_RorP=P;
                printf("Result copied through forwarding bus\n");
                //free forward bus
                bus2.result = -1;
                bus2.register_address = -1;
            } else if (bus3.register_address == reg_address3) {
                stage_decode.input_buffer.source_register2.value = bus3.result;
                stage_decode.input_buffer.source_register2.address = reg_address3;
                stage_decode.input_buffer.validity=TRUE;
                stage_decode.input_buffer.src2_RorP=P;
                printf("Result copied through forwarding bus\n");
                //free forward bus
                bus3.result = -1;
                bus3.register_address = -1;
            } else {
                stage_decode.input_buffer.source_register2 = register_file1.dummy;
                stage_decode.input_buffer.validity=FALSE;
                stage_decode.input_buffer.src2_RorP=-1;
                stage_decode.input_buffer.source_register2.address = reg_address3;
            }
        }
//        stage_decode.input_buffer.source_register2 = register_file1.reg[reg_address2];
//        stage_decode.input_buffer.source_register2.address = reg_address2;
//        register_file1.reg[reg_address2].address = reg_address2;
//        if (register_file1.reg[reg_address2].status == FALSE) {
//            if (bus1.register_address == register_file1.reg[reg_address2].address) {
//                stage_decode.input_buffer.source_register1.value = bus1.result;
//                printf("Result copied through forwarding bus\n");
//                //free forward bus
//                bus1.result = -1;
//                bus1.register_address = -1;
//            } else if (bus2.register_address == register_file1.reg[reg_address2].address) {
//                stage_decode.input_buffer.source_register1.value = bus2.result;
//                printf("Result copied through forwarding bus\n");
//                //free forward bus
//                bus2.result = -1;
//                bus2.register_address = -1;
//            } else if (bus3.register_address == register_file1.reg[reg_address2].address) {
//                stage_decode.input_buffer.source_register1.value = bus3.result;
//                printf("Result copied through forwarding bus\n");
//                //free forward bus
//                bus3.result = -1;
//                bus3.register_address = -1;
//            } else {
//                //update the stall flag
//                common.stalled = TRUE;
//                printf("\nStall is triggered in decode stage for register: R%d\n", reg_address2);
//                // Push NOP instruction
//                stage_decodeA.output_buffer.PC = 1;
//                strcpy(stage_decodeA.output_buffer.mnemonic, "NOP");
//                return;
//            }
//        }
        //register_file1.reg[reg_address2].value = 0;

        strncpy(string_copy1, str4, 1);
        if (string_copy1[0] == '#') {
            sscanf(str4, "#%d", &literal);
            printf("The literal value is : %d\n", literal);
            stage_decode.input_buffer.literal = literal;
        } else
            printf("Error in STORE instruction\n");

//        if (execute_int_free == FALSE)
//            decode_stalled_flag = TRUE;
//        output_buffer_decodeA();


    } else if (!strcmp(stage_decode.input_buffer.mnemonic, "BNZ")) {
        printf("BNZ inst fetched \n");
        strncpy(string_copy1, str2, 1);
        if (string_copy1[0] == '#') {
            sscanf(str4, "#%d", &literal);
            printf("The literal value is : %d\n", literal);
            stage_decode.input_buffer.literal = literal;
        } else
            printf("Error in BNZ instruction\n");

//        if (execute_int_free == FALSE)
//            decode_stalled_flag = TRUE;
      //  output_buffer_decodeA();

    } else if (!strcmp(stage_decode.input_buffer.mnemonic, "BZ")) {
        printf("BZ inst fetched \n");
        strncpy(string_copy1, str2, 1);
        if (string_copy1[0] == '#') {
            sscanf(str4, "#%d", &literal);
            printf("The literal value is : %d\n", literal);
            stage_decode.input_buffer.literal = literal;
        } else
            printf("Error in BZ instruction\n");

//        if (execute_int_free == FALSE)
//            decode_stalled_flag = TRUE;
      //  output_buffer_decodeA();

    } else if (!strcmp(stage_decode.input_buffer.mnemonic, "JUMP")) {

        sscanf(str2, "R%d", &reg_address1); // The register number is stored in the variable reg_number

        printf("The source 1 register is : R%d\n", reg_address1);

        for (int i = 0; i < 16; ++i) {
            if (RAT1[i].index_R == reg_address2 && RAT1[i].RorP == R) {
                stage_decode.input_buffer.source_register1 = register_file1.reg[reg_address2];
                stage_decode.input_buffer.source_register1.address = reg_address2;
                stage_decode.input_buffer.validity = TRUE;
                stage_decode.input_buffer.src1_RorP = R;
                break;
            } else if (RAT1[i].index_R == reg_address2 && RAT1[i].RorP == P) {
                stage_decode.input_buffer.source_register1 = physical_RF.prf[RAT1[i].index_P];
                stage_decode.input_buffer.source_register1.address = RAT1[i].index_P;
                stage_decode.input_buffer.validity = TRUE;
                stage_decode.input_buffer.src1_RorP = P;
                break;
            }
        }
        if(stage_decode.input_buffer.validity!=TRUE) {
            if (bus1.register_address == reg_address2) {
                stage_decode.input_buffer.source_register1.value = bus1.result;
                stage_decode.input_buffer.source_register1.address = reg_address2;
                stage_decode.input_buffer.validity=TRUE;
                stage_decode.input_buffer.src1_RorP=P;
                printf("Result copied through forwarding bus\n");
                //free forward bus
                bus1.result = -1;
                bus1.register_address = -1;
            } else if (bus2.register_address == reg_address2) {
                stage_decode.input_buffer.source_register1.value = bus2.result;
                stage_decode.input_buffer.source_register1.address = reg_address2;
                stage_decode.input_buffer.validity=TRUE;
                stage_decode.input_buffer.src1_RorP=P;
                printf("Result copied through forwarding bus\n");
                //free forward bus
                bus2.result = -1;
                bus2.register_address = -1;
            } else if (bus3.register_address == reg_address2) {
                stage_decode.input_buffer.source_register1.value = bus3.result;
                stage_decode.input_buffer.source_register1.address = reg_address2;
                stage_decode.input_buffer.validity=TRUE;
                stage_decode.input_buffer.src1_RorP=P;
                printf("Result copied through forwarding bus\n");
                //free forward bus
                bus3.result = -1;
                bus3.register_address = -1;
            } else {
                stage_decode.input_buffer.source_register1 = register_file1.dummy;
                stage_decode.input_buffer.validity=FALSE;
                stage_decode.input_buffer.src1_RorP=-1;
                stage_decode.input_buffer.source_register1.address = reg_address2;
            }
        }
//        stage_decode.input_buffer.source_register1 = register_file1.reg[reg_address1];
//        stage_decode.input_buffer.source_register1.address = reg_address1;
//        register_file1.reg[reg_address1].address = reg_address1;
//        if (register_file1.reg[reg_address1].status == FALSE) {
//            if (bus1.register_address == register_file1.reg[reg_address1].address) {
//                stage_decode.input_buffer.source_register1.value = bus1.result;
//                printf("Result copied through forwarding bus\n");
//                //free forward bus
//                bus1.result = -1;
//                bus1.register_address = -1;
//            } else if (bus2.register_address == register_file1.reg[reg_address1].address) {
//                stage_decode.input_buffer.source_register1.value = bus2.result;
//                printf("Result copied through forwarding bus\n");
//                //free forward bus
//                bus2.result = -1;
//                bus2.register_address = -1;
//            } else if (bus3.register_address == register_file1.reg[reg_address1].address) {
//                stage_decode.input_buffer.source_register1.value = bus3.result;
//                printf("Result copied through forwarding bus\n");
//                //free forward bus
//                bus3.result = -1;
//                bus3.register_address = -1;
//            } else {
//                //update the stall flag
//                common.stalled = TRUE;
//                printf("\nStall is triggered in decode stage for register: R%d\n", reg_address1);
//                // Push NOP instruction
//                stage_decodeA.output_buffer.PC = 1;
//                strcpy(stage_decodeA.output_buffer.mnemonic, "NOP");
//                return;
//            }
//        }

        strncpy(string_copy1, str3, 1);
        if (string_copy1[0] == '#') {
            sscanf(str3, "#%d", &literal);
            printf("The literal value is : %d\n", literal);
            stage_decode.input_buffer.literal = literal;
        } else
            printf("Error in STORE instruction\n");

//        if (execute_int_free == FALSE)
//            decode_stalled_flag = TRUE;
//        output_buffer_decodeA();


    } else if (!strcmp(stage_decode.input_buffer.mnemonic, "JAL")) {
        //Source Register 1
        strncpy(string_copy1, str3, 1);
        if (string_copy1[0] == 'R') {
            sscanf(str3, "R%d", &reg_address2); // The register number is stored in the variable reg_number

            printf("The source register is : R%d\n", reg_address2);
            //fetch_stage.input_buffer.source_register1=register_file1.reg[greg_number];
            //update the register in

            for (int i = 0; i < 16; ++i) {
                if (RAT1[i].index_R == reg_address2 && RAT1[i].RorP == R) {
                    stage_decode.input_buffer.source_register1 = register_file1.reg[reg_address2];
                    stage_decode.input_buffer.source_register1.address = reg_address2;
                    stage_decode.input_buffer.validity = TRUE;
                    stage_decode.input_buffer.src1_RorP = R;
                    break;
                } else if (RAT1[i].index_R == reg_address2 && RAT1[i].RorP == P) {
                    stage_decode.input_buffer.source_register1 = physical_RF.prf[RAT1[i].index_P];
                    stage_decode.input_buffer.source_register1.address = RAT1[i].index_P;
                    stage_decode.input_buffer.validity = TRUE;
                    stage_decode.input_buffer.src1_RorP = P;
                    break;
                }
            }
            if(stage_decode.input_buffer.validity!=TRUE) {
                if (bus1.register_address == reg_address2) {
                    stage_decode.input_buffer.source_register1.value = bus1.result;
                    stage_decode.input_buffer.source_register1.address = reg_address2;
                    stage_decode.input_buffer.validity=TRUE;
                    stage_decode.input_buffer.src1_RorP=P;
                    printf("Result copied through forwarding bus\n");
                    //free forward bus
                    bus1.result = -1;
                    bus1.register_address = -1;
                } else if (bus2.register_address == reg_address2) {
                    stage_decode.input_buffer.source_register1.value = bus2.result;
                    stage_decode.input_buffer.source_register1.address = reg_address2;
                    stage_decode.input_buffer.validity=TRUE;
                    stage_decode.input_buffer.src1_RorP=P;
                    printf("Result copied through forwarding bus\n");
                    //free forward bus
                    bus2.result = -1;
                    bus2.register_address = -1;
                } else if (bus3.register_address == reg_address2) {
                    stage_decode.input_buffer.source_register1.value = bus3.result;
                    stage_decode.input_buffer.source_register1.address = reg_address2;
                    stage_decode.input_buffer.validity=TRUE;
                    stage_decode.input_buffer.src1_RorP=P;
                    printf("Result copied through forwarding bus\n");
                    //free forward bus
                    bus3.result = -1;
                    bus3.register_address = -1;
                } else {
                    stage_decode.input_buffer.source_register1 = register_file1.dummy;
                    stage_decode.input_buffer.validity=FALSE;
                    stage_decode.input_buffer.src1_RorP=-1;
                    stage_decode.input_buffer.source_register1.address = reg_address2;
                }
            }
//            stage_decode.input_buffer.source_register1 = register_file1.reg[reg_address2];
//            stage_decode.input_buffer.source_register1.address = reg_address2;
//            register_file1.reg[reg_address2].address = reg_address2;
//            if (register_file1.reg[reg_address2].status == FALSE) {
//                if (bus1.register_address == register_file1.reg[reg_address2].address) {
//                    stage_decode.input_buffer.source_register1.value = bus1.result;
//                    printf("Result copied through forwarding bus\n");
//                    //free forward bus
//                    bus1.result = -1;
//                    bus1.register_address = -1;
//                } else if (bus2.register_address == register_file1.reg[reg_address2].address) {
//                    stage_decode.input_buffer.source_register1.value = bus2.result;
//                    printf("Result copied through forwarding bus\n");
//                    //free forward bus
//                    bus2.result = -1;
//                    bus2.register_address = -1;
//                } else if (bus3.register_address == register_file1.reg[reg_address2].address) {
//                    stage_decode.input_buffer.source_register1.value = bus3.result;
//                    printf("Result copied through forwarding bus\n");
//                    //free forward bus
//                    bus3.result = -1;
//                    bus3.register_address = -1;
//                } else {
//                    //update the stall flag
//                    common.stalled = TRUE;
//                    printf("\nStall is triggered in decode stage for register: R%d\n", reg_address2);
//                    stage_decodeA.output_buffer.PC = 1;
//                    strcpy(stage_decodeA.output_buffer.mnemonic, "NOP");
//                    return;
//                }
//            }
        } else if (string_copy1[0] == '#') {
            sscanf(str3, "#%d", &literal);
            printf("The literal value is : %d\n", literal);
            stage_decode.input_buffer.literal = literal;
        }

        //Source Register 2
        strncpy(string_copy2, str4, 1);
        if (string_copy2[0] == 'R') {
            sscanf(str4, "R%d", &reg_address3); // The register number is stored in the variable reg_number

            printf("The source register is : R%d\n", reg_address3);

            for (int i = 0; i < 16; ++i) {
                if (RAT1[i].index_R == reg_address3 && RAT1[i].RorP == R) {
                    stage_decode.input_buffer.source_register2 = register_file1.reg[reg_address3];
                    stage_decode.input_buffer.source_register2.address = reg_address3;
                    stage_decode.input_buffer.validity = TRUE;
                    stage_decode.input_buffer.src2_RorP = R;
                    break;
                } else if (RAT1[i].index_R == reg_address3 && RAT1[i].RorP == P) {
                    stage_decode.input_buffer.source_register2 = physical_RF.prf[RAT1[i].index_P];
                    stage_decode.input_buffer.source_register2.address = RAT1[i].index_P;
                    stage_decode.input_buffer.validity = TRUE;
                    stage_decode.input_buffer.src2_RorP = P;
                    break;
                }
            }
            if(stage_decode.input_buffer.validity!=TRUE) {
                if (bus1.register_address == reg_address3) {
                    stage_decode.input_buffer.source_register2.value = bus1.result;
                    stage_decode.input_buffer.source_register2.address = reg_address3;
                    stage_decode.input_buffer.validity=TRUE;
                    stage_decode.input_buffer.src2_RorP=P;
                    printf("Result copied through forwarding bus\n");
                    //free forward bus
                    bus1.result = -1;
                    bus1.register_address = -1;
                } else if (bus2.register_address == reg_address3) {
                    stage_decode.input_buffer.source_register2.value = bus2.result;
                    stage_decode.input_buffer.source_register2.address = reg_address3;
                    stage_decode.input_buffer.validity=TRUE;
                    stage_decode.input_buffer.src2_RorP=P;
                    printf("Result copied through forwarding bus\n");
                    //free forward bus
                    bus2.result = -1;
                    bus2.register_address = -1;
                } else if (bus3.register_address == reg_address3) {
                    stage_decode.input_buffer.source_register2.value = bus3.result;
                    stage_decode.input_buffer.source_register2.address = reg_address3;
                    stage_decode.input_buffer.validity=TRUE;
                    stage_decode.input_buffer.src2_RorP=P;
                    printf("Result copied through forwarding bus\n");
                    //free forward bus
                    bus3.result = -1;
                    bus3.register_address = -1;
                } else {
                    stage_decode.input_buffer.source_register2 = register_file1.dummy;
                    stage_decode.input_buffer.validity=FALSE;
                    stage_decode.input_buffer.src2_RorP=-1;
                    stage_decode.input_buffer.source_register2.address = reg_address3;
                }
            }
//            stage_decode.input_buffer.source_register2 = register_file1.reg[reg_address3];
//            stage_decode.input_buffer.source_register2.address = reg_address3;
//            register_file1.reg[reg_address3].address = reg_address3;
//            if (register_file1.reg[reg_address3].status == FALSE) {
//                if (bus1.register_address == register_file1.reg[reg_address2].address) {
//                    stage_decode.input_buffer.source_register1.value = bus1.result;
//                    printf("Result copied through forwarding bus\n");
//                    //free forward bus
//                    bus1.result = -1;
//                    bus1.register_address = -1;
//                } else if (bus2.register_address == register_file1.reg[reg_address2].address) {
//                    stage_decode.input_buffer.source_register1.value = bus2.result;
//                    printf("Result copied through forwarding bus\n");
//                    //free forward bus
//                    bus2.result = -1;
//                    bus2.register_address = -1;
//                } else if (bus3.register_address == register_file1.reg[reg_address2].address) {
//                    stage_decode.input_buffer.source_register1.value = bus3.result;
//                    printf("Result copied through forwarding bus\n");
//                    //free forward bus
//                    bus3.result = -1;
//                    bus3.register_address = -1;
//                } else {
//                    //update the stall flag
//                    common.stalled = TRUE;
//                    printf("\nStall is triggered in decode stage for register: R%d\n", reg_address3);
//                    // Push NOP instruction
//                    stage_decodeA.output_buffer.PC = 1;
//                    strcpy(stage_decodeA.output_buffer.mnemonic, "NOP");
//                    return;
//                }
//
//            }
        } else if ((string_copy2[0]) == '#') {
            sscanf(str4, "#%d", &literal);
            printf("The literal value is : %d\n", literal);
            stage_decode.input_buffer.literal = literal;
        } else
            printf("Error in Arithmetic instruction\n");

        //Destination Register
        sscanf(str2, "R%d", &reg_address1); // The register number is stored in the variable reg_number

        printf("The destination register is : R%d\n", reg_address1);

        register_file1.reg[reg_address1].address = reg_address1;
        for (int i = 0; i < 32; ++i) {
            if (physical_RF.prf[i].status == FREE) {
                physical_index = i;
                printf("the register R[%d] is converted to P[%d]\n", reg_address1, i);
                stage_decode.input_buffer.dest_register = physical_RF.prf[i];
                stage_decode.input_buffer.dest_register.address = i;
                RAT1[reg_address1].RorP = P;
                stage_decode.input_buffer.dest_RorP=P;
                RAT1[reg_address1].index_P = i;
                physical_RF.prf[i].status=ALLOCATED;

                break;
            }
        }
        register_file1.reg[reg_address1].status = FALSE;


//        stage_decode.input_buffer.dest_register = register_file1.reg[reg_address1];
//        stage_decode.input_buffer.dest_register.address = reg_address1;
//        register_file1.reg[reg_address1].address = reg_address1;
//        if (register_file1.reg[reg_address2].status == FALSE) {
//            if (bus1.register_address == register_file1.reg[reg_address2].address) {
//                stage_decode.input_buffer.source_register1.value = bus1.result;
//                printf("Result copied through forwarding bus\n");
//                //free forward bus
//                bus1.result = -1;
//                bus1.register_address = -1;
//            } else if (bus2.register_address == register_file1.reg[reg_address2].address) {
//                stage_decode.input_buffer.source_register1.value = bus2.result;
//                printf("Result copied through forwarding bus\n");
//                //free forward bus
//                bus2.result = -1;
//                bus2.register_address = -1;
//            } else if (bus3.register_address == register_file1.reg[reg_address2].address) {
//                stage_decode.input_buffer.source_register1.value = bus3.result;
//                printf("Result copied through forwarding bus\n");
//                //free forward bus
//                bus3.result = -1;
//                bus3.register_address = -1;
//            } else {
//                //update the stall flag
//                common.stalled = TRUE;
//                printf("\nStall is triggered in decode stage for register: R%d\n", reg_address2);
//                stage_decodeA.output_buffer.PC = 1;
//                strcpy(stage_decodeA.output_buffer.mnemonic, "NOP");
//                return;
//            }
//        } else
            // set the register invalid flag so that the other instructions cannot access the register until set in WB stage
         //   register_file1.reg[reg_address1].status = FALSE; //0=INVALID

//        if (execute_int_free == FALSE)
//            decode_stalled_flag = TRUE;
//        output_buffer_decodeA();

    }
    issue_queue_decode();
    create_ROB();
    output_buffer_decode();
}

void issue_queue_decode(void){
    for (int i = 0; i < 32; ++i) {
        if(issue_queue_struct[i].status_of_allocation == FREE){
            issue_queue_struct[i].src1_RorP=stage_decode.input_buffer.src1_RorP;
            issue_queue_struct[i].src1_value=stage_decode.input_buffer.source_register1.value;
            issue_queue_struct[i].src1_status=stage_decode.input_buffer.source_register1.status;
            issue_queue_struct[i].src1_index=stage_decode.input_buffer.source_register1.address;
            issue_queue_struct[i].src2_RorP=stage_decode.input_buffer.src2_RorP;
            issue_queue_struct[i].src2_value=stage_decode.input_buffer.source_register2.value;
            issue_queue_struct[i].src2_status=stage_decode.input_buffer.source_register2.status;
            issue_queue_struct[i].src2_index=stage_decode.input_buffer.source_register2.address;
            issue_queue_struct[i].dest_RorP=stage_decode.input_buffer.dest_RorP;
            issue_queue_struct[i].dest_index=stage_decode.input_buffer.dest_register.address;
            issue_queue_struct[i].clock_cycle=stats1.cycle;
            strcpy(issue_queue_struct[i].inst_opcode,stage_decode.input_buffer.mnemonic);
            issue_queue_struct[i].status_of_allocation=ALLOCATED;
            break;
        }
    }
}
void issue_queue(void) {
    if ((stage_decode.output_buffer.PC != 0) && (stage_issue_queue.stalled == FALSE)) {
        input_buffer_iq();
        for (int i = 0; i < 32; ++i) {
            if (issue_queue_struct[i].status_of_allocation == ALLOCATED) {
                if (issue_queue_struct[i].src1_value == register_file1.dummy.value) {
                    if (bus1.register_address == issue_queue_struct[i].src1_index) {
                        issue_queue_struct[i].src1_value = bus1.result;
                        stage_issue_queue.input_buffer.validity = TRUE;
                        printf("Result copied through forwarding bus\n");
                    } else if (bus2.register_address == issue_queue_struct[i].src1_index) {
                        issue_queue_struct[i].src1_value = bus1.result;
                        stage_issue_queue.input_buffer.validity = TRUE;
                        printf("Result copied through forwarding bus\n");
                    } else if (bus3.register_address == issue_queue_struct[i].src1_index) {
                        issue_queue_struct[i].src1_value = bus1.result;
                        stage_issue_queue.input_buffer.validity = TRUE;
                        printf("Result copied through forwarding bus\n");
                    }
                }
                if (issue_queue_struct[i].src2_value == register_file1.dummy.value) {
                    if (bus1.register_address == issue_queue_struct[i].src2_index) {
                        issue_queue_struct[i].src2_value = bus1.result;
                        stage_issue_queue.input_buffer.validity = TRUE;
                        printf("Result copied through forwarding bus\n");
                    } else if (bus2.register_address == issue_queue_struct[i].src2_index) {
                        issue_queue_struct[i].src2_value = bus1.result;
                        stage_issue_queue.input_buffer.validity = TRUE;
                        printf("Result copied through forwarding bus\n");
                    } else if (bus3.register_address == issue_queue_struct[i].src2_index) {
                        issue_queue_struct[i].src2_value = bus1.result;
                        stage_issue_queue.input_buffer.validity = TRUE;
                        printf("Result copied through forwarding bus\n");
                    }
                }
            }

        }
        //wake up
        for (int j = 0; j < 32; ++j) {
            if (issue_queue_struct[j].status_of_allocation == ALLOCATED) {
                if ((!strcmp(issue_queue_struct[j].inst_opcode, "LOAD"))||
                    (!strcmp(issue_queue_struct[j].inst_opcode, "JUMP"))||
                    (!strcmp(issue_queue_struct[j].inst_opcode, "JAL"))||
                    (!strcmp(issue_queue_struct[j].inst_opcode, "MOVC"))){
                    if (issue_queue_struct[j].src1_value != register_file1.dummy.value) {
                        issue_queue_struct[j].wakeup = YES;
                    } else
                        issue_queue_struct[j].wakeup = NO;
                } else if ((!strcmp(issue_queue_struct[j].inst_opcode, "ADD")) ||
                           (!strcmp(issue_queue_struct[j].inst_opcode, "SUB")) ||
                           (!strcmp(issue_queue_struct[j].inst_opcode, "MUL")) ||
                           (!strcmp(issue_queue_struct[j].inst_opcode, "DIV")) ||
                           (!strcmp(issue_queue_struct[j].inst_opcode, "STORE"))) {
                    if ((issue_queue_struct[j].src1_value != register_file1.dummy.value) &&
                        (issue_queue_struct[j].src2_value != register_file1.dummy.value)) {
                        issue_queue_struct[j].wakeup = YES;
                    } else
                        issue_queue_struct[j].wakeup = NO;
                }
            }
        }
        //selection
        int selection_number=0,lowest_cycle=100,FU_selection=0;
        for (int k = 0; k < 32; ++k) {
            if(issue_queue_struct[k].wakeup==YES){
                issue_queue_selection.selection_logic[selection_number]=issue_queue_struct[k];
                selection_number++;
            }
        }
        for (int m = 0; m < 32; ++m) {
            if((!strcmp(issue_queue_selection.selection_logic[m].inst_opcode,"ADD"))||
               (!strcmp(issue_queue_selection.selection_logic[m].inst_opcode, "LOAD"))||
               (!strcmp(issue_queue_selection.selection_logic[m].inst_opcode, "SUB"))||
               (!strcmp(issue_queue_selection.selection_logic[m].inst_opcode, "MOVC"))||
               (!strcmp(issue_queue_selection.selection_logic[m].inst_opcode, "JAL"))||
               (!strcmp(issue_queue_selection.selection_logic[m].inst_opcode, "STORE"))||
               (!strcmp(issue_queue_selection.selection_logic[m].inst_opcode, "BNZ"))||
               (!strcmp(issue_queue_selection.selection_logic[m].inst_opcode, "BZ"))||
               (!strcmp(issue_queue_selection.selection_logic[m].inst_opcode, "JUMP"))){
                issue_queue_FU_selection.selection_FU[FU_selection]=issue_queue_selection.selection_logic[m];
            }
            else if((!strcmp(issue_queue_selection.selection_logic[m].inst_opcode,"MUL"))){
                issue_queue_FU_selection.selection_FU[FU_selection]=issue_queue_selection.selection_logic[m];
            }
            else if((!strcmp(issue_queue_selection.selection_logic[m].inst_opcode,"DIV"))||
                    (!strcmp(issue_queue_selection.selection_logic[m].inst_opcode,"HALT"))){
                issue_queue_FU_selection.selection_FU[FU_selection]=issue_queue_selection.selection_logic[m];
            }
        }
        for (int l = 0; l < 32; ++l) {
            if((issue_queue_FU_selection.selection_FU[l].clock_cycle)<lowest_cycle)
                lowest_cycle=issue_queue_FU_selection.selection_FU[l].clock_cycle;
        }

        for (int n = 0; n < 32; ++n) {
            if((issue_queue_FU_selection.selection_FU[n].clock_cycle==lowest_cycle)&&issue_queue_FU_selection.selection_FU[n].clock_cycle!=1000) {
                if ((!strcmp(issue_queue_selection.selection_logic[n].inst_opcode, "ADD")) ||
                    (!strcmp(issue_queue_selection.selection_logic[n].inst_opcode, "LOAD")) ||
                    (!strcmp(issue_queue_selection.selection_logic[n].inst_opcode, "SUB")) ||
                    (!strcmp(issue_queue_selection.selection_logic[n].inst_opcode, "MOVC")) ||
                    (!strcmp(issue_queue_selection.selection_logic[n].inst_opcode, "JAL")) ||
                    (!strcmp(issue_queue_selection.selection_logic[n].inst_opcode, "STORE")) ||
                    (!strcmp(issue_queue_selection.selection_logic[n].inst_opcode, "BNZ")) ||
                    (!strcmp(issue_queue_selection.selection_logic[n].inst_opcode, "BZ")) ||
                    (!strcmp(issue_queue_selection.selection_logic[n].inst_opcode, "JUMP"))) {
                    output_buffer_iq_intFU(n);
                }
                if ((!strcmp(issue_queue_selection.selection_logic[n].inst_opcode, "MUL"))) {
                    output_buffer_iq_MUL(n);
                }
                if ((!strcmp(issue_queue_selection.selection_logic[n].inst_opcode, "DIV")) ||
                           (!strcmp(issue_queue_selection.selection_logic[n].inst_opcode, "HALT"))) {
                    output_buffer_iq_DIV(n);

                }
            }
        }
        for (int i1 = 0; i1 <32 ; ++i1) {
            printf("%s,%d,%d",issue_queue_struct[i1].inst_opcode,issue_queue_struct[i1].dest_index,issue_queue_struct[i1].src1_value);
            printf("%d\n",issue_queue_struct[i1].src2_value);

        }
    }
}

void input_buffer_iq(void){
    stage_issue_queue.input_buffer.PC = stage_decode.output_buffer.PC;
    strcpy(stage_issue_queue.input_buffer.instruction_string,stage_decode.output_buffer.instruction_string);
    strcpy(stage_issue_queue.input_buffer.mnemonic,stage_decode.output_buffer.mnemonic);
    stage_issue_queue.input_buffer.dest_register = stage_decode.output_buffer.dest_register;
    stage_issue_queue.input_buffer.source_register1 = stage_decode.output_buffer.source_register1;
    stage_issue_queue.input_buffer.source_register2 = stage_decode.output_buffer.source_register2;
    stage_issue_queue.input_buffer.literal = stage_decode.output_buffer.literal;

    //delete data from the input buffer
    stage_decode.output_buffer.PC = 0;
    memset(stage_decode.output_buffer.instruction_string, 0, sizeof(stage_decode.output_buffer.instruction_string));
    memset(stage_decode.output_buffer.mnemonic, 0, sizeof(stage_decode.output_buffer.mnemonic));
    memset(&stage_decode.output_buffer.dest_register, 0, sizeof(stage_decode.output_buffer.dest_register));
    memset(&stage_decode.output_buffer.source_register1, 0, sizeof(stage_decode.output_buffer.source_register1));
    memset(&stage_decode.output_buffer.source_register2, 0, sizeof(stage_decode.output_buffer.source_register2));
    stage_decode.output_buffer.literal = 0;
}


void output_buffer_iq_intFU(int num){
    stage_iq_intFU.output_buffer.selected_inst=num;
    stage_iq_intFU.output_buffer.PC = stage_issue_queue.input_buffer.PC;
    strcpy(stage_iq_intFU.output_buffer.instruction_string, stage_issue_queue.input_buffer.instruction_string);
    strcpy(stage_iq_intFU.output_buffer.mnemonic, issue_queue_FU_selection.selection_FU[num].inst_opcode);
    stage_iq_intFU.output_buffer.dest_register.address=issue_queue_FU_selection.selection_FU[num].dest_index;
    stage_iq_intFU.output_buffer.dest_RorP=issue_queue_FU_selection.selection_FU[num].dest_RorP;
    stage_iq_intFU.output_buffer.source_register1.address=issue_queue_FU_selection.selection_FU[num].src1_index;
    stage_iq_intFU.output_buffer.source_register1.value=issue_queue_FU_selection.selection_FU[num].src1_value;
    stage_iq_intFU.output_buffer.source_register1.status=issue_queue_FU_selection.selection_FU[num].src1_status;
    stage_iq_intFU.output_buffer.src1_RorP=issue_queue_FU_selection.selection_FU[num].src1_RorP;
    stage_iq_intFU.output_buffer.source_register2.address=issue_queue_FU_selection.selection_FU[num].src2_index;
    stage_iq_intFU.output_buffer.source_register2.value=issue_queue_FU_selection.selection_FU[num].src2_value;
    stage_iq_intFU.output_buffer.source_register2.status=issue_queue_FU_selection.selection_FU[num].src2_status;
    stage_iq_intFU.output_buffer.src2_RorP=issue_queue_FU_selection.selection_FU[num].src2_RorP;
    stage_iq_intFU.output_buffer.literal = stage_issue_queue.input_buffer.literal;

    //delete data from the input buffer
    stage_decode.input_buffer.PC = 0;
    memset(stage_issue_queue.input_buffer.instruction_string, 0, sizeof(stage_issue_queue.input_buffer.instruction_string));
    memset(stage_issue_queue.input_buffer.mnemonic, 0, sizeof(stage_issue_queue.input_buffer.mnemonic));
    memset(&stage_issue_queue.input_buffer.dest_register, 0, sizeof(stage_issue_queue.input_buffer.dest_register));
    memset(&stage_issue_queue.input_buffer.source_register1, 0, sizeof(stage_issue_queue.input_buffer.source_register1));
    memset(&stage_issue_queue.input_buffer.source_register2, 0, sizeof(stage_issue_queue.input_buffer.source_register2));
    stage_issue_queue.input_buffer.literal = 0;


}

void output_buffer_iq_MUL(int num){
    stage_iq_MUL.output_buffer.selected_inst=num;
    stage_iq_MUL.output_buffer.PC = stage_issue_queue.input_buffer.PC;
    strcpy(stage_iq_MUL.output_buffer.instruction_string, stage_issue_queue.input_buffer.instruction_string);
    strcpy(stage_iq_MUL.output_buffer.mnemonic, issue_queue_FU_selection.selection_FU[num].inst_opcode);
    stage_iq_MUL.output_buffer.dest_register.address=issue_queue_FU_selection.selection_FU[num].dest_index;
    stage_iq_MUL.output_buffer.dest_RorP=issue_queue_FU_selection.selection_FU[num].dest_RorP;
    stage_iq_MUL.output_buffer.source_register1.address=issue_queue_FU_selection.selection_FU[num].src1_index;
    stage_iq_MUL.output_buffer.source_register1.value=issue_queue_FU_selection.selection_FU[num].src1_value;
    stage_iq_MUL.output_buffer.source_register1.status=issue_queue_FU_selection.selection_FU[num].src1_status;
    stage_iq_MUL.output_buffer.src1_RorP=issue_queue_FU_selection.selection_FU[num].src1_RorP;
    stage_iq_MUL.output_buffer.source_register2.address=issue_queue_FU_selection.selection_FU[num].src2_index;
    stage_iq_MUL.output_buffer.source_register2.value=issue_queue_FU_selection.selection_FU[num].src2_value;
    stage_iq_MUL.output_buffer.source_register2.status=issue_queue_FU_selection.selection_FU[num].src2_status;
    stage_iq_MUL.output_buffer.src2_RorP=issue_queue_FU_selection.selection_FU[num].src2_RorP;
    stage_iq_MUL.output_buffer.literal = stage_issue_queue.input_buffer.literal;

    //delete data from the input buffer
    stage_issue_queue.input_buffer.PC = 0;
    memset(stage_issue_queue.input_buffer.instruction_string, 0, sizeof(stage_issue_queue.input_buffer.instruction_string));
    memset(stage_issue_queue.input_buffer.mnemonic, 0, sizeof(stage_issue_queue.input_buffer.mnemonic));
    memset(&stage_issue_queue.input_buffer.dest_register, 0, sizeof(stage_issue_queue.input_buffer.dest_register));
    memset(&stage_issue_queue.input_buffer.source_register1, 0, sizeof(stage_issue_queue.input_buffer.source_register1));
    memset(&stage_issue_queue.input_buffer.source_register2, 0, sizeof(stage_issue_queue.input_buffer.source_register2));
    stage_issue_queue.input_buffer.literal = 0;

}

void output_buffer_iq_DIV(int num){
    stage_iq_DIV.output_buffer.selected_inst=num;
    stage_iq_DIV.output_buffer.PC = stage_issue_queue.input_buffer.PC;
    strcpy(stage_iq_DIV.output_buffer.instruction_string, stage_issue_queue.input_buffer.instruction_string);
    strcpy(stage_iq_DIV.output_buffer.mnemonic, issue_queue_FU_selection.selection_FU[num].inst_opcode);
    stage_iq_DIV.output_buffer.dest_register.address=issue_queue_FU_selection.selection_FU[num].dest_index;
    stage_iq_DIV.output_buffer.dest_RorP=issue_queue_FU_selection.selection_FU[num].dest_RorP;
    stage_iq_DIV.output_buffer.source_register1.address=issue_queue_FU_selection.selection_FU[num].src1_index;
    stage_iq_DIV.output_buffer.source_register1.value=issue_queue_FU_selection.selection_FU[num].src1_value;
    stage_iq_DIV.output_buffer.source_register1.status=issue_queue_FU_selection.selection_FU[num].src1_status;
    stage_iq_DIV.output_buffer.src1_RorP=issue_queue_FU_selection.selection_FU[num].src1_RorP;
    stage_iq_DIV.output_buffer.source_register2.address=issue_queue_FU_selection.selection_FU[num].src2_index;
    stage_iq_DIV.output_buffer.source_register2.value=issue_queue_FU_selection.selection_FU[num].src2_value;
    stage_iq_DIV.output_buffer.source_register2.status=issue_queue_FU_selection.selection_FU[num].src2_status;
    stage_iq_DIV.output_buffer.src2_RorP=issue_queue_FU_selection.selection_FU[num].src2_RorP;
    stage_iq_DIV.output_buffer.literal = stage_issue_queue.input_buffer.literal;

    //delete data from the input buffer
    stage_issue_queue.input_buffer.PC = 0;
    memset(stage_issue_queue.input_buffer.instruction_string, 0, sizeof(stage_issue_queue.input_buffer.instruction_string));
    memset(stage_issue_queue.input_buffer.mnemonic, 0, sizeof(stage_issue_queue.input_buffer.mnemonic));
    memset(&stage_issue_queue.input_buffer.dest_register, 0, sizeof(stage_issue_queue.input_buffer.dest_register));
    memset(&stage_issue_queue.input_buffer.source_register1, 0, sizeof(stage_issue_queue.input_buffer.source_register1));
    memset(&stage_issue_queue.input_buffer.source_register2, 0, sizeof(stage_issue_queue.input_buffer.source_register2));
    stage_issue_queue.input_buffer.literal = 0;

}


struct Queue* createQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;  // This is important, see the enqueue
    queue->array =malloc(queue->capacity * sizeof(struct ROB_struct));
    return queue;
}

int isFull(struct Queue* queue)
{  return (queue->size == queue->capacity);  }

int isEmpty(struct Queue* queue)
{  return (queue->size == 0); }

void enqueue(struct Queue* queue, struct ROB_struct ROB)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->array[queue->rear] = ROB.src1_RorP;
    queue->array[queue->rear] = ROB.src1_status;
    queue->array[queue->rear] = ROB.src1_value;
    queue->array[queue->rear] = ROB.src1_index;
    queue->array[queue->rear] = ROB.src2_RorP;
    queue->array[queue->rear] = ROB.src2_index;
    queue->array[queue->rear] = ROB.src2_status;
    queue->array[queue->rear] = ROB.src2_value;
    queue->array[queue->rear] = ROB.dest_RorP;
    queue->array[queue->rear] = ROB.dest_index;
    //strcpy(ROB.inst_opcode,queue->array[queue->rear]);
    queue->array[queue->rear] = ROB.ROB_index;
    queue->array[queue->rear] = ROB.LSQ_index;
    queue->array[queue->rear] = ROB.status_of_allocation_ROB;
    queue->size = queue->size + 1;
    printf("ROB entry enqueued to the queue\n");
}

int dequeue(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;

    int item=TRUE;
    return item;
}

int front(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}

int rear(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->rear];
}

void create_ROB(void){
    struct Queue* queue = createQueue(32);
    for (int i = 0; i < 32; ++i) {
        if(ROB[i].status_of_allocation_ROB==FREE){
            ROB[i].src1_RorP=stage_decode.input_buffer.src1_RorP;
            ROB[i].src1_index=stage_decode.input_buffer.source_register1.address;
            ROB[i].src1_value=stage_decode.input_buffer.source_register1.value;
            ROB[i].src1_status=stage_decode.input_buffer.source_register1.status;
            ROB[i].src2_value=stage_decode.input_buffer.source_register2.value;
            ROB[i].src2_status=stage_decode.input_buffer.source_register2.status;
            ROB[i].src2_index=stage_decode.input_buffer.source_register2.address;
            ROB[i].src2_RorP=stage_decode.input_buffer.src2_RorP;
            ROB[i].dest_index=stage_decode.input_buffer.dest_register.address;
            ROB[i].dest_RorP=stage_decode.input_buffer.dest_RorP;
            ROB[i].clock_cycle=stats1.cycle;
            strcpy(ROB[i].inst_opcode,stage_decode.input_buffer.mnemonic);
            ROB[i].status_of_allocation_ROB=ALLOCATED;
            enqueue(queue,ROB[i]);
            break;
        }
    }
}

void ROB_stage(void){
    for (int i = 0; i < 32; ++i) {
        if (ROB[i].status_of_allocation_ROB == ALLOCATED) {
            ROB[i].src1_RorP = stage_decode.input_buffer.src1_RorP;
            ROB[i].src1_index = stage_decode.input_buffer.source_register1.address;
            ROB[i].src1_value = stage_decode.input_buffer.source_register1.value;
            ROB[i].src1_status = stage_decode.input_buffer.source_register1.status;
            ROB[i].src2_value = stage_decode.input_buffer.source_register2.value;
            ROB[i].src2_status = stage_decode.input_buffer.source_register2.status;
            ROB[i].src2_index = stage_decode.input_buffer.source_register2.address;
            ROB[i].src2_RorP = stage_decode.input_buffer.src2_RorP;
            ROB[i].dest_index = stage_decode.input_buffer.dest_register.address;
            ROB[i].dest_RorP = stage_decode.input_buffer.dest_RorP;
            ROB[i].clock_cycle = stats1.cycle;
            strcpy(ROB[i].inst_opcode, stage_decode.input_buffer.mnemonic);
            ROB[i].status_of_allocation_ROB = FREE;
            break;
        }
    }
}

void LSQ_stage(void){
    for (int i = 0; i < 32; ++i) {
        if(issue_queue_struct[i].status_of_allocation == ALLOCATED){
            data_memory1.data_array[issue_queue_struct[i].dest_index]=stage_mem.input_buffer.result;
            issue_queue_struct[i].clock_cycle=1000;
            issue_queue_struct[i].dest_index=0;
            issue_queue_struct[i].dest_RorP=-1;
            memset(issue_queue_struct[i].inst_opcode,0, sizeof(issue_queue_struct[i].inst_opcode));
            issue_queue_struct[i].LSQ_index=-1;
            issue_queue_struct[i].ROB_index=-1;
            memset(issue_queue_struct[i].original_instruction_string,0, sizeof(issue_queue_struct[i].original_instruction_string));
            issue_queue_struct[i].src1_index=-1;
            issue_queue_struct[i].src1_RorP=-1;
            issue_queue_struct[i].src1_status=-1;
            issue_queue_struct[i].src1_value=-1;
            issue_queue_struct[i].src2_index=-1;
            issue_queue_struct[i].src2_RorP=-1;
            issue_queue_struct[i].src2_status=-1;
            issue_queue_struct[i].src2_value=-1;
            issue_queue_struct[i].status_of_allocation=FREE;
            memset(issue_queue_struct[i].translated_instruction_string,0, sizeof(issue_queue_struct[i].translated_instruction_string));
            issue_queue_struct[i].wakeup=NO;
            break;
        }
    }
}