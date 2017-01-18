#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/types.h>
#include<mqueue.h>
#include<string.h>

#define MSGSZ 100

typedef struct buffer
{
	long msg_type ;
	char str[100] ;
	int count ;
}BUFFER ;

int main()
{	
	key_t key;
	int msgflag;
	int msqid;
	char msg_cmd[MSGSZ + 6];
	FILE *fp;

	BUFFER msg_details;	

	msgflag = IPC_CREAT | IPC_EXCL | 0644;

generate_key:
	printf("Enter the key to generate the message queue: ");
	fflush(stdin);
	scanf(" %d", &key );
	
	msqid = msgget( key, msgflag );
	if( msqid < 0 )
	{
		printf("MESSAGE QUEUE NOT CREATED\n");
		goto generate_key;
	}
	else
		printf("MESSAGE QUEUE CREATED :) \n");

	msg_details.msg_type = 1;
	msgflag = 0;

	while( 1 )
	{
	msgrcv( msqid, &msg_details, sizeof( BUFFER ), msg_details.msg_type, msgflag);
	strcpy( msg_cmd, "wc -w " );
	strcat( msg_cmd, msg_details.str );

	fp = popen( msg_cmd, "r" );
	fscanf( fp, "%d", &msg_details.count );


	msgsnd( msqid, &msg_details, sizeof( BUFFER ), msgflag );

	}
	return 0;
}
