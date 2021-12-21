#include "project.h"
extern pointer EXN;
extern ptr HSL;
extern int IC,DC,warningFlag,symbolWarningFlag;
extern FILE *fd1,*fd3,*fd4,*fd5;
/*function.c file :This file contains all the function that I have been used in the project
every function have their own documentation*/



/*---------------function implementation---------------*/

/*check_symbol : with a given string this function checks if it a valid symbol, return 0 if it is,else returns 1.
 * param 1)flag - this flag determines if the candidate symbol is a new defined symbol that terminates with ':'
 * or it is a direct addressing symbol which located in one of the operands
 *       2)word - a candidate symbol
 *       3)lineCnt- a line counter*/
int check_symbol(char *word,int flag,int lineCnt)
{
    int i ,length = strlen(word);
    if(length>MAX_SYMBOL_LENGTH)
    {
        symbolWarningFlag = ON;
        warningFlag = ON;
        fprintf(stderr,"in file firstrun.c:line %d:the symbol %s have more than 31 characters,please revise.\n",lineCnt,word);
        return 1;
    }

    /*checks if the the given word is with the same name of any registers */
    if(!strcmp(word , "r0:") || !strcmp(word , "r1:") || !strcmp(word , "r2:") || !strcmp(word , "r3:") || !strcmp(word , "r4:") || !strcmp(word , "r5:")
       || !strcmp(word , "r6:") || !strcmp(word , "r7:") ||  !strcmp(word , "data:") ||  !strcmp(word , "string:"))
    {
        symbolWarningFlag = ON;
        warningFlag = ON;
        fprintf(stderr,"in file firstrun.c:line %d:illegal definition of label,please revise.\n",lineCnt);
        return 1;
    }
    if(!strcmp(word , "r0") || !strcmp(word , "r1") || !strcmp(word , "r2") || !strcmp(word , "r3") || !strcmp(word , "r4") || !strcmp(word , "r5")
       || !strcmp(word , "r6") || !strcmp(word , "r7") ||  !strcmp(word , ".data") ||  !strcmp(word , ".string"))
        return 1;


    if(!strcmp(word,"mov") || !strcmp(word,"cmp") || !strcmp(word,"add") || !strcmp(word,"sub") ||
       !strcmp(word,"lea") || !strcmp(word,"clr") || !strcmp(word,"not") || !strcmp(word,"inc") ||
       !strcmp(word,"dec") || !strcmp(word,"jmp") || !strcmp(word,"bne") || !strcmp(word,"red") ||
       !strcmp(word,"prn") || !strcmp(word,"jsr") ||  !strcmp(word,"rts")|| !strcmp(word,"stop"))
        return 1;


    else
    {           /* check if starts with english word and ends with ':' */
        if( (!(word[ZERO] >= LARGE_A && word[ZERO] <= LARGE_Z) && !(word[ZERO] >= SMALL_A && word[ZERO] <= SMALL_Z)))
            return 1;
        if(word[length-1] != ':'&& flag == OFF)
        {
            warningFlag = ON;
            symbolWarningFlag = ON;
            fprintf(stderr,"in file firstrun.c:line %d:Error:\tnot a valid symbol,missing ':' \n",lineCnt);
            return 1;
        }

        for(i=1 ;i<length-1;i++)
        {
            if( !(word[i] >= LARGE_A &&word[i]<= LARGE_Z) &&!(word[i]>=SMALL_A&&word[i]<=SMALL_Z)&&!(word[i] >= ZERO_NUM && word[i] <= NINE_NUM)&&word[length-1]==':')
            {
                warningFlag = ON;
                symbolWarningFlag = ON;
                fprintf(stderr,"in file firstrun.c:line %d:Error: Illegal label name,please revise. \n",lineCnt);
                return 1;
            }

        }

        for(i=1 ;i<length-1;i++)
        {
            if( !(word[i] >= LARGE_A &&word[i]<= LARGE_Z) &&!(word[i]>=SMALL_A&&word[i]<=SMALL_Z)&&!(word[i] >= ZERO_NUM && word[i] <= NINE_NUM))
                return 1;
        }

    }
    return 0 ;
}

/*check_opcode : with a given string this function checks if it a valid opcode, return 0 if it is,else returns 1.
param 1) word - A string token*/
int check_opcode(char *word)
{
    if(!strcmp(word,"mov") || !strcmp(word,"cmp") || !strcmp(word,"add") || !strcmp(word,"sub") ||
       !strcmp(word,"lea") || !strcmp(word,"clr") || !strcmp(word,"not") || !strcmp(word,"inc") ||
       !strcmp(word,"dec") || !strcmp(word,"jmp") || !strcmp(word,"bne") || !strcmp(word,"red") ||
       !strcmp(word,"prn") || !strcmp(word,"jsr") ||!strcmp(word,"rts") || !strcmp(word,"stop"))
        return 0;
    else return 1;
}

