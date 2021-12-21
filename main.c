#include "project.h"


/*---------Global variables--------------*(*necessary ones*)*/
ptr HSL=NULL; /*The head of the linked list of the symbol_table*/
pointer EXN = NULL;/*The head of the linked list of the extry type*/
/*IC,DC,warningFlag,symbolWarningFlag,extCnt,entCnt : all these variables are in fluency usage during
 * the execution of all file,hat why I have decided to use them as global variables */
int IC =IC_INITIAL, DC =DC_INITIAL,warningFlag = OFF,symbolWarningFlag=OFF ,extCnt = 0,entCnt = 0;
FILE *fd1 = NULL;	/*A pointer of type FILE that we can get access to the file characters*/
FILE *fd3,*fd4,*fd5; /*the file pointers that I used in all the 5 files that why I have decided to use them as global variables*/


/*---------------------------------------*/
int main(int argc, char *argv[])
{
/*--------------------Local variables------------------*/
    int i = 1;
    int len;

/*----------------------------------------------------*/

    /*start of the first iteration!*/
    for(;i<argc;i++)
    {		
    		
    		int lineCnt = ONE;
        char arr1[MAX_ARRAY],arr2[MAX_ARRAY],arr3[MAX_ARRAY];
        HSL=NULL,EXN = NULL;
        IC =IC_INITIAL, DC =DC_INITIAL;
        len = strlen(argv[i]);
        if(argc<TWO)
        {
            fprintf(stdout,"Error! no arguments were delivered!\n");
            exit(0);
        }
        fprintf(stdout,"opening file %s...\n",argv[i]);
        if(!(fd1 = fopen(argv[i],"r+")))
        {
            fprintf(stdout,"Error! cannot open file \n");
            exit(1);
        }
        file_concatenation_ent(argv[i],arr1,".ent",len);
        if(!(fd3 = fopen(arr1,"w")))
        {
            fprintf(stdout,"Error! cannot open file \n");
            exit(1);
        }
        file_concatenation_obj(argv[i],arr2,".obj",len);
        if(!(fd5 = fopen(arr2,"w")))
        {
            fprintf(stdout,"Error! cannot open file \n");
            exit(1);
        }
        file_concatenation_ext(argv[i],arr3,".ext",len);
        if(!(fd4 = fopen(arr3,"w")))
        {
            fprintf(stdout,"Error! cannot open file \n");
            exit(1);
        }



        /*---------------------------first read ------------------------------*/
        first_read_run(lineCnt);
        /*--------------------------------------------------------------------*/
       
        update_data_symbol_address(HSL);
        rewind(fd1);
        if(!check_warnings(warningFlag))
        {
            fprintf(stdout,"there are error occurrences in the first transition,therefor second transition will stay on halt\n");
            fprintf(stdout,"program exits...\n\n.");
            exit(1);
        }
            fprintf(stdout,"first transition ended successfully, proceeding...\n");   
        /*---------------------------second read ------------------------------*/
        second_read_run(lineCnt);
        /*---------------------------------------------------------------------*/
                
        if(!check_warnings(warningFlag))
        {
            printf("NOTICE:there are error occurrences in the second transition,therefor creating the files will stay on halt\n");
            printf("\n**program exits**\n");
            fclose(fd1);
            fclose(fd5);
            fclose(fd3);
            fclose(fd4);
            exit(1);
        }
        
        else 
        {
        		fprintf(stdout,"second transition ended successfully, proceeding...\n");
            fprintf(stdout,"program terminated successfully. \n");/*all clear we are ready to create the files!*/
            fprintf(stdout,"Creating the files...\n\n");
            create_obj_file(dataImage,codeImage);
            create_ext_file();
            create_ent_file();
        }



    }	/*end of for loop*/
    fprintf(stdout,"program exits...\n");
    fclose(fd1);
    fclose(fd5);
    fclose(fd3);
    fclose(fd4);
    free(HSL);
    free(EXN);
    return 0;
}
/*end of main*/

