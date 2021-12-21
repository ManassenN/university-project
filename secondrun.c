#include "project.h"
extern ptr HSL;
extern int IC,DC,warningFlag,extCnt,entCnt;
extern FILE *fd1,*fd3,*fd4;
/*the second read/transition of the program as follows :
 *  MAIN GOAL : because in the first transition we couldn't encode the image code because we were lack of information of symbol
    that yet to be defined, only after the assembler has been read the file once such that all the symbols have been
    addressed the assembler can complete the encoding,the process as it follows :*/
void second_read_run(int lineCnt)
{
    enum type2 firstOp;
    int operandCnt,length;
    lineCnt = 1;
    IC = 100;
    while(FOREVER)
    {
    		int flag = OFF;
        int  j = 0;
        initialize_array_to_zero(buf);
        fgets(buf,sizeof(buf),fd1);/*2*/	/*get next line in the specific text*/
        if(buf[ZERO] == '\n')
            buf[ZERO] = 0;
        lineCnt++;

        INITIALIZE_BUFFERS
       
        /*if the current line from the text is empty-line OR comment line we will proceed to the next line*/
        if(!check_if_line_is_empty(buf) && !check_if_comment_line(buf))
        {
            token = get_first_word(buf);
            while(FOREVER)
            {

                if(!check_symbol(token,flag,lineCnt)) /*3*/
                    token = get_next_word(++j);
                if(!identify_string(token) || !identify_data(token) || !check_if_extern_guidance_sentence_ver2(token,extCnt))/*4*/
                    break;

                if(!check_if_entry_guidance_sentence(token,entCnt))/*5*/
                {
                    flag = ON;
                    token = get_next_word(++j);
                    if(!check_symbol(token,flag,lineCnt))
                    {
                        length = strlen(token);
                        update_entry(token);/*6*/ /*reminder:(word,IC\DC,extern,guidance,entry)*/
                        update_ext_array(token,length,IC,ENT);
                        flag = OFF;
                        break;
                    }
                }
                /**************************start of the addressing methods******************************/
                operandCnt = operand_counter();
                if(operandCnt)
                    firstOp = check_type_of_first_operand(lineCnt);
                if(firstOp==DEFAULT)
                    break;
                if(!check_valid_opcodes_to_operands(lineCnt))
                    break;
                /*coding and initializing the first word*/
                codeImage[IC++] = initialize_first_word_to_imageCode_array(firstOp,operandCnt,lineCnt);
                if(operandCnt)
                    /*coding and initializing the second word*/
                    codeImage[IC++] = initialize_second_word_to_imageCode_array(lineCnt);
                /*coding and initializing the third word if there are two operands*/
                if(operand_counter()==TWO&&check_if_two_operands_are_register())
                    codeImage[IC++] = initialize_third_word_to_imageCode_array(lineCnt);
                break;
            }
        }
        if(feof(fd1))
            break;
    }

}