/*add_symbol_to_symbol_table ->void function : This function adds a given symbol to the designated linked list that holds the symbol table
 * param 1)str - the symbol candidate
 *       2)IC - Instruction counter.
 *       3)ext - a variable that determines if the symbol candidate is external.
 *       4)ent - a variable that determines if the symbol candidate is entry.
 *       5)goi - a variable that determines if the symbol belongs to guidance sentence.
*/
void add_symbol_to_symbol_table(char *str,int IC,int ext,int goi,int ent)
{
    char piece[MAX_SYMBOL_LENGTH];
    int length;
    ptr newNode,curr;
    strcpy(piece,str);
    length = strlen(piece);
    newNode=(ptr)malloc(sizeof(symbol_tabel));
    memcpy(newNode->symbolName,piece,length);
    newNode->address = IC;
    newNode->next = NULL;
    newNode->external = ext;
    newNode->guidance_or_instruction = goi;
    newNode->entry = ent;
    if(HSL==NULL)
    {
        HSL = newNode;
        return;
    }
    curr =  HSL;
    while(curr->next!=NULL)
        curr = curr->next;

    curr->next = newNode;
}
/*check_immediate_value :This function checks with a given string if the a specific operand is within the immediate value
 * addressing,return 0 if it is,else return 1
 * param str - a string token*/
int check_immediate_value(char *str,int lineCnt)
{
    int i,length;
    char buf[MAX];
    length = strlen(str);
    memcpy(buf,str,length);
    if(buf[0]=='#')
    {
        for(i = 1;i<length;i++)
        {
            if((isspace(buf[i])) || (!isdigit(buf[i])&&buf[i]!='-'))
            {
                warningFlag = ON;
                fprintf(stdout,"line %d:not a legal value , please after the char '#' enter a decimal number \n",lineCnt);
                return 1;
            }
        }
        return 0;
    }
    return 1;
}
/*check_if_symbol_already_exist : this function determine if a specific candidate symbol is already
 * in the symbol_table that represents via linked list depicted in project.h file
 * returns 1 if it is,else returns 0
 * param 1)str - a string token that represents a candidate symbol
 *       2)lineCnt - a line counter*/
int check_if_symbol_already_exist(char *str,int lineCnt)
{
    char arr[MAX_ARRAY];
    int length;
    ptr p;
    p = HSL;
    length = strlen(str);
    strcpy(arr,str);
    arr[length-1]=arr[length];
    while(p)
    {

        if(!memcmp(p->symbolName,arr,length-1))
        {
            symbolWarningFlag = ON;
            warningFlag = ON;
            fprintf(stderr,"in file firstrun.c:line %d: %s symbol already exist!,please use different name!\n",lineCnt,arr);
            return 1;
        }
        p = p->next;
    }
    return 0;
}
/*get_next_word : this function get the next string token from a string array that contains a specific
 * line from the text
 * params 1) c - a counter that counts the current location in the array*/
char *get_next_word(int c)
{
    int i = 0;
    char newBuf[MAX];
    char *piece;
    strcpy(newBuf,originalBuf);
    for(;i<=c;i++)
    {
        if(i == 0)
        {
            piece = strtok(newBuf," \\ \" ,\t\n");
            continue;
        }
        piece = strtok(NULL," \\ \" ,\t\n");
    }
    return piece;
}


/*cut_last_character_from_symbol:this function cuts the ':' character from the symbol
 * returns the modified string
 * params: str - a string token*/
char *cut_last_character_from_symbol(char *str)
{
    int length = strlen(str);
    str[length-1]=str[length];
    return str;
}
char *get_first_word(char str[])
{
    char *token;
    token = strtok(str," \\ \" ,\t\n");
    return token;
}
/*check_if_word_is_register : this function receiving a line from the text and determine if the line is empty */
/*returns 0 if it is,else returns 1*/
/*params 1) word- a string token*/
int check_if_line_is_empty()
{
    int i = 0;
    for(;i<MAX_ARRAY;i++)
    {
        if(buf[i] == ZERO || isspace(buf[i]))
            continue;
        else
            return 0;
    }
    return 1;
}
/*check_if_word_is_register : this function receiving a string token and decides if it is with type "register"  */
/*returns 0 if it is,else returns 1*/
/*params 1) word- a string token*/
int check_if_word_is_register(char *word)
{
    if(!word)
        return 1;
    if(!strcmp(word,"r0") || !strcmp(word,"r1") || !strcmp(word,"r2") || !strcmp(word,"r3") || !strcmp(word,"r4") || !strcmp(word,"r5")
       || !strcmp(word,"r6") || !strcmp(word,"r7"))
        return 0;
    if(!strcmp(word,"*r0") || !strcmp(word,"*r1") || !strcmp(word,"*r2") || !strcmp(word,"*r3") || !strcmp(word,"*r4") || !strcmp(word,"*r5")
       || !strcmp(word,"*r6") || !strcmp(word,"*r7"))
        return 0;

    return 1;
}

/*check_if_extern_guidance_sentence_ver2 : this function determines if a the current line in the file is with type "extern"'*/
/*returns 0 if it is,else returns 1*/
/*params:NONE */
int check_if_extern_guidance_sentence_ver1()
{
    char newBuf[MAX];
    char *str;
    strcpy(newBuf,originalBuf);
    str = strtok(newBuf," \\ \" ,\t\n");
    if(!strcmp(str,".extern"))
        return 0;
    return 1;
}
/*check_if_entry_guidance_sentence_ver2 : this function receiving a string token and decides if it is with type "entry"*/
/*returns 0 if it is,else returns 1*/
/*params: 1) str-a given string token
 *        2)entCnt-a given counter that counts the number of entry symbol usage in the file */
