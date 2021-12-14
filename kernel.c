//Kyle Stearns,Scott Belliveau and Shawn Lovell
void printString(char*);
void printChar(char);
void readString(char*);
void readSector(char*,int);
void handleInterrupt21(int,int,int,int);
void readFile(char*, char*,int*);
void executeProgram(char*);
void terminate();
void handleTimerInterrupt(int,int);
int processActive[8];
int processStackPointer[8];
int currentProcess;
main()
{
	int i = 0;
	
	
		
	makeInterrupt21();
	for(i=0;i<8;i++)
	{
		processActive[i]=0;
		processStackPointer[i]=0xff00;
	}
	currentProcess= -1;
//	makeTimerInterrupt();
	interrupt(0x21,4,"shell",0,0);
	makeTimerInterrupt();	
	while(1)
	{
	}
}
void printString(char* string)
{
	
	int i=0;
	while(string[i]!='\0')
	{
	interrupt(0x10,0xe*256+string[i],0,0,0);
	i=i+1;
	}
	
}
void printChar(char c)
{

	interrupt(0x10,0xe*256+c,0,0,0);
}
void readString(char* line)
{
	int i=0;
	int input=0;
	
	while(input!=0xd)
	
	{
	input=interrupt(0x16,0*256+line[i],0,0,0);
	
	if (input==0x8 && i>0)
	{
	
	i=i-1;
	printChar(input);
	printChar(' ');
	printChar(input);
	}
	else if(input!=0xd)
	{
	line[i]=input;
	printChar(line[i]);
	i=i+1;
	}
}
	line[i+1]=0xa;
	line[i+2]=0x0;
	printChar(line[i]);
	printChar(line[i+1]);
}
void readSector(char* buffer, int sector)
{
	interrupt(0x13,2*256+1,buffer,0*256+sector+1,0*256+0x80);
}

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
	if(ax==0)
	{
	printString(bx);
	}
	else if(ax==1)
	{
	readString(bx);
	}
	else if(ax==2)
	{
	readSector(bx,cx);
	}
	else if(ax==3)
	{
	readFile(bx,cx,dx);
	}
	else if(ax==4)
	{
	executeProgram(bx,0,0);
	}
	else if(ax==5)
	{
	terminate(0,0,0);
	}
	else
	{
	printString("Sorry NOPE");
	}


}
void readFile(char* fileName, char* buffer, int* sectorsReadPtr)
{

     char dir[512];
     int s;
     int sector = 0;
     int check = 1;
     int fileEntry = 0;
     int ltr = 0;
	readSector(dir,2);
	for(fileEntry = 0;fileEntry < 512; fileEntry+=32)
	{
		check=1;

		for(ltr=0;ltr<6;ltr++)
		{
			if(fileName[ltr]!=dir[ltr+fileEntry])
			{
				check = 0;
				*sectorsReadPtr = 0;
			
				break;
			}
		}
	
		if(check==1)
		{
		break;
		}
	}
	
//	
	if(check==1)
	{
		for(s=6;s<26;s++)
		{
		sector =dir[fileEntry+s];
		
			if(sector==0)
			{
			
			break;

			}


		readSector(buffer,sector);

		buffer =buffer + 512;
		*sectorsReadPtr = *sectorsReadPtr+1;

		}


	}

}
void executeProgram(char* name){

char buffer[13312];
int i = 0;
int entryNum=0;
int segNum;
int sectorsRead=0;
int dataseg=0;
readFile(name,buffer,&sectorsRead);
dataseg = setKernelDataSegment();
while(processActive[entryNum]!=0)

{
entryNum++;
}
restoreDataSegment(dataseg);
segNum = (entryNum+2)*0x1000;
for(i=0;i<13122;i++)
        {
        putInMemory(segNum,i,buffer[i]);
        //printChar('g');
        }




initializeProgram(segNum);
dataseg = setKernelDataSegment();
processStackPointer[entryNum]=0xff00;
restoreDataSegment(dataseg);
dataseg = setKernelDataSegment();
processActive[entryNum]=1;
restoreDataSegment(dataseg);
//printChar('b');
//printChar('a');
//


}
void terminate()
{
setKernelDataSegment();
processActive[currentProcess]=0;
restoreDataSegment();

while(1);
}
void handleTimerInterrupt(int segment, int sp)
{
//printChar('t');
//printChar('i');
//printChar('c');
int dataseg;
dataseg = setKernelDataSegment();
if(currentProcess!=-1)
{
processStackPointer[currentProcess]=sp;
printChar('b');
}
currentProcess++;
if(currentProcess==8)
{
currentProcess=0;
}
while(processActive[currentProcess]!=1){
currentProcess++;
	if(currentProcess==8)
	{
	//printChar('s');
	currentProcess=0;
//	printChar('q');
	}
}
segment = (currentProcess+2)*0x1000;
sp = processStackPointer[currentProcess];
restoreDataSegment(dataseg);
returnFromTimer(segment,sp);

}
