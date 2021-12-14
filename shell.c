//Kyle Stearns, Scott and Sean

main()
{
enableInterrupts();
while(1)
{

char line[80];
int i = 0;
char buffer[512];
int sectorsRead=0;
syscall(0,"SHELL>");
syscall(1,line);

if(line[0]=='t'&&line[1]=='y'&&line[2]=='p'&&line[3]=='e')
{
	if(line[5]!='\0')
	{
	char fileName[6];
	for(i = 0; i<6;i++)
		{
		fileName[i]=line[5+i];

		}
	syscall(3,fileName,buffer,&sectorsRead);
	if(sectorsRead==0)
	{
	syscall(0,"File not found!");
	}
	else
	{
	syscall(0,buffer,0,0);
	}
	}

}
else if(line[0]=='e'&&line[1]=='x'&&line[2]=='e'&&line[3]=='c')
{
	if(line[5]!='\0')
        {
        char proName[6];
        for(i = 0; i<6;i++)
                {
                proName[i]=line[5+i];

            	}
		
		syscall(4,proName,0,0);
		
		syscall(0,"No program like that");
	}
}
else
{
syscall(0,"Bad Command");
}
}

}