int check_if_entry_guidance_sentence(char *str,int entCnt)
{
    if(!strcmp(str,".entry"))
        return 0;
    return 1;
}
/*check_if_extern_guidance_sentence_ver2 : this function receiving a string token and decides if it is with type "extern"*/
/*returns 0 if it is,else returns 1*/
/*params: 1) str-a given string token
 *        2)extCnt-a given counter that counts the number of external symbol usage in the file */
int check_if_extern_guidance_sentence_ver2(char *str,int extCnt)
{
    if(!strcmp(str,".extern"))
        return 0;
    return 1;
}
/*check_if_it_is_guidance_sentence : this function check if the current line in the text is a guidance sentence */
/*returns 0 if it is,else returns 1*/
/*params:NONE*/
int check_if_it_is_guidance_sentence()
{
    int i = 0;
    char newBuf[MAX];
    char *piece = "start";
    strcpy(newBuf,originalBuf);
    while(piece)
    {
        if(i == 0)
            piece = strtok(newBuf,"  ,\t\n");
        if(!strcmp(piece,".data") || !strcmp(piece,".string"))
            return 0;
        piece = strtok(NULL,"  ,\t\n");
        i++;
    }
    return 1;
}

/*identify_string : this functions receiving a string token and decides if it is with type ".string"*/
/*returns 0 if it is,else returns 1*/
int identify_string(char *str)
{
    if(!strcmp(str,".string"))
        return 0;
    return 1;
}

/*identify_data : this functions receiving a string token and decides if it is with type ".data"*/
/*returns 0 if it is,else returns 1*/
int identify_data(char *str)
{
    if(!strcmp(str,".data"))
        return 0;
    return 1;
}
/*add_string_to_data_image : this functions receiving a string and adding it to a destinated dataImage array that defined in
 * project.h file,and updating DC currectly
 * param 1)str- a string
 *       2)wordLength - the length of the string str*/
void add_string_to_data_image(char *str,int wordLength)
{
    int l = DC;
    char newBuf[MAX_CONST];
    strcpy(newBuf,str);
    wordLength = l + wordLength;
    for(;l<wordLength;l++)
    {
        dataImage[l] = newBuf[l];
    }
    DC =l+1;
}
/*check_warnings : this function determines if a warning flag has been turned on
 * param 1) c - the warning flag*/
int check_warnings(int c)
{
    if(c==ON)
        return 0;
    return 1;
}
/*update_data_symbol_address : this function updating the data symbol address after the first transition terminates
 * param 1)NONE*/
void update_data_symbol_address(ptr p)
{
    p = HSL;
    while(p)
    {
        if(p->guidance_or_instruction == YES)
            p->address = p->address +IC;
        p = p->next;
    }
}
/*operand_counter : this function counts the number of operands in the instruction sentence
 * param 1)NONE*/
int operand_counter()
{
    int cnt = 0;
    char newBuf[MAX];
    char *piece;
    strcpy(newBuf,originalBuf);
    piece = strtok(newBuf," \\ \" ,\t\n");
    piece = strtok(NULL," \\ \" ,\t\n");
    while(piece)
    {
        if(!check_opcode(piece)||!check_if_entry_guidance_sentence(piece,ZERO)||!check_if_extern_guidance_sentence_ver2(piece,ZERO))
        {
            piece = strtok(NULL," \\ \" ,\t\n");
            continue;
        }
        cnt++;
        piece = strtok(NULL," \\ \" ,\t\n");
    }
    return cnt;
}

/*get_first_operand : This function returns the first operand of the instruction sentence.
 * param 1)NONE*/
char *get_first_operand()
{
    char newBuf[MAX];
    char *piece;
    strcpy(newBuf,originalBuf);
    piece = strtok(newBuf," \\ \" ,\t\n");
    piece = strtok(NULL," \\ \" ,\t\n");
    while(piece)
    {
        if(!check_opcode(piece))
        {
            piece = strtok(NULL," \\ \" ,\t\n");

        }
        return piece;
    }
    return piece;
}
/*get_second_operand : This function returns the second operand of the instruction sentence.
 * param 1)NONE*/
char *get_second_operand()
{
    char newBuf[MAX];
    char *piece;
    strcpy(newBuf,originalBuf);
    piece = strtok(newBuf," \\ \" ,\t\n");
    if(!check_opcode(piece))
    {
        piece = strtok(NULL," \\ \" ,\t\n");
        piece = strtok(NULL," \\ \" ,\t\n");

    }
    else{
        piece = strtok(NULL," \\ \" ,\t\n");
        piece = strtok(NULL," \\ \" ,\t\n");
        piece = strtok(NULL," \\ \" ,\t\n");
    }
    return piece;

}
/*check_if_two_operands_are_register : this functions determines if the two operands in the instruction sentence
 * are registers returns 0 if it is,else returns 1
 * param 1)NONE*/
int check_if_two_operands_are_register()
{
    char *piece1,*piece2;
    piece1 = get_first_operand();
    piece2 = get_second_operand();
    if((!check_if_word_is_register(piece1))&&(!check_if_word_is_register(piece2)))
        return 0;
    return 1;
}

/*initialize_first_word_to_imageCode_array : this function initialize the first word of the sentence to
 * the designated imageCode array that depicted in project.h file
 * param 1)lineCnt - a line counter
 *       2)operandCnt -the number of operands in the sentence
 *       3)firstOperandType - the first operand type*/
