#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MEM_SIZE 4096 
#define LINE_LEN 500
#define LABEL_LEN 50
#define LEN_REG_STR 6
#define LEN_OPCODE_STR 6
#define PRAM_NUM 5

//  structs 
typedef struct instruction {
	
	int data_word;
	int valid_word;

	int inst_vs_imm;  //
	char opcode[LEN_OPCODE_STR];
	char rd[LEN_REG_STR];
	char rs[LEN_REG_STR];
	char rt[LEN_REG_STR];
	char imm[LABEL_LEN];
	int PC;
}instruction;


typedef struct Label
{
	char name[50];
	int ADDREESS;
}label;

// Global variables 
FILE* asm_code = NULL;
FILE* memin_file = NULL;
label* our_labels = NULL;
unsigned long** array_hex_inst = NULL;


// Function 

int ASMCODETOIN(instruction inst_arr[], int* max_address, int* PC, int* num_of_labels);
/*it scans the ass code and update the instruc array get the max address of the data and get the max pc so we can know whos bigger
return 1 in succes o in fail*/


int AddWord(const char line[], int* max_address);
/*as it name this func takes a line and ifs a word line it upadte 
the word array and in the end it update the max address succes return 1 0 otherwise*/



int AddInst(const char line[], instruction inst_arr[], int* PC);
/*we get a line and update the array of instruc and then we update the vaule of pc if the function fails it returns 0 
other wise 1*/




int LABELED_LINES(char line[], int* num_of_labels, int PC);
/*the function takes a labeled line check if its in our labels array if yes it returns 0 otherwise 
we update our labels and the new label to it retrun -1 if it fails otherwise 1*/



int EmptyLine(const char line[]);
/*check if the line is empty if yes it return 1 otherwise 0 */



void RemoveWhiteSpaces(char str[]);
/*it removes the white spaces*/



unsigned long StrNumToHex(char num[]);
/*we take a str that represnt a nubmer and return the the number in hex */



int InitHexArrays(int max_PC, int max_address);
/*allocating memory array_hex_inst and the array_hex_word if the allocating success it returns 1 */



int RegisterNumber(const char reg[]);
/*it takes a str the represnt the name of a reg and return its nummber ,returns -1 if the reg name is not found*/



int OpcodeNumber(const char Op[]);
/*it takes a str the represnt the name of a opcode and return its nummber ,returns -1 if the opcode name is not found*/



int ADDREESSOfLabel(char label[], int num_of_labels);
/*it takes a a label and search for it in our labels array and retrun the address if we didnt found it it returns -1 */



unsigned long StrInstToHex(instruction inst, int num_of_labels);
/*give the hex vaule of an instruc in fail return -1 */



int InstructonsToHexArr(instruction inst_arr[], int max_PC, int num_of_labels);
/* puts instructions on hex array returns 1 in success
, 0 in fail */



void PrintToFiles(int max_PC, int max_address);
/* The function print all the hex instructions to the memin output file*/



void FreeAll(int max_PC, int max_address);
/* close files and free allocated memory */

void InitWordArr(instruction* arr);

// main
int main(int argc, char* argv[])
{
	
	instruction inst_arr[MEM_SIZE];
	

	
	int max_address = 0;
	int max_PC = 0;
	int num_of_labels = 0;

	// bulding the labels array
	our_labels = malloc(sizeof(label));
	if (NULL == our_labels) {
		printf("Error in alADDREESS\n");
		FreeAll(max_PC, max_address);
		exit(-1);
	}

	// Initialization the word_arr to unvaild
	InitWordArr(inst_arr);

	// Open files
	asm_code = fopen(argv[1], "r");
	if (NULL == asm_code) {
		printf("Error open files!\n");
		FreeAll(max_PC, max_address);
		exit(-1);
	}
	memin_file = fopen(argv[2], "w");
	if (NULL == memin_file) {
		printf("Error open files!\n");
		FreeAll(max_PC, max_address);
		exit(-1);
	}
	

	// Putting the data of the asm_code into two arrays of structs
	if (!ASMCODETOIN(inst_arr, &max_address, &max_PC, &num_of_labels)) {
		FreeAll(max_PC, max_address);
		exit(-1);
	}

	// Initialize hex array of instructions and hex array of data
	//unsigned long long array_hex_inst[max_PC];
	//unsigned long array_hex_word[max_address];
	int len = max_PC;
	if (len < max_address) len = max_address + 1;

	array_hex_inst = malloc(len * sizeof(unsigned long*));
	if (NULL == array_hex_inst) {
		printf("invalid pointer!\n");
		FreeAll(max_PC, max_address);
		exit(-1);
	}


	if (!InitHexArrays(max_PC, max_address)) {
		FreeAll(max_PC, max_address);
		exit(-1);
	}

	// Put the instructions and data in hex array
	if (!InstructonsToHexArr(inst_arr, max_PC, num_of_labels, max_address)) {
		FreeAll(max_PC, max_address);
		exit(-1);
	}

	// Write the data and the instructions in hex to the output files
	PrintToFiles(max_PC, max_address);

	// Close all the open files and free all the allocated memory at the end of the code
	FreeAll(max_PC, max_address);
	return 0;
}


