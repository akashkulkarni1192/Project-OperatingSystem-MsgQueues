#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

typedef struct buffer
{
	long msg_type ;
	char str[100] ;
	int count ;
}BUFFER ;

int main ()
{
	int temp ;
	char name[20] ;

	key_t key ;
	int msgid ;
	size_t length ;
	BUFFER message ;
	
	printf ( "Enter the key to access message queue \n" ) ;
	scanf ( "%d",&key ) ;
	
	msgid = msgget ( key, IPC_CREAT | 0644 ) ;

	if ( msgid < 0 )
	{
		fprintf ( stderr, "error: Error in generating the msgid\n" ) ;
		exit (1) ;
	}
	else
		printf ( "The generated msgid is %d \n\n", msgid ) ;
	
	printf ( "Enter the filename to search for: \n" ) ;
	fflush(stdin) ;
	scanf ( " %[^\n]",name ) ;
	
//	printf ( "The entered file_name is \n%s\n",name ) ;
	
	message.msg_type = 1 ;
	strcpy ( message.str, name ) ;
	length = sizeof(struct buffer);
	temp = msgsnd ( msgid, &message, length, 0 ) ;
	
	if ( temp < 0 )
	{
		fprintf ( stderr, "error: Error in sending the message \n" ) ;
		exit (1) ;
	}
	else
		printf ( "\nThe filename %s has been sent successufully \n\n", name ) ;

	temp = msgrcv ( msgid, &message, length, 1, 0 ) ;
	if ( temp < 0 )
	{
		fprintf ( stderr, "error: Error in recieving the count \n" ) ;
		exit (1) ;
	}
	else
		printf ( "The output has been recieved as: \n" ) ;
	
	if ( message.count < 0 )
		fprintf ( stderr, "error: cannot access %s: No such file or directory\n", message.str ) ;
	else
		printf ( "%d %s\n", message.count, message.str ) ;
	
	return 0 ;
}