word initialize_first_word_to_imageCode_array(enum type2 firstOperandType,int operandCnt,int lineCnt)
{
    int num= 0;
    char *token,*token2;
    word w;
    w.all.full = 0; /*initialize w to zero*/
    token = get_first_operand();
    token2 = get_opcode_name();
    if(!operandCnt)/*this block taking care in the case if the opcode is from the group {rts,stop},if it is then initialize and returns accordingly*/
    {
        if(!strcmp(token2,"rts"))
        {
            w.regular.ARE = ABSOLUTE;
            w.regular.opcode = get_opcode_num();
            return w;
        }
        else if(!strcmp(token2,"stop"))
        {
            w.regular.ARE = ABSOLUTE;
            w.regular.opcode = get_opcode_num();
            return w;
        }

    }

    if(firstOperandType==IA) /*immediate addressing*/
    {
        if(operandCnt==TWO) /*case of two operands*/
        {
            w.regular.ARE = ABSOLUTE;
            w.regular.source_immediate_addressing = ONE;
            w.regular.opcode = get_opcode_num();
            if (check_type_of_second_operand(lineCnt)== DA)
                w.regular.dest_direct_addressing = ONE;
            else if (check_type_of_second_operand(lineCnt) == IDRA)
                w.regular.dest_undirect_register_addressing = ONE;
            else if (check_type_of_second_operand(lineCnt) == IRA)
                w.regular.dest_direct_register_addressing = ONE;
            else if (check_type_of_second_operand(lineCnt) == IA)
                w.regular.dest_immediate_addressing = ONE;
        } else  if(operandCnt==ONE)/*case of one operands*/
        {
            w.regular.ARE = ABSOLUTE;
            w.regular.dest_immediate_addressing = ONE;
            w.regular.opcode = get_opcode_num();
        }
    } else
    if(firstOperandType==DA)/*Direct addressing*/
    {
        if (operandCnt == TWO)/*case of two operands*/
        {
            if (!check_if_label_is_extern(token,lineCnt)) {
                num = get_opcode_num();
                w.regular.opcode = num;
                w.regular.ARE = ABSOLUTE;
                w.regular.source_direct_addressing = ONE;

            } else {
                num = get_opcode_num();
                w.regular.opcode = num;
                w.regular.source_direct_addressing = ONE;
                w.regular.ARE = ABSOLUTE;
            }
            if (check_type_of_second_operand(lineCnt)== DA)
                w.regular.dest_direct_addressing = ONE;
            else if (check_type_of_second_operand(lineCnt) == IDRA)
                w.regular.dest_undirect_register_addressing = ONE;
            else if (check_type_of_second_operand(lineCnt) == IRA)
                w.regular.dest_direct_register_addressing = ONE;
            else if (check_type_of_second_operand(lineCnt)== IA)
                w.regular.dest_immediate_addressing = ONE;
        }

        else if (operandCnt == ONE)/*case of one operand*/
        {
            if (!check_if_label_is_extern(token,lineCnt)) {
                num = get_opcode_num();
                w.regular.opcode = num;
                w.regular.ARE = ABSOLUTE;
                w.regular.dest_direct_addressing = ONE;

            } else
            {
                num = get_opcode_num();
                w.regular.opcode = num;
                w.regular.ARE = ABSOLUTE;
                w.regular.dest_direct_addressing = ONE;
            }

        }

    }


    else if(firstOperandType==IDRA)/*indirect register addressing*/
    {
        if(operandCnt==ONE) /*case of one operand*/
        {
            w.regular.opcode = get_opcode_num();
            w.regular.ARE = ABSOLUTE;
            w.regular.dest_undirect_register_addressing=1;
        }
        if(operandCnt==TWO)/*case of two operands*/
        {
            w.regular.opcode = get_opcode_num();
            w.regular.ARE = ABSOLUTE;
            w.regular.source_undirect_register_addressing = ONE;
            if (check_type_of_second_operand(lineCnt)== DA)
                w.regular.dest_direct_addressing = ONE;
            else if (check_type_of_second_operand(lineCnt) == IDRA)
                w.regular.dest_undirect_register_addressing = ONE;
            else if (check_type_of_second_operand(lineCnt) == IRA)
                w.regular.dest_direct_register_addressing = ONE;
            else if (check_type_of_second_operand(lineCnt) == IA)
                w.regular.dest_immediate_addressing = ONE;
        }
    } else if (firstOperandType==IRA)/*immediate register addressing*/
    {
        if(operandCnt==ONE)
        {
            w.regular.opcode = get_opcode_num();
            w.regular.ARE = ABSOLUTE;
            w.regular.dest_direct_register_addressing= ONE;
        }
        else if(operandCnt==TWO)
        {
            w.regular.opcode = get_opcode_num();
            w.regular.ARE = ABSOLUTE;
            w.regular.source_direct_register_addressing = ONE;
            if (check_type_of_second_operand(lineCnt)== DA)
                w.regular.dest_direct_addressing = ONE;
            else if (check_type_of_second_operand(lineCnt) == IDRA)
                w.regular.dest_undirect_register_addressing = ONE;
            else if (check_type_of_second_operand(lineCnt) == IRA)
                w.regular.dest_direct_register_addressing = ONE;
            else if (check_type_of_second_operand(lineCnt) == IA)
                w.regular.dest_immediate_addressing = ONE;
        }
    }
    return w;
}/*end of the first word*/

