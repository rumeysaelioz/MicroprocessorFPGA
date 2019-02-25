#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Converts a hexadecimal string to integer.
int hex2int( char* hex)  
{
    int result=0;

    while ((*hex)!='\0')
    {
        if (('0'<=(*hex))&&((*hex)<='9'))
            result = result*16 + (*hex) -'0';
        else if (('a'<=(*hex))&&((*hex)<='f'))
            result = result*16 + (*hex) -'a'+10;
        else if (('A'<=(*hex))&&((*hex)<='F'))
            result = result*16 + (*hex) -'A'+10; 
        hex++;
    }
    return(result);
}

int main()
{     
    FILE *fp;
    char line[100];
    char *token = NULL;
    char *op1, *op2, *op3;
    char ch;
    int  chch;

    int program[1000];
    int counter=0;  //holds the address of the machine code instruction
    int dataarea=512;


    fp = fopen("instructions.txt","r");

    while(fgets(line,sizeof line,fp)!= NULL)
    {
            token=strtok(line,"\n\t\r ");  //get the instruction mnemonic or labe

            if (strcmp(token,"ldi")==0)        //---------------LDI INSTRUCTION--------------------
            {
                    op1 = strtok(NULL,"\n\t\r ");                                //get the 1st operand of ldi, which is the register that ldi loads
                    op2 = strtok(NULL,"\n\t\r ");                                //get the 2nd operand of ldi, which is the data that is to be loaded
                    program[counter]=0x4000+hex2int(op1);                        //generate the first 16-bit of the ldi instruction
                    counter++;                                                   //move to the second 16-bit of the ldi instruction
                    if ((op2[0]=='0')&&(op2[1]=='x'))                            //if the 2nd operand is twos complement hexadecimal
                        program[counter]=hex2int(op2+2)&0xffff;              //convert it to integer and form the second 16-bit 
                    else if ((  (op2[0])=='-') || ((op2[0]>='0')&&(op2[0]<='9')))       //if the 2nd operand is decimal 
                        program[counter]=atoi(op2)&0xffff;                         //convert it to integer and form the second 16-bit 
                    else                                                           //if the second operand is not decimal or hexadecimal, it is a laber or a variable.
                    {                                                               //in this case, the 2nd 16-bits of the ldi instruction cannot be generated.
                        printf("unrecognizable LDI offset\n");
                    }        
                    counter++;                                                    //skip to the next memory location 
            }                                       
            else if (strcmp(token,"add")==0) //----------------- ADD -------------------------------
            {
               	op1 = strtok(NULL,"\n\t\r ");  //dest  
            	op2 = strtok(NULL,"\n\t\r ");  //src1
                op3 = strtok(NULL,"\n\t\r ");  //src2
                chch = (op1[0]-48)| ((op3[0]-48)<<3)|((op2[0]-48)<<6);  
            	program[counter]=0x8000+((chch)&0x00ff); //0
                counter++; 
            }
            else if (strcmp(token,"sub")==0)
            {
                op1 = strtok(NULL,"\n\t\r ");
                op2 = strtok(NULL,"\n\t\r ");
                op3 = strtok(NULL,"\n\t\r ");
                chch = (op1[0]-48) | ((op3[0]-48)<<3) | ((op2[0]-48)<<6);
                program[counter]=0x8100+((chch)&0x00ff);  //1
                counter++;
            }
            else if (strcmp(token,"and")==0)
            {
                op1 = strtok(NULL,"\n\t\r ");
                op2 = strtok(NULL,"\n\t\r ");
                op3 = strtok(NULL,"\n\t\r ");
            	chch = (op1[0]-48) | ((op3[0]-48)<<3) | ((op2[0]-48)<<6);
                program[counter]=0x8200+((chch)&0x00ff); //2
                counter++;
            }
            else if (strcmp(token,"or")==0)
            {
                    op1 = strtok(NULL,"\n\t\r ");
                    op2 = strtok(NULL,"\n\t\r "); //src1
                    op3 = strtok(NULL,"\n\t\r "); //src2
                    chch = (op1[0]-48) | ((op3[0]-48)<<3) | ((op2[0]-48)<<6);
                    program[counter]=0x8300+((chch)&0x00ff); //3
        			counter++;
            }
            else if (strcmp(token,"xor")==0)
            {
                op1 = strtok(NULL,"\n\t\r ");
                op2 = strtok(NULL,"\n\t\r ");
                op3 = strtok(NULL,"\n\t\r ");
                chch = (op1[0]-48) | ((op3[0]-48)<<3) | ((op2[0]-48)<<6);
                program[counter]=0x8400+((chch)&0x00ff); //4
				counter++;
            }                        
            else if (strcmp(token,"not")==0)
                {
            	op1 = strtok(NULL,"\n\t\r ");
                op2 = strtok(NULL,"\n\t\r ");
                ch = (op1[0]-48)| ((op2[0]-48)<<3);
                program[counter]=0x8500+((ch)&0x00ff);  //5
                counter++;
            }
            else if (strcmp(token,"mov")==0)
            {
                op1 = strtok(NULL,"\n\t\r ");
				op2 = strtok(NULL,"\n\t\r ");
				ch = (op1[0]-48) | ((op2[0]-48)<<3);
				program[counter]=0x8600+((ch)&0x00ff); // 6
				counter++;
            }
            else if (strcmp(token,"inc")==0)
            {
                op1 = strtok(NULL,"\n\t\r ");
            	ch = (op1[0]-48)| ((op1[0]-48)<<3);
                program[counter]=0x8700+((ch)&0x00ff);  //7
            	counter++;
            }
            else if (strcmp(token,"dec")==0)
            {
                op1 = strtok(NULL,"\n\t\r ");
				ch = (op1[0]-48) | ((op1[0]-48)<<3);
				program[counter]=0x8800+((ch)&0x00ff); // 8
				counter++;
            }
            else //------WHAT IS ENCOUNTERED IS NOT A VALID INSTRUCTION OPCODE
            {
                     printf("no valid opcode\n");
            } 
     } //while
     int i; 
     fclose(fp);
     fp = fopen("RAM","w");
     //fprintf(fp,"v2.0 raw\n");  //needed for logisim, remove this line for verilog..
     for (i=0;i<counter; i++)  //complete this for memory size in verilog
            fprintf(fp,"%04x\n",program[i]);
} //main
