#include<stdio.h>
#include<syslog.h>
int main(int argc, char *argv[])
{
	openlog(NULL,0,LOG_USER);
	char *writefile=argv[1];
	char *writestr=argv[2];
	FILE *out_file;
	if ( argc < 3 )
	{
		syslog(LOG_ERR,"Invalid Number of arguments");
		return 1;
	}
	out_file=fopen(writefile,"w");
	if (out_file == NULL)
	{
		syslog(LOG_ERR, "Unable to open file");
		return 1;
	}
	else
	{
		fprintf(out_file, "%s", writestr);
		syslog(LOG_DEBUG, "Writing %s to %s", writestr, writefile);
		return 0;
	}
}