/*initialize_second_word_to_imageCode_array : this function initialize the second word of the sentence to
 * the designated imageCode array that depicted in project.h file
 * param 1)lineCnt - a line counter*/
word initialize_second_word_to_imageCode_array(int lineCnt)
{
    word w;
    char *piece1,*piece2;
    char newBuf[MAX];
    int num,length;
    w.num = 0;
    strcpy(newBuf,originalBuf);
    if(check_type_of_first_operand(lineCnt)==IA) /*if the first operand is in immediate addressing mode*/
    {
        piece1 = get_first_operand();
        w.memory.ARE = ABSOLUTE;
        num= cut_hash_from_number(piece1);
        w.memory.num =num;
    }
    else  if(check_type_of_first_operand(lineCnt)==DA)/*if the first operand is in direct addressing mode*/

    {
        char *piece2 = get_first_operand();
        if(!check_if_label_is_extern(piece2,lineCnt))
        {
            length = strlen(piece2);
            w.memory.ARE = EXTERNAL;
            w.memory.num = get_symbol_address(piece2);
            update_ext_array(piece2,length,IC,EXTE);
        }
        else
        {
            w.memory.ARE = RELOCATABLE;
            w.memory.num = get_symbol_address(piece2);
        }
    }
    else if(!check_if_two_operands_are_register())/*if the first and second operands are registers*/
    {
        int num1,num2;
        piece1 = get_first_operand();
        piece2 = get_second_operand();
        num1 = extract_register_number_from_register_first_operand(piece1,lineCnt);
        num2 = extract_register_number_from_register_second_operand(piece2,lineCnt);
        w.reg.source_reg_num = num1;
        w.reg.dest_reg_num = num2;
        w.reg.ARE = ABSOLUTE;
    }
    else if (check_type_of_first_operand(lineCnt)==IDRA ||check_type_of_first_operand(lineCnt)==IRA)
    {
        if(operand_counter()==ONE)/*case of one operand*/
        {
            int num1;
            piece1 = get_first_operand();
            num1 = extract_register_number_from_register_first_operand(piece1,lineCnt);
            w.reg.dest_reg_num = num1;
            w.reg.ARE = ABSOLUTE;
        }
        else if(operand_counter()==TWO)/*case of two operands*/
        {
            int num1;
            piece1 = get_first_operand();
            num1 = extract_register_number_from_register_first_operand(piece1,lineCnt);
            w.reg.source_reg_num = num1;
            w.reg.ARE = ABSOLUTE;
        }
    }
    return w;
}
/*initialize_third_word_to_imageCode_array : this function initialize the third word of the sentence to
 * the designated imageCode array that depicted in project.h file
 * param 1)lineCnt - a line counter*/
word initialize_third_word_to_imageCode_array(int lineCnt)
{

    char *piece1;
    char newBuf[MAX];
    int num,length;
    word w;
    w.num = 0;
    piece1 = get_second_operand();
    strcpy(newBuf,originalBuf);
    if(check_type_of_second_operand(lineCnt)==IA) /*if the first operand is in immediate addressing mode*/
    {
        w.memory.ARE = ABSOLUTE;
        num= cut_hash_from_number(piece1);
        w.memory.num =num;
    }
    else  if(check_type_of_second_operand(lineCnt)==DA)/*if the first operand is in direct addressing mode*/

    {
        if(!check_if_label_is_extern(piece1,lineCnt))
        {
            length = strlen(piece1);
            w.memory.ARE = EXTERNAL;
            w.memory.num =get_symbol_address(piece1);
            update_ext_array(piece1,length,IC,EXTE);
        }
        else
        {
            char *piece2 = get_second_operand();
            w.memory.ARE = RELOCATABLE;
            w.memory.num = get_symbol_address(piece2);
        }
    }

    else if (check_type_of_second_operand(lineCnt)==IDRA ||check_type_of_second_operand(lineCnt)==IRA)
    {
        int num1;
        piece1 = get_second_operand();
        num1 = extract_register_number_from_register_second_operand(piece1,lineCnt);
        w.reg.dest_reg_num = num1;
        w.reg.ARE = ABSOLUTE;
    }
    return w;
}
/*check_type_of_first_operand : This function uses the function get_first_operand to receive the first operand
 * and checks the type of the addressing method of the specific operand, and returns the type.
 * the types are depicted in project.h file in "enum type2"
 * param 1)lineCnt - A line counter*/
enum type2 check_type_of_first_operand(int lineCnt)
{
    int length;
    char *token1;
    char buf[MAX];
    token1 = get_first_operand();
    if(token1==NULL)
        return DEFAULT;
    length = strlen(token1);
    memcpy(buf,token1,length);
    if(token1[0]=='#')
        return IA;
    else if(token1[0]=='*')
        return IDRA;
    else if(!check_symbol(token1,ON,lineCnt))
        return DA;
    else if(!check_if_word_is_register(token1))
        return IRA;
    else{
        warningFlag = ON;
        fprintf(stderr,"in file secondrun.c  line %d : Unknown type of operand! ",lineCnt);
    }
    return DEFAULT;
}
/*check_type_of_second_operand : This function uses the function get_second_operand to receive the second operand
 * and checks the type of the addressing method of the specific operand, and returns the type.
 * the types are depicted in project.h file in "enum type2"
 * param 1)lineCnt - A line counter*/