int ASMCODETOIN(instruction inst_arr[], int* max_address, int* PC, int* num_of_labels)
{
	char line[LINE_LEN];
	int is_labeled = 0;

	// we get every line in the file 
	while (!feof(asm_code) && fgets(line, LINE_LEN, asm_code)) {
		if (strstr(line, "#") != NULL)               // we have a note
			strcpy(line, strtok(line, "#"));         // Remove the note

		// we check if the line is labeld if yes update our labels array 
		is_labeled = LABELED_LINES(line, num_of_labels, *PC);
		if (is_labeled == -1) {
			printf("Error in LABELED_LINES function");
			return 0;
		}

		
		if (EmptyLine(line)) {// check if the line is empty 
			continue;
		}

		if (strstr(line, ".word") != NULL) {         // check if its a word 
			if (!AddWord(line, inst_arr, max_address)) {
				printf("Error in AddWord function");
				return 0;
			}
		}
		else {                                         // its an instruction
			if (!AddInst(line, inst_arr, PC)) {
				printf("Error in AddInst function");
				return 0;
			}
		}
	}
	return 1;
}

int AddWord(const char line[], instruction inst_arr[], int* max_address)
{
	char* token;
	unsigned long data, address;
	strtok(line, " \t\r\n\f");             // delete ".word" 
	token = strtok(NULL, " \t\r\n\f");     // take the address
	if (token == NULL) {
		printf("Error, Missing address\n");
		return 0;
	}
	address = StrNumToHex(token);
	token = strtok(NULL, " \t\r\n\f");     // take the data
	if (token == NULL) {
		printf("Error, Missing data\n");
		return 0;
	}
	data = StrNumToHex(token);
	if (address < MEM_SIZE && address >= 0)
	{
		if (*max_address < address)
			*max_address = address;
		
		inst_arr[address].valid_word = 1;
		inst_arr[address].data_word = data;
	}
	else
	{
		printf("Error ,invalid address\n");
		return 0;
	}
	return 1;
}

int AddInst(const char line[], instruction inst_arr[], int* PC)
{
	int there_is_imm = 0;
	char* args[PRAM_NUM];
	int i = 0;
	args[i] = strtok(line, " \t\r\n\f,");          
	if (args[i] == NULL) {
		printf("Error in inst %d, Missing %d param\n", PC, i);
		return 0;
	}
	for (i = 1; i < PRAM_NUM; i++) {      
		args[i] = strtok(NULL, " \t\r\n\f,");
		if (strcmp(args[i], "$imm") == 0)there_is_imm = 1;
		if (args[i] == NULL) {
			printf("Error in inst %d, Missing %d param\n", PC, i);
			return 0;
		}
	}

	// update the inst array with the right parm
	inst_arr[*PC].inst_vs_imm = 0;
	strcpy(inst_arr[*PC].opcode, args[0]);
	strcpy(inst_arr[*PC].rd, args[1]);
	strcpy(inst_arr[*PC].rs, args[2]);
	strcpy(inst_arr[*PC].rt, args[3]);
	inst_arr[*PC].PC = *PC;
	
	if (there_is_imm) {
		*PC += 1;
		inst_arr[*PC].inst_vs_imm = 1;
		strcpy(inst_arr[*PC].imm, args[4]);
		inst_arr[*PC].PC = *PC;
	}


	*PC += 1;           
	return 1;
}

