#include "project.h"
extern ptr HSL;
extern int IC,DC,extCnt,entCnt,warningFlag,symbolWarningFlag;
extern FILE *fd1;

/*the first read/transition of the program as follows :
 *MAIN GOAL : in the first transition, we will determine which address will be attached to a specific symbol
 * to achieve this goal we will count with a designated counter named IC(instruction counter)
 * which encodes line by line to a designated arrays named dataImage,codeImage (the coding process will occur in the
 * second run)
 * the numbers that attached to the code below (example : /1/) is to indicate the current
 * stage number in the process */
void first_read_run(int lineCnt)
{
    int num = 0;
    int flag,symbolFlag= OFF;
    int length;
    while(FOREVER)
    {
        int  j = 0;
        initialize_array_to_zero(buf);
        fgets(buf,sizeof(buf),fd1);/*2*/	/*get next line in the specific text*/
        if(buf[ZERO] == '\n')
            buf[ZERO] = 0;
        if(feof(fd1))
            break;
        lineCnt++;
        if(!check_syntax(lineCnt)) /*checking the syntax of the line*/
            break;
        INITIALIZE_BUFFERS
        /*if the current line from the text is empty-line OR comment line we will proceed to the next line*/
        if(!check_if_line_is_empty() && !check_if_comment_line(buf))
        {
            token = get_first_word(buf);/*1*/
            while(FOREVER)
            {

                if(!check_symbol(token,OFF,lineCnt)&&!check_if_symbol_already_exist(token,lineCnt)) /*3*/
                    symbolFlag = ON;/*4*/
                if(symbolWarningFlag == ON)
                {
                    warningFlag = ON;
                    symbolWarningFlag = OFF;
                    break;
                }
                if(!check_if_it_is_guidance_sentence())/*5*/
                {
                    if(symbolFlag == ON)
                    {
                        token = cut_last_character_from_symbol(token);
                        add_symbol_to_symbol_table(token,DC,NO,YES,NO);/*6*//*reminder : (symbol_name,address IC/DC,is it extern?,is it guidance?,is it entry?)*/
                        symbolFlag = OFF;
                        token = get_next_word(++j);
                    }

                    if(!identify_string(token))
                    {
                        token = get_next_word(++j);
                        if(!token)
                        {
                            warningFlag = ON;
                            fprintf(stderr,"line %d:missing string\n",lineCnt);
                            break;
                        }
                        length = strlen(token);
                        add_string_to_data_image(token,length);
                        break;
                    }
                    else if(!identify_data(token))
                    {
                        token = get_next_word(++j);
                        while(token)
                        {
                            num = atoi(token);
                            dataImage[DC++] = num;
                            token = get_next_word(++j);
                        }
                        j = 0;
                        break;
                    }

                }
                /*determine if the line is entry or extern*/
                if(!check_if_entry_guidance_sentence(token,entCnt) || !check_if_extern_guidance_sentence_ver2(token,extCnt))/*8*/
                {
                    if(!check_if_entry_guidance_sentence(token,entCnt))/*9*/
                        break;
                    if(!check_if_extern_guidance_sentence_ver2(token,extCnt))/*10*/
                    {
                        token = get_next_word(++j);                     
                        add_symbol_to_symbol_table(token,ZERO_ADDRESS,YES,UNKNOWN,NO);/*reminder : (symbol_name,address IC/DC,is it extern?,is it guidance?,is it entry?)*/
                        break;

                    }
                }
                /*if we the code reached here that means that we are on instruction sentence*/
                if(symbolFlag == ON)/*11*/
                {
                    token = cut_last_character_from_symbol(token);
                    add_symbol_to_symbol_table(token,IC,NO,NO,NO);
                    symbolFlag = OFF;
                    token = get_next_word(++j);
                    if(token==NULL)
                   	 {
                        warningFlag = ON;
                        fprintf(stdout,"in file firstrun.c:line %d:there is a missing instruction/guidance sentence,please revise. \n",lineCnt);
                        break;
                   	 }
                     else if(!check_if_entry_guidance_sentence(token,entCnt) || !check_if_extern_guidance_sentence_ver2(token,extCnt))
                   	 {
                        fprintf(stdout,"NOTICE: putting label before entry OR extern is irrelevant!\n");
                        break;
                   	 }
                }
                if(token==NULL)
                    break;
                /*from this part to the end of this section,we will count the number of memory slots*/
                while(token)
                {
                    if(!check_opcode(token))/*12*/
                    {
                        if (!check_valid_opcodes(token,lineCnt))
                        {
                            IC++;
                            token = get_next_word(++j);
                        } else
                            token = NULL;
                    }

                    else if(!check_if_two_operands_are_register())
                    {
                        IC++;
                        break;
                    }

                    else if(!check_if_word_is_register(token))/*13*+*14*/
                    {
                        IC++;
                        token = get_next_word(++j);
                    }
                    else if(j!=0)
                    {
                        flag = ON;
                        if(!check_symbol(token,flag,lineCnt))
                        {
                            IC++;
                            flag = OFF;
                            token = get_next_word(++j);
                        }
                        else if(!check_immediate_value(token,lineCnt))
                        {
                            IC++;
                            token = get_next_word(++j);
                        }
                        else
                        {
                            fprintf(stderr,"in file firstrun.c: line %d :the token %s is not valid,please enter a valid token\n",lineCnt,token);
                            warningFlag = ON;
                            token = get_next_word(++j);
                            break;
                        }
                    }
                }/*end of inner1 while*/
                j = 0;
                break;
            }/*end of inner2 while*/

        }	/*end of outer if*/
        else continue;

    }/*end of outter while*/

}