enum type2 check_type_of_second_operand(int lineCnt)
{
    int length;
    char *token1;
    token1 = get_second_operand();
    if(!token1)
        return 0;
    length = strlen(token1);
    memcpy(buf,token1,length);
    if(token1[0]=='#')
        return IA;
    else if(token1[0]=='*')
        return IDRA;
    else if(!check_symbol(token1,ON,lineCnt))
        return DA;
    else if(!check_if_word_is_register(token1))
        return IRA;
    else{
        warningFlag = ON;
        fprintf(stderr,"line %d : Unknown type of operand! ",lineCnt);
    }
    return DEFAULT;
}
/*cut_hash_from_number : This function receives hash with number attached to him(from the right) and extract the number
 * param 1)str - the hash with the number attached to it*/
int cut_hash_from_number(char *str)
{
    int length,num;
    char buf[MAX];
    length =strlen(str);
    memcpy(buf,str,length);
    str = strtok(buf,"#");
    num = atoi(str);
    return num;
}
/*extract_register_number_from_register_second_operand: This function receives a register and extract his number
 * param 1)str - the register
 *       2)lineCnt - line counter*/
int extract_register_number_from_register_first_operand(char *str,int lineCnt)
{
    int length,num;
    char buf[MAX];
    length =strlen(str);
    memcpy(buf,str,length);
    if(check_type_of_first_operand(lineCnt)==IDRA)
        str = strtok(buf,"*r");
    else
        str = strtok(buf,"r");
    num = atoi(str);
    return num;
}
/*extract_register_number_from_register_second_operand: This function receives a register and extract his number
 * param 1)str - the register
 *       2)lineCnt - line counter*/
int extract_register_number_from_register_second_operand(char *str,int lineCnt)
{
    int length,num;
    char buf[MAX];
    length =strlen(str);
    memcpy(buf,str,length);
    if(check_type_of_second_operand(lineCnt)==IDRA)
        str = strtok(buf,"*r");
    else
        str = strtok(buf,"r");
    num = atoi(str);
    return num;
}
/*get_opcode_name : This function receives no paramaters and returns the opcode name
 * param 1) NONE*/
char *get_opcode_name()
{
    char newBuf[MAX];
    char *piece;
    strcpy(newBuf,originalBuf);
    piece = strtok(newBuf," \\ \" ,\t\n");
    while(piece)
    {
        if(!check_opcode(piece))
        {
            break;
        }
        piece = strtok(NULL," \\ \" ,\t\n");
    }
    return piece;
}
/*get_opcode_num : This function uses the function get_opcode_name and returns the opcode number via the operands array struct
 * that defined in project.h file
 * param 1)NONE*/
int get_opcode_num()
{
    int i = 0,num = 0;
    char *piece;
    piece = get_opcode_name();
    for(;operands[i].nameOfOperand;i++)
    {
        if(!strcmp(piece,operands[i].nameOfOperand))
        {
            num = operands[i].opcode;
            break;
        }
    }
    return num;
}
/*check_if_label_is_extern : This function search for a given symbol that in the symbol_table linked list which depicted in project.h file */
/*and decide of the current symbol is extern*/
int check_if_label_is_extern(char *str,int lineCnt)
{
    int length;
    ptr p;
    p = HSL;
    length = strlen(str);
    if(!check_symbol(str,ON,lineCnt))
    {
        while(p)
        {
            if(!memcmp(p->symbolName,str,length))
            {
                if(p->external==ON)
                    return 0;
            }
            p = p->next;
        }
    }
    return 1;
}
/*get_symbol_address : This function search for a given symbol that in the symbol_table linked list which depicted in project.h file*/
/*and returns the symbol address in the memory*/
int get_symbol_address(char *str)
{
    int address,length;
    ptr p;
    p = HSL;
    length = strlen(str);
    while (p)
    {
        if(!memcmp(p->symbolName,str,length))
        {
            address = p->address;
            break;
        }
        p = p->next;
    }
    return address;
}
/*update_entry*/
void update_entry(char *str) {
    int length;
    ptr p;
    p = HSL;
    length = strlen(str);
    while (p) {
        if (!memcmp(p->symbolName, str, length)) {
            p->entry = ONE;
        }
        p = p->next;
    }
}
/*check_valid_opdcodes : This function checks the validation of the given opcodes*/
/*the validation is according the the table in page 30 in the maman booklet
param 1)str - an operand
			2)line - line counter*/
int check_valid_opcodes(char *str,int lineCnt)
{
    int cnt;
    cnt = operand_counter();
    if(cnt == ZERO)/*case: number of operands is ZERO*/
    {
        if(!strcmp("rts",str) || !strcmp("stop",str))
            return 0;
        else
        {
            warningFlag = ON;
            fprintf(stderr,"in file secondrun.c : line %d : the opcode- %s is not valid it should be from the group {rts,stop} which suppose to be with non-operands please revise.\n",lineCnt,str);
            return 1;
        }

    }
    else if (cnt == ONE) /*case: number of operands is ONE*/
    {
        if (!strcmp("clr",str) || !strcmp("not",str)
            || !strcmp("inc",str) || !strcmp("dec",str) || !strcmp("jmp",str)
            || !strcmp("bne",str) || !strcmp("red",str) || !strcmp("prn",str) || !strcmp("jsr",str))
            return 0;
        else
        {
            warningFlag = ON;
            fprintf(stderr,"in file functions.c : line %d : the opcode - %s is not valid it should be from the group"
                           " {clr,not,inc,dec,jmp,bne,red,prn,jsr} which suppose to be with one-operand please revise.\n",lineCnt,str);
            return 1;
        }
    }
    else if(cnt == TWO)/*case: number of operands is TWO*/
    {
        if (!strcmp("mov",str) || !strcmp("cmp",str)
            || !strcmp("add",str) || !strcmp("sub",str) || !strcmp("lea",str))
            return 0;
        else
        {
            warningFlag = ON;
            fprintf(stderr,"\nin file secondrun.c : line %d : the opcode - %s is not valid it should be from the group {mov,cmp,add,sub,lea}"
                           " which suppose to be with two-operands please revise.\n",lineCnt,str);
            return 1;
        }
    }
    return 1;
}