int LABELED_LINES(char line[], int* num_of_labels, int PC)
{
	char label_name[LABEL_LEN];
	if (strstr(line, ":") != NULL && ((strstr(line, ":") < strstr(line, "#")) || (strstr(line, "#") == NULL))) { // Label
		//delete ":" and the spaces to get the label
		strcpy(label_name, strtok(line, ":"));
		strcpy(line, (strtok(NULL, "")));
		RemoveWhiteSpaces(label_name);

		
		our_labels = realloc(our_labels, (*num_of_labels + 1) * sizeof(label));
		if (NULL == our_labels) {
			printf("Error in alADDREESS\n");
			return -1;
		}
		//update the right vaules in our array
		strcpy(our_labels[*num_of_labels].name, label_name);
		our_labels[*num_of_labels].ADDREESS = PC;
		*num_of_labels += 1;
		return 1;
	}
	return 0;       // its not label
}

int EmptyLine(const char line[])
{
	char cpy_line[LINE_LEN];
	strcpy(cpy_line, line);
	RemoveWhiteSpaces(cpy_line);
	if (strcmp(cpy_line, "\n") == 0) return 1;
	return 0;
}

void RemoveWhiteSpaces(char str[])
{
	int i = 0, j = 0;
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t')
			str[j++] = str[i];
		i++;
	}
	str[j] = '\0';
}

unsigned long StrNumToHex(char num[])
{
	unsigned long hex_num;
	if (strstr(num, "0x") != NULL || strstr(num, "0X") != NULL)     // hex num
		hex_num = strtol(num, NULL, 16);
	else hex_num = strtol(num, NULL, 10);                           // dec nume
	return hex_num;
}


void InitWordArr(instruction* arr)
{
	
	for (int i = 0; i < MEM_SIZE; i++) {
		
		arr[i].valid_word = 0;
	}
		
}

int InitHexArrays(int max_PC, int max_address)
{
	int len = max_PC;
	if (len < max_address) len = max_address + 1;
	
	
	for (int i = 0; i < len; i++)
	{
		array_hex_inst[i] = malloc(sizeof(unsigned long));
		if (NULL == array_hex_inst[i]) {
			printf("invalid pointer!\n");
			return 0;
		}
	}

	return 1;
}

int RegisterNumber(const char reg_name[])
{
	if (reg_name == NULL) {
		return -1;
	}	if (strcmp(reg_name, "$zero") == 0)
		return 0;
	else if (strcmp(reg_name, "$imm") == 0)
		return 1;
	else if (strcmp(reg_name, "$v0") == 0)
		return 2;
	else if (strcmp(reg_name, "$a0") == 0)
		return 3;
	else if (strcmp(reg_name, "$a1") == 0)
		return 4;
	else if (strcmp(reg_name, "$a2") == 0)
		return 5;
	else if (strcmp(reg_name, "$a3") == 0)
		return 6;
	else if (strcmp(reg_name, "$t0") == 0)
		return 7;
	else if (strcmp(reg_name, "$t1") == 0)
		return 8;
	else if (strcmp(reg_name, "$t2") == 0)
		return 9;
	else if (strcmp(reg_name, "$s0") == 0)
		return 10;
	else if (strcmp(reg_name, "$s1") == 0)
		return 11;
	else if (strcmp(reg_name, "$s2") == 0)
		return 12;
	else if (strcmp(reg_name, "$gp") == 0)
		return 13;
	else if (strcmp(reg_name, "$sp") == 0)
		return 14;
	else if (strcmp(reg_name, "$ra") == 0)
		return 15;
	
	else {
		printf("Error, Invailed register: %s\n ", reg_name);
		return -1;
	}
}

