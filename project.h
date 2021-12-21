#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/*A various of macros that will be used in all the other four files*/
#define MAX 1000
#define MAX_ARRAY 100
#define IC_INITIAL 100
#define DC_INITIAL 0
#define MAX_CONST 80
#define FOREVER 1
#define TWO 2
#define ONE 1
#define MAX_SYMBOL_LENGTH 31
#define ZERO_ADDRESS 0
#define ABSOLUTE 4
#define RELOCATABLE 2
#define EXTERNAL 1
#define ZERO 0
#define MAX_ROW_LENGTH 80
#define EXTENSION 4
#define LARGE_A 65
#define LARGE_Z 90
#define SMALL_A 97
#define SMALL_Z 122
#define ZERO_NUM 48
#define NINE_NUM 57
#define MAX_LENGTH 5
#define INITIALIZE_BUFFERS \
    strcpy(originalBuf,buf);


/*this union structure holds the various types of words that describe in the maman booklet
 * regular - describe the first word that we encoding
 * all  - describe the word in case of guidance sentence
 * memory - describe the word in case of types of instruction sentence
 * reg    - describe the word in the one of the operand (or both of them) are registers
 * all of those defines sums up to the definition of word type*/
typedef union st
{
    int num;
    struct  {
        unsigned int ARE:3;
        unsigned int dest_immediate_addressing:1;
        unsigned int dest_direct_addressing:1;
        unsigned int dest_undirect_register_addressing:1;
        unsigned int dest_direct_register_addressing:1;
        unsigned int source_immediate_addressing:1;
        unsigned int source_direct_addressing:1;
        unsigned int source_undirect_register_addressing:1;
        unsigned int source_direct_register_addressing:1;
        unsigned int opcode:4;
    }regular;
    struct
    {
        unsigned int ARE:3;
        unsigned int num:12;
    }memory;
    struct {
        unsigned int full:15;
    }all;
    struct {
        unsigned int ARE:3;
        unsigned int dest_reg_num:3;
        unsigned int source_reg_num:3;
    }reg;
}word;
char buf[MAX];
char originalBuf[MAX],*token;
int dataImage[MAX];
word codeImage[MAX];
enum type1{NO = 0,YES,UNKNOWN};
enum type{OFF = 0,ON};
enum type2{IA =0,DA,IDRA,IRA,DEFAULT};/*{Immediate Addressing,direct addressing,indirect register addressing ,immediate register addressing}*/
enum type5{EXTE,ENT};

/*representation of single node in the LABEL table,the representation of the LABEL table will be via linked-list*/
typedef struct node *ptr;
typedef struct node{
    char symbolName[MAX];
    int address;/*starts from 100*/
    int external;/*YES=1 NO = 0*/
    int guidance_or_instruction;/*YES=1 NO = 0*/
    int entry; /*YES=1 NO = 0*/
    ptr next;
}symbol_tabel;

/*representation of typedef extry that holds every extern OR entry label in a linked list that defined partially in
 * the other four files*/
typedef struct extry *pointer;
typedef struct extry
{
    char labelExt[MAX_ARRAY];
    char labelEnt[MAX_ARRAY];
    int address;
    pointer nxt;
    enum type5 type;
}extry;

/*the struct Operand defined as array and hols two member,the first one is the opcode name
 * and the second is the opcode operation number*/
struct Operand
{
    char *nameOfOperand;
    int opcode;

}static operands[] ={{"mov",0},{"cmp",1},{"add",2},{"sub",3,},{"lea",4},
                     {"clr",5},{"not",6},{"inc",7},{"dec",8},{"jmp",9},{"bne",10},{"red",11},{"prn",12},{"jsr",13},
                     {"rts",14},{"stop",15} } ;

/*PLEASE NOTICE : all the function documentation is in the function.c file*/
/*-----void functions------*/
void first_read_run(int);
void second_read_run(int);
void add_symbol_to_symbol_table(char*,int,int,int,int);
void add_string_to_data_image(char *,int);
void update_data_symbol_address(ptr);
void update_entry(char *);
void create_obj_file(int [],word []);
void create_ext_file();
void create_ent_file();
void update_ext_array(char *,int,int,enum type5);
void file_concatenation_obj(char *,char [],char *,int);
void file_concatenation_ext(char *,char [],char *,int);
void file_concatenation_ent(char *,char [],char *,int);
void initialize_array_to_zero(char []);
/*-----int functions------*/
int length_of_buf(char []);
int check_symbol(char *,int,int);
int check_if_line_is_empty();
int check_if_comment_line(char *);
int check_if_symbol_already_exist(char *,int);
int check_opcode(char *);
int check_if_word_is_register(char *);
int check_if_extern_guidance_sentence_ver2(char *,int);
int check_if_label_is_extern(char *,int);
int check_if_entry_guidance_sentence(char *,int);
int check_immediate_value(char *,int);
int check_if_it_is_guidance_sentence(void);
int identify_string(char *);
int identify_data(char *);
int check_warnings(int);
int check_valid_opcodes(char *,int);
int check_valid_opcodes_to_operands(int);
int check_illegal_commas(char []);
int check_syntax(int);
int extract_register_number_from_register_first_operand(char *,int);
int extract_register_number_from_register_second_operand(char *,int);
int cut_hash_from_number(char *);
int operand_counter(void);
int get_opcode_num(void);
int get_symbol_address(char *);
int check_if_two_operands_are_register();
int check_missing_commas(char *,int lineCnt);
/*-----char* functions------*/
char *get_first_word(char []);
char *get_next_word(int);
char *get_opcode_name(void);
char *cut_last_character_from_symbol(char*);
char *get_first_operand(void);
char *get_second_operand(void);
/*-----word functions------*/
word initialize_first_word_to_imageCode_array(enum type2 ,int,int );
word initialize_second_word_to_imageCode_array(int);
word initialize_third_word_to_imageCode_array(int);
/*-----enum type2 functions------*/
enum type2 check_type_of_first_operand(int);
enum type2 check_type_of_second_operand(int);