/*create_obj_file:This function creates the requested object file when the program terminates successfully
param 1) data - the designated dataImage
			2) code - the designated codeImage */
void create_obj_file(int data[],word code[])
{
    char array[MAX];
    int i = 100,slot,length;
    int newIC = IC -100;
    initialize_array_to_zero(array);
    fprintf(fd5,"%d \t\t\t %d\n",newIC,DC);
    for(;i<IC;i++)
    {
        slot = code[i].num;
        sprintf(array,"%05o",slot);
        length = strlen(array);
        if(length>MAX_LENGTH) /*if the length more than 5 ,we will print charaters one by one*/
        {
            int j = length - 5;
            fprintf(fd5,"0%d  \t",i);
            for(;j<length;j++)
            {
                fprintf(fd5, "%c",array[j]);
            }
            fprintf(fd5,"\n");
        }
        else
        {
            fprintf(fd5,"0%d \t  %s\n",i,array);
        }

    }
    for(i = 0 ;i<DC;i++)
    {
        slot = data[i];
        sprintf(array,"%05o",slot);
        length = strlen(array);
        if(length>MAX_LENGTH)/*if the length more than 5 ,we will print charaters one by one*/
        {
            int j = length - 5;
            fprintf(fd5,"0%d  \t",IC++);
            for(;j<length;j++)
            {
                fprintf(fd5, "%c",array[j]);
            }
            fprintf(fd5,"\n");
        } else
        {
            fprintf(fd5,"0%d \t  %s\n",IC++,array);

        }
    }
}
/*length_of_buf : This function will be depicted in check_syntax function.
param 1)str - the array that contains the entire line in the specific place in the file*/
int length_of_buf(char str[])
{
    int cnt = 0,i = 0;
    while(str[i++])
    {
        cnt++;
    }
    if(cnt >MAX_ROW_LENGTH)/*if the counter is bigger than 80*/
        return 0;
    return 1;
}
/*check_illegal_commas: This function will be depicted in check_syntax function.*/
int check_illegal_commas(char str[])
{
    int i = 0;
    for(;i<MAX_ROW_LENGTH;i++)
    {
        if(str[i]==',')
            break;
    }
    for(;i==' ';i++);
    if(str[++i]==',')
        return 0;
    return 1;
}
/*check_missing_commas - This function determine if in the specific line in the text file there are missing commas
 * param 1)str - the array that contains the line from the text
 *       2)lineCnt - A line counter*/
int check_missing_commas(char str[],int lineCnt)
{
    int i = 0;
    if(operand_counter()==TWO && (check_if_it_is_guidance_sentence()) && (check_if_extern_guidance_sentence_ver1()))
    {
        for(;i<strlen(str);i++)
        {
            if(str[i]==',')
                return 1;
        }

    }
    return 1;
}
/*check_syntax : This function reciving the global variable string names "buf" and operates a several
 * functions to decide if there is any syntax errors\problems
 * param 1)lineCnt - a line counter*/