int OpcodeNumber(const char opcode_name[])
{
	if (opcode_name == NULL) {
		return -1;
	}
	if (strcmp(opcode_name, "add") == 0)
		return 0;
	else if (strcmp(opcode_name, "sub") == 0)
		return 1;
	else if (strcmp(opcode_name, "mul") == 0)
		return 2;
	else if (strcmp(opcode_name, "and") == 0)
		return 3;
	else if (strcmp(opcode_name, "or") == 0)
		return 4;
	else if (strcmp(opcode_name, "xor") == 0)
		return 5;
	else if (strcmp(opcode_name, "sll") == 0)
		return 6;
	else if (strcmp(opcode_name, "sra") == 0)
		return 7;
	else if (strcmp(opcode_name, "srl") == 0)
		return 8;
	else if (strcmp(opcode_name, "beq") == 0)
		return 9;
	else if (strcmp(opcode_name, "bne") == 0)
		return 10;
	else if (strcmp(opcode_name, "blt") == 0)
		return 11;
	else if (strcmp(opcode_name, "bgt") == 0)
		return 12;
	else if (strcmp(opcode_name, "ble") == 0)
		return 13;
	else if (strcmp(opcode_name, "bge") == 0)
		return 14;
	else if (strcmp(opcode_name, "jal") == 0)
		return 15;
	else if (strcmp(opcode_name, "lw") == 0)
		return 16;
	else if (strcmp(opcode_name, "sw") == 0)
		return 17;
	else if (strcmp(opcode_name, "reti") == 0)
		return 18;
	else if (strcmp(opcode_name, "in") == 0)
		return 19;
	else if (strcmp(opcode_name, "out") == 0)
		return 20;
	else if (strcmp(opcode_name, "halt") == 0)
		return 21;
	
	else {
		printf("Error, Invailed opcode %s\n", opcode_name);
		return -1;
	}
}

int ADDREESSOfLabel(char label[], int num_of_labels)
{
	int i;
	for (i = 0; i < num_of_labels; i++) {
		if (strcmp(label, our_labels[i].name) == 0) break;   // found a label
	}
	if (i == num_of_labels) return -1;             //the label is not in our labels
	return our_labels[i].ADDREESS;
}

unsigned long StrInstToHex(instruction inst, int num_of_labels,int PC)
{
	unsigned  long opcode, rd, rs, rt, imm;

	if (inst.inst_vs_imm == 0) {
		opcode = OpcodeNumber(inst.opcode);
		rd = RegisterNumber(inst.rd);
		rs = RegisterNumber(inst.rs);
		rt = RegisterNumber(inst.rt);

		// Invaild name of opcode or regester
		if (opcode == -1 || rd == -1 || rs == -1 || rt == -1) {
			//error
			printf("Error ,invalid Invaild name of opcode or regester\n");
			return -1;
		}

		rt = (rt & 0xF);
		rs = (rs & 0xF) << 4;
		rd = (rd & 0xF) << 8;
		opcode = (opcode & 0xFF) << 12;
		return opcode + rd + rs + rt;// ordering the values in one line
	}

	if (inst.inst_vs_imm == 1) {

		
		if ((inst.imm[0] >= 'a' && inst.imm[0] <= 'z') || (inst.imm[0] >= 'A' && inst.imm[0] <= 'Z'))  
		{
			imm = ADDREESSOfLabel(inst.imm, num_of_labels);
			if (imm == -1 || imm >= MEM_SIZE) {
				//error 
				printf("Error ,invalid label in imm2\n");
				return -1;
			}
		}
		else {
			imm = StrNumToHex(inst.imm);
		}

		// The result
		imm = imm & 0xFFFFF;
		return imm;
	}

}

int InstructonsToHexArr(instruction inst_arr[], int max_PC, int num_of_labels, int max_address)
{
	unsigned long hex_inst = 0;
	for (int i = 0; i < max_PC; i++) {
		//  str instruction to hex

		hex_inst = StrInstToHex(inst_arr[i], num_of_labels, i);
		if (hex_inst == -1) return 0;       

		
		*array_hex_inst[i] = hex_inst;
	}

	unsigned long hex_word = 0;
	for (int i = max_PC; i <= max_address; i++) {
		if (inst_arr[i].valid_word) {           // found data
			hex_word = inst_arr[i].data_word;
		}
		else {                             //  no data
			hex_word = 0;
		}
		
		*array_hex_inst[i] = hex_word;
	}

	return 1;
}


void PrintToFiles(int max_PC, int max_address)
{
	for (int i = 0; i < max_PC; i++) {
		fprintf(memin_file, "%05lX\n", *array_hex_inst[i]);
	}
	if (max_address == 0) return;
	for (int i = max_PC; i <= max_address; i++) {
		fprintf(memin_file, "%05lX\n", *array_hex_inst[i]);
	}
}

void FreeAll(int max_PC, int max_address)
{
	if (asm_code != NULL) {
		fclose(asm_code);
	}
	if (memin_file != NULL) {
		fclose(memin_file);
	}


	if (our_labels != NULL) free(our_labels);
	if (array_hex_inst != NULL) {
		for (int i = 0; i < max_PC; i++)
			if (array_hex_inst[i] != NULL) free(array_hex_inst[i]);
		free(array_hex_inst);
	}
}