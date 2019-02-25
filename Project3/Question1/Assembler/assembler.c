#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *decimal_to_binary(int);
int bin_to_hex(int);


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

main()
{

	FILE *fp;
    char line[100];
    char *token = NULL;
	char *op1, *op2, *op3, *label;
	char ch;
	int  chch;
    char sp = '9';
    int jumpdistance;

	int program[1000];
	int instructions[1000];
	int counter=0; 
	char* R0, *R1, *R2, *R3, *R4, *R5, *R6, *R7 ;
	R0 = decimal_to_binary(0); // R0 = 000
	R1 =decimal_to_binary(1);
	R2 = decimal_to_binary(2);
	R3 = decimal_to_binary(3);
	R4 = decimal_to_binary(4);
	R5 = decimal_to_binary(5);
	R6 = decimal_to_binary(6);
	R7 = decimal_to_binary(7); // R7 means 111

	struct label
	{
		int location;
		char *label;
	};
	struct label labeltable[50];
	int nooflabels = 0; 


	struct jumpinstruction
	{
		int location;
		char *label;
	};
	struct jumpinstruction jumptable[100];
	int noofjumps=0;  


	struct variable
	{
		int location;
		char *name;
	};
	struct variable variabletable[50];
	int noofvariables = 0;


	struct ldiinstruction
	{
		int location;
		char *name;
	};
	struct ldiinstruction lditable[100];
	int noofldis=0;



	fp = fopen("instructions.txt","r");

	if (fp != NULL)
	{
        counter = 0;
        while(fgets(line,sizeof line,fp)!= NULL)  
		{
			token=strtok(line,"\n\t\r ");
			if (strcmp(token,".code")==0)
			{
				printf("\nsegment .code\n");
				break;
			}
		}
		while(fgets(line,sizeof line,fp)!= NULL) 
		{
			token=strtok(line,"\n\t\r ");
			if (token[strlen(token) - 1] == ':')
			{
				char *p = token;
                p[strlen(token)-1] = 0;
                printf("label\t%s\t%d\n", token, counter);
                labeltable[nooflabels].location = counter;  
                op1=(char*)malloc(sizeof(token));
                strcpy(op1,token);
                labeltable[nooflabels].label=op1;
                nooflabels++;

                counter--;
			}

            if (strcmp(token,"ldi")==0)
            {
                counter++;
                counter++;
               
            }
            else
                counter++;
                
            
		}

    }


    fp = fopen("instructions.txt","r");

        counter = 0;
        
        while(fgets(line,sizeof line,fp)!= NULL)  
		{
			token=strtok(line,"\n\t\r ");
			if (strcmp(token,".code")==0)
			{
				printf("\nsegment .code\n");
				break;
			}
		}
		while(fgets(line,sizeof line,fp)!= NULL)
		{
			token=strtok(line,"\n\t\r ");  

//  FIRST PASS 
			while (token)
			{
				if (strcmp(token,"ldi")==0) {
					op1 = strtok(NULL,"\n\t\r ");                                //get the 1st operand of ldi, which is the register that ldi loads
					op2 = strtok(NULL,"\n\t\r ");                                //get the 2nd operand of ldi, which is the data that is to be loaded
					printf("\n\t%s\t%s   %s\n",strupr(token),op1,op2);			// print instruction
					program[counter]=0x1000+(int)strtol(op1, NULL, 0);                        //generate the first 16-bit of the ldi instruction
					printf("> %d\t%04x\n",counter,program[counter]);
					counter++;                                                   //move to the second 16-bit of the ldi instruction
					if ((op2[0]=='0')&&(op2[1]=='x')) {
					                         
						program[counter]=hex2int(op2+2)&0xffff; 
						}                 
					else if ((  (op2[0])=='-') || ((op2[0]>='0')&&(op2[0]<='9'))){
					      
						program[counter]=atoi(op2)&0xffff;  
						}                      
					else                                                           //if the second operand is not decimal or hexadecimal, it is a laber or a variable.
					{                                                                //in this case, the 2nd 16-bits of the ldi instruction cannot be generated.
						lditable[noofldis].location = counter;                 //record the location of this 2nd 16-bit
						op1=(char*)malloc(sizeof(op2));                         //and the name of the label/variable that it must contain
						strcpy(op1,op2);                                        //in the lditable array.
						lditable[noofldis].name = op1;
						noofldis++;
					}
					printf("> %d\t%04x\n",counter,program[counter]);
					counter++; 
					printf("%d",counter);                                                    
				}
				else if (strcmp(token,"ld")==0)     
				{
				
					op1 = strtok(NULL,"\n\t\r ");                //get the 1st operand of ld, which is the destination register
                    op2 = strtok(NULL,"\n\t\r ");                //get the 2nd operand of ld, which is the source register
                    printf("\n\t%s\t%s   %s\n",strupr(token),op1,op2);
                    int dest = op1[0]-48 ; // dest
                    int src1 = op2[0]-48 ; // src2
                    char str_st[9];
                    unsigned int ch2;
                	char* dest_1, *src1_2;
                	dest_1 = decimal_to_binary(dest);
					src1_2 = decimal_to_binary(src1);
					strcpy(str_st,"");
                    strcat(str_st, R0);
					strcat(str_st, R0);
					strcat(str_st,src1_2);
					strcat(str_st,dest_1);
					int num = atoi(str_st);
					ch2 = bin_to_hex(num);
				    program[counter]=0x2000+((ch2)&0xffff);       
                    printf("> %d\t%04x\n",counter,program[counter]);
                    counter++;
                    free(src1_2);
					free(dest_1);
				                               //skip to the next empty location in memory
				}
				else if (strcmp(token,"st")==0) 
				{
                    op1 = strtok(NULL,"\n\t\r ");                //get the 1st operand of ld, which is the destination register
                    op2 = strtok(NULL,"\n\t\r ");                //get the 2nd operand of ld, which is the source register
                    printf("\n\t%s\t%s   %s\n",strupr(token),op1,op2);
                    int dest = op1[0]-48 ; // dest
                    int src1 = op2[0]-48 ; // src1
                    char str_st[20];
                	char* dest_1, *src1_2;
                	dest_1 = decimal_to_binary(dest);
					src1_2 = decimal_to_binary(src1);
                    strcpy(str_st, R0);
					strcat(str_st, src1_2);
					strcat(str_st,dest_1);
					strcat(str_st,R0);
					unsigned int ch = strtol(str_st, NULL,2);
					free(dest_1);
					free(src1_2);
                    program[counter]=0x3000+((ch)&0x01ff);       //form the instruction and write it to memory
                    printf("> %d\t%04x\n",counter,program[counter]);
                    counter++;                                   //skip to the next empty location in memory
				}
				
				else if (strcmp(token,"jmp")==0)  //-------------- JUMP -----------------------------
				{
					op1 = strtok(NULL,"\n\t\r ");			//read the label
					printf("\n\t%s\t%s\n",strupr(token),op1);
					jumptable[noofjumps].location = counter;	//write the jmp instruction's location into the jumptable
					op2=(char*)malloc(sizeof(op1)); 		//allocate space for the label
					strcpy(op2,op1);				//copy the label into the allocated space
					jumptable[noofjumps].label=op2;			//point to the label from the jumptable
					noofjumps++;					//skip to the next empty location in jumptable

					program[counter]=0x4000;			
                    int n;
                    

					printf("> %d\t%04x\n",counter,program[counter]);
					counter++;					//skip to the next empty location in memory.
				}
			else if (strcmp(token,"jz")==0) //------------- CONDITIONAL JUMP ------------------
				{
                    op1 = strtok(NULL,"\n\t\r ");			//read the label
                    printf("\n\t%s\t%s\n",strupr(token),op1);
                    jumptable[noofjumps].location = counter;	//write the jz instruction's location into the jumptable
                    op2=(char*)malloc(sizeof(op1)); 		//allocate space for the label
                    strcpy(op2,op1);				//copy the label into the allocated space
                    jumptable[noofjumps].label=op2;			//point to the label from the jumptable
                    noofjumps++;					//skip to the next empty location in jumptable

                    program[counter]=0x5000;
                    int n;
                    

                    printf("> %d\t%04x\n",counter,program[counter]);
                    counter++;					//skip to the next empty location in memory.
				}
				else if (strcmp(token,"add")==0) 
				{
					op1 = strtok(NULL,"\n\t\r "); //dest
					op2 = strtok(NULL,"\n\t\r "); // src1
					op3 = strtok(NULL,"\n\t\r "); // src2
					printf("\n\t%s\t%s   %s   %s\n",strupr(token),op1,op2,op3);
					chch = (op1[0]-48) | ((op3[0]-48)<<3) | ((op2[0]-48)<<6) | (0<<9);
					program[counter]=0x7000+((chch)&0xffff); // 7 opcode of ALU operations, 0 ALU code of ADD operation
					printf("> %d\t%04x\n",counter,program[counter]);
					counter++;
				}
				else if (strcmp(token,"sub")==0)
				{
					
                    op1 = strtok(NULL,"\n\t\r "); //dest
                    op2 = strtok(NULL,"\n\t\r "); // src1
                    op3 = strtok(NULL,"\n\t\r "); // src2
                    printf("\n\t%s\t%s   %s   %s\n",strupr(token),op1,op2,op3);
                    chch = (op1[0]-48) | ((op3[0]-48)<<3) | ((op2[0]-48)<<6) | (1<<9) ;
                    program[counter]=0x7000+((chch)&0xffff); // 7 opcode of ALU operations, 1 ALU code of SUB operation
                    printf("> %d\t%04x\n",counter,program[counter]);
                    counter++;
				}
				else if (strcmp(token,"and")==0)
				{	
                    op1 = strtok(NULL,"\n\t\r "); //dest
                    op2 = strtok(NULL,"\n\t\r "); //src1
                    op3 = strtok(NULL,"\n\t\r "); //src2
                    printf("\n\t%s\t%s   %s   %s\n",strupr(token),op1,op2,op3);
                    
					int dest = op1[0]-48 ;
                    int src1 = op2[0]-48 ;
                    int src2 = op3[0]-48 ;
               		char str[20];
                	char* dest_1, * src1_2, *src2_2;
                	dest_1 = decimal_to_binary(dest);
					src1_2 = decimal_to_binary(src1);
					src2_2 = decimal_to_binary(src2);		
					strcpy(str, R2); // 7 opcode of ALU operations, 2 ALU code of AND operation
					strcat(str, src1_2);
					strcat(str,src2_2);
					strcat(str,dest_1);
					unsigned int ch = strtol(str, NULL,2);
					free(dest_1);
					free(src1_2);
					free(src2_2);
                    program[counter]=0x7000+((ch)&0xffff); 
                    printf("> %d\t%04x\n",counter,program[counter]);
                    counter++;
				}
				else if (strcmp(token,"or")==0)
				{
                    op1 = strtok(NULL,"\n\t\r ");
                    op2 = strtok(NULL,"\n\t\r ");
                    op3 = strtok(NULL,"\n\t\r ");
                    int dest = op1[0]-48 ;
                    int src1 = op2[0]-48 ;
                    int src2 = op3[0]-48 ;
                    printf("\n\t%s\t%s   %s   %s\n",strupr(token),op1,op2,op3);
					char binary[20];
               		char* dest_1, * src1_2, *src2_2;
                	dest_1 = decimal_to_binary(dest);
					src1_2 = decimal_to_binary(src1);
					src2_2 = decimal_to_binary(src2);		
					strcpy(binary, R3); // 7 opcode of ALU operations, 3 ALU code of AND operation
					strcat(binary, src1_2);
					strcat(binary,src2_2);
					strcat(binary,dest_1);
					unsigned int ch = strtol(binary, NULL,2);
					free(dest_1);
					free(src1_2);
					free(src2_2);
                    
                    program[counter]=0x7000+((ch)&0xffff);
                    
                    printf("> %d\t%04x\n",counter,program[counter]);
                    counter++;
				}
				else if (strcmp(token,"xor")==0)
				{
					op1 = strtok(NULL,"\n\t\r ");
                    op2 = strtok(NULL,"\n\t\r ");
                    op3 = strtok(NULL,"\n\t\r ");
                    int dest = op1[0]-48 ;
                    int src1 = op2[0]-48 ;
                    int src2 = op3[0]-48 ;
                    printf("\n\t%s\t%s   %s   %s\n",strupr(token),op1,op2,op3);
					char binary_code[20];
               		char* dest_1, * src1_2, *src2_2;
                	dest_1 = decimal_to_binary(dest);
					src1_2 = decimal_to_binary(src1);
					src2_2 = decimal_to_binary(src2);		
					strcpy(binary_code, R4); // 7 opcode of ALU operations, 4 ALU code of AND operation
					strcat(binary_code, src1_2);
					strcat(binary_code,src2_2);
					strcat(binary_code,dest_1);
					unsigned int ch = strtol(binary_code, NULL,2);
					free(dest_1);
					free(src1_2);
					free(src2_2);
                    program[counter]=0x7000+((ch)&0xffff); // 7 opcode of ALU operations, 4 ALU code of OR operation
                    
					printf("> %d\t%04x\n",counter,program[counter]);
                    counter++;
				}
				else if (strcmp(token,"not")== 0)
				{
					op1 = strtok(NULL,"\n\t\r ");  // dest
					op2 = strtok(NULL,"\n\t\r ");  //src2
					printf("\n\t%s\t%s   %s\n",strupr(token),op1,op2);
					ch = (op1[0]-48) | ((op2[0]-48)<<3 | 0<<6 | 7 << 9);
					program[counter]=0x7e00+((ch)&0xffff); // 7 opcode of ALU operations, 5 ALU code of not operation
					printf("> %d\t%04x\n",counter,program[counter]);
					counter++;
				}
				else if (strcmp(token,"mov")==0)
				{
                    op1 = strtok(NULL,"\n\t\r ");
					op2 = strtok(NULL,"\n\t\r ");
					printf("\n\t%s\t%s   %s\n",strupr(token),op1,op2);
					ch = (op1[0]-48) | ((op2[0]-48)<<3) | 1<<6  ;
					program[counter]=0x7e00+((ch)&0xffff); // 7 opcode of ALU operations, 6 ALU code of MOV operation
					printf("> %d\t%04x\n",counter,program[counter]);
					counter++;
				}
				else if (strcmp(token,"inc")==0)
				{
					op1 = strtok(NULL,"\n\t\r ");
					printf("\n\t%s\t%s\n",strupr(token),op1);
					int x =op1[0]-48;
					ch = (x | ((x)<<3)) ;
					program[counter]=0x7e80+((ch)&0xffff); // 7 opcode of ALU operations, 7 ALU code of INC operation
					printf("> %d\t%04x\n",counter,program[counter]);
					counter++;
				}
				else if (strcmp(token,"dec")==0)
				{
                    op1 = strtok(NULL,"\n\t\r ");
					printf("\n\t%s\t%s\n",strupr(token),op1);
					int x = op1[0]-48;
					ch = (x) | ((x)<<3);
					program[counter]=0x7ec0+((ch)&0xffff); // 0111 / 111 / 011 / op1 / op1
					printf("> %d\t%04x\n",counter,program[counter]);
					counter++;
				}
                else if (strcmp(token,"push")==0) 
                {
					op1 = strtok(NULL,"\n\t\r ");
					printf("\n\t%s\t%s\n",strupr(token),op1);
					int x = op1[0]-48 ;
					char *pointer,str[30];
					pointer = decimal_to_binary(x);
					strcpy(str, R0);
					strcat(str, pointer); 	
					strcat(str, R7); // -< stack reg
					strcat(str, R0);
					free(pointer);
					unsigned int ch = strtol(str, NULL, 2);
					program[counter]=0x8000+((ch)&0x01ff); 
					printf("> %d\t%04x\n",counter,program[counter]);
					counter++;
                }
                else if (strcmp(token,"pop")==0) 
                {
                    op1 = strtok(NULL,"\n\t\r ");
					printf("\n\t%s\t%s\n",strupr(token),op1);
					int x = op1[0]-48 ;
					char *pointer,str[30];
					pointer = decimal_to_binary(x);
					strcpy(str, R0);
					strcat(str, R0);
					strcat(str, R7);
					strcat(str, pointer);
					free(pointer);
					unsigned int ch = strtol(str, NULL, 2);	
					program[counter]=0x9000+((ch)&0xffff); // 7 opcode of ALU operations, 8 ALU code of DEC operation
					
					printf("> %d\t%04x\n",counter,program[counter]);
					counter++;
                }
                else if (strcmp(token,"call")==0) 
                {
                 op1 = strtok(NULL,"\n\t\r ");			//read the label
                    printf("\n\t%s\t%s\n",strupr(token),op1);
                    jumptable[noofjumps].location = counter;	//write the jz instruction's location into the jumptable
                    op2=(char*)malloc(sizeof(op1)); 		//allocate space for the label
                    strcpy(op2,op1);				//copy the label into the allocated space
                    jumptable[noofjumps].label=op2;			//point to the label from the jumptable
                    noofjumps++;					//skip to the next empty location in jumptable

                    int n;
                      program[counter]=0xa000;

                    printf("> %d\t%04x\n",counter,program[counter]);
                  	  counter++;
                }
              
				
                else if (strcmp(token,"ret")==0)
                {
                    printf("\n\t%s\n",strupr(token));
                    program[counter]=0xb000; // 7 opcode of ALU operations, 8 ALU code of DEC operation
                    printf("> %d\t%04x\n",counter,program[counter]);
                    counter++;
                }
                else if (strcmp(token,"sti")==0)
                {
                    printf("\n\t%s\n",strupr(token));
                    program[counter]=0xc000; // 7 opcode of ALU operations, 8 ALU code of DEC operation
                    printf("> %d\t%04x\n",counter,program[counter]);
                    counter++;
                }
                else if (strcmp(token,"cli")==0)
                {
                    printf("\n\t%s\n",strupr(token));
                    program[counter]=0xd000; // 7 opcode of ALU operations, 8 ALU code of DEC operation
                    printf("> %d\t%04x\n",counter,program[counter]);
                    counter++;
                }
                else if (strcmp(token,"iret")==0)
                {
                    printf("\n\t%s\n",strupr(token));
                    program[counter]=0xe000; // 7 opcode of ALU operations, 8 ALU code of DEC operation
                    printf("> %d\t%04x\n",counter,program[counter]);
                    counter++;
                }

				token = strtok(NULL,",\n\t\r ");
			}
		}


//================================= SECOND PASS ==============================
	
                //supply the address fields of the jump and jz instructions from the
                
	int i,j;
		for (i=0; i<noofjumps;i++)                                                                   //for all jump/jz instructions
		{
			j=0;
			while ( strcmp(jumptable[i].label , labeltable[j].label) != 0 )             //if the label for this jump/jz does not match with the
				j++;                                                                // jth label in the labeltable, check the next label..
			program[jumptable[i].location] +=(labeltable[j].location-jumptable[i].location-1)&0x0fff;       //copy the jump address into memory.
		}





               
		rewind(fp);
		while(fgets(line,sizeof line,fp)!= NULL) 
		{
			token=strtok(line,"\n\t\r ");
			if (strcmp(token,".data")==0)
			{
				printf("\nsegment .data\n");
				break;
			}

		}

		int dataarea=0;
 		while(fgets(line,sizeof line,fp)!= NULL)
		{
			token=strtok(line,"\n\t\r ");
			if (strcmp(token,".code")==0)  //go till the .code segment
			{
				printf("\nsegment .code\n");
				break;
			}
			else if (token[strlen(token)-1]==':')
			{
				token[strlen(token)-1]='\0';  
				variabletable[noofvariables].location=counter+dataarea;
				op1=(char*)malloc(sizeof(token));
				strcpy(op1,token);
				variabletable[noofvariables].name=op1;
				token = strtok(NULL,",\n\t\r ");
				if (token==NULL)
					program[counter+dataarea]=0;
				else if (strcmp(token, ".space")==0)
				{
					token=strtok(NULL,"\n\t\r ");
					dataarea+=atoi(token);
				}
				else if((token[0]=='0')&&(token[1]=='x'))
					program[counter+dataarea]=(int)strtol(token, NULL, 0)&0xffff;
				else if ((  (token[0])=='-') || ('0'<=(token[0])&&(token[0]<='9'))  )
					program[counter+dataarea]=atoi(token)&0xffff;
				noofvariables++;
				dataarea++;
			}
		}

// supply the address fields for the ldi instructions from the variable table
		for( i=0; i<noofldis;i++)
		{
			j=0;
			while ((j<noofvariables)&&( strcmp( lditable[i].name , variabletable[j].name)!=0 ))
				j++;
			if (j<noofvariables)
				program[lditable[i].location] = variabletable[j].location;
		}

// supply the address fields for the ldi instructions from the label table
		for( i=0; i<noofldis;i++)
		{
			j=0;
			while ((j<nooflabels)&&( strcmp( lditable[i].name , labeltable[j].label)!=0 ))
				j++;
			if (j<nooflabels){
				program[lditable[i].location] = (labeltable[j].location)&0x0fff;
				printf("%d %d %d\n", i, j, (labeltable[j].location));
			}
		}

//display the resulting tables

		printf("LABEL TABLE\n");
		for (i=0;i<nooflabels;i++)
			printf("%d %s\n", labeltable[i].location, labeltable[i].label);
		printf("\n");
		printf("JUMP TABLE\n");
		for (i=0;i<noofjumps;i++)
			printf("%d %s\n", jumptable[i].location, jumptable[i].label);
		printf("\n");
		printf("VARIABLE TABLE\n");
		for (i=0;i<noofvariables;i++)
			printf("%d %s\n", variabletable[i].location, variabletable[i].name);
		printf("\n");
		printf("LDI INSTRUCTIONS\n");
		for (i=0;i<noofldis;i++)
			printf("%d %s\n", lditable[i].location, lditable[i].name);
		printf("\n");
		fclose(fp);
		FILE* fp2 = fopen("RAM.txt","w");
		fprintf(fp2,"v2.0 raw\n");
		for (i=0;i<counter+dataarea;i++) {
			fprintf(fp2,"%04x\n",program[i]);
		}
		fclose(fp2);
		for (i=0;i<counter+dataarea;i++) {
			printf("%04x\n",program[i]);
		}
	}

char *decimal_to_binary(int n)
{
   int c, d, count;
   char *pointer;
   
   count = 0;
   pointer = (char*)malloc(2+1);
   
   if (pointer == NULL)
      exit(EXIT_FAILURE);
     
   for (c = 2 ; c >= 0 ; c--)
   {
      d = n >> c;
     
      if (d & 1)
         *(pointer+count) = 1 + '0';
      else
         *(pointer+count) = 0 + '0';
     
      count++;
   }
   *(pointer+count) = '\0';
   
   return  pointer;
}
int bin_to_hex(int binaryval)
{
    int hexadecimalval = 0, i = 1, remainder;
 
    while (binaryval != 0)
    {
        remainder = binaryval % 10;
        hexadecimalval = hexadecimalval + remainder * i;
        i = i * 2;
        binaryval = binaryval / 10;
    }
    
    return hexadecimalval;
}

 
   