int check_syntax(int lineCnt)
{
    if(!length_of_buf(buf))
    {
        warningFlag = ON;
        fprintf(stderr,"in file firstrun.c:line %d:the length of the sentence in the file is more than 80 characthers,please revise.\n",lineCnt);
        return 0;

    }
    else if (!check_illegal_commas(buf))
    {
        warningFlag = ON;
        fprintf(stderr,"in file firstrun.c:line %d: Multiple consecutive commas,please revise.\n",lineCnt);
        return 0;
    }
    else if(!check_missing_commas(buf,lineCnt))
    {
        warningFlag = ON;
        fprintf(stderr,"in file firstrun.c:line %d:Missing comma,please revise.\n",lineCnt);
        return 0;
    }
    return 1;
}
/* check_valid_opcodes_to_operands : This function analyzing the given operands according to the table depicted */
/* in page 30 in the MAMAN booklet*/
int check_valid_opcodes_to_operands(int lineCnt)
{

    char *piece;
    int opCnt = operand_counter() ;
    enum type2 firstOperandType = check_type_of_first_operand(lineCnt);
    piece = get_opcode_name();
    if(!strcmp("stop",piece) || !strcmp("rts",piece))
    {
        IC++;
        return 0;
    }


    if(opCnt==TWO)/*case of two operands*/
    {
        if(!strcmp("add",piece)||!strcmp("clr",piece) || !strcmp("not",piece) || !strcmp("inc",piece) ||
           !strcmp("dec",piece) || !strcmp("jmp",piece) || !strcmp("dec",piece) || !strcmp("jmp",piece) ||
           !strcmp("bne",piece) || !strcmp("red",piece) || !strcmp("prn",piece) || !strcmp("jsr",piece))
        {
            if(piece==NULL)
            {
                warningFlag = ON;
                fprintf(stderr,"in file secondrun.c:line %d:the opcode %s doesn't have source addressing,please revise.",lineCnt,piece);
                return 0;
            }
            else if(check_type_of_second_operand(lineCnt)==IA)
            {
                warningFlag = ON;
                fprintf(stderr,"in file secondrun.c:line %d:the opcode %s cannot have immediate addressing in the destination operand,please revise.",lineCnt,piece);
                return 0;
            }
        }
        else if(!strcmp("lea",piece))
        {
            if(firstOperandType != DA)
            {
                warningFlag = ON;
                fprintf(stderr,"in file secondrun.c:line %d:the opcode lea can have only direct addressing method,please revise.\n",lineCnt);
                return 0;
            }
        }

    }
    if(opCnt == ONE)/*case of one operand*/
    {
        if(!strcmp("clr",piece) || !strcmp("not",piece) || !strcmp("inc",piece) ||
           !strcmp("dec",piece) || !strcmp("red",piece))
        {
            if(firstOperandType==IA)
            {
                warningFlag = ON;
                fprintf(stderr,"in file secondrun.c:line %d:illegal destination operand ,please revise.\n",lineCnt);
                return 0;
            }
        }
        else if(!strcmp("jmp",piece) || !strcmp("bne",piece) || !strcmp("jsr",piece))
        {
            if(firstOperandType == IA || firstOperandType == IRA)
            {
                warningFlag = ON;
                fprintf(stderr,"in file secondrun.c:line %d:illegal destination operand ,please revise.\n",lineCnt);
                return 0;
            }
        }

    }
    return 1;
}
/*file_concatenation_obj : This function concatenating the suffix ".obj" the current file name
 * param 1)str1 - the file name
 *       2)str3 - the file name + ".obj" extension
 *       3)str2 - the extension ".obj"*/
void file_concatenation_obj(char *str1,char str3[],char *str2,int length)
{
    char piece[MAX_ARRAY];
    strcpy(piece,str1);
    strcat(piece,str2);
    memcpy(str3,piece,length + EXTENSION);
}
/*file_concatenation_ext : This function concatenating the suffix ".ext" the current file name
 * param 1)str1 - the file name
 *       2)str3 - the file name + ".ext" extension
 *       3)str2 - the extension ".ext"*/
void file_concatenation_ext(char str1[],char str3[],char *str2,int length)
{
    char piece[MAX_ARRAY];
    strcpy(piece,str1);
    strcat(piece,str2);
    memcpy(str3,piece,length + EXTENSION);
}
/*file_concatenation_ent : This function concatenating the suffix ".ent" the current file name
 * param 1)str1 - the file name
 *       2)str3 - the file name + ".ent" extension
 *       3)str2 - the extension ".ent"*/
void file_concatenation_ent(char str1[],char str3[],char *str2,int length)
{
    char piece[MAX_ARRAY];
    strcpy(piece,str1);
    strcat(piece,str2);
    memcpy(str3,piece,length + EXTENSION);
}
/*update_ext_array : This function `updates the ext_array depicted in the project.h file
 * param 1)str - a string token
 *       2)length - the string length
 *       3)IC - the instruction counter
 *       4)the specific type {entry OR extern}*/
void update_ext_array(char *str,int length,int IC,enum type5 type)
{
    char piece[MAX_ARRAY];
    pointer newNode,curr;
    strcpy(piece,str);
    length = strlen(piece);
    newNode=(pointer)malloc(sizeof(extry));
    strcpy(str,piece);
    if(type == EXTE)
        memcpy(newNode->labelExt,piece,length);
    else if (type == ENT)
        memcpy(newNode->labelEnt,piece,length);
    newNode->address = IC;
    newNode->nxt = NULL;
    newNode->type = type;
    if(EXN==NULL)
    {
        EXN = newNode;
        return;
    }
    curr =  EXN;
    while(curr->nxt!=NULL)
        curr = curr->nxt;
    curr->nxt = newNode;
}
/*create_ext_file : This function creates the requested external file
 * param 1)NONE*/
void create_ext_file()
{
    pointer curr;
    curr = EXN;
    while(curr)
    {
        if(curr->type==EXTE)
            fprintf(fd4,"%s \t %d\n",curr->labelExt,curr->address);
        curr = curr->nxt;
    }
}
/*create_ent_file : This function creates the requested entry file
 * param 1)NONE*/
void create_ent_file()
{
    pointer curr;
    curr = EXN;
    while(curr)
    {
        if(curr->type==ENT)
            fprintf(fd3,"%s \t %d\n",curr->labelEnt,curr->address);
        curr = curr->nxt;
    }
}
/*check_if_comment_line :This function determine if a specific line in the given text is a comment line starts with ':'*/
/*return 1 is it does,else zero*/
int check_if_comment_line(char *str)
{
    int i = 0;
    for(;i<strlen(str);i++)
    {
        if(!isspace(str[i]))
            break;
    }
    if(str[i]==';')
        return 1;
    return 0;
}/*initialize_array_to_zero : This function initialize the global variable buf to zero
param 1)NONE*/
void initialize_array_to_zero(char buf[])
{
    int i;
    for(i = 0;i<MAX_ARRAY;i++)
    {
        buf[i] = ZERO;
    }
}

