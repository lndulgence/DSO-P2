
/*
 *
 * Operating System Design / Diseño de Sistemas Operativos
 * (c) ARCOS.INF.UC3M.ES
 *
 * @file 	test.c
 * @brief 	Implementation of the client test routines.
 * @date	01/03/2017
 *
 */


#include <stdio.h>
#include <string.h>
#include "filesystem/filesystem.h"


// Color definitions for asserts
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_BLUE "\x1b[34m"

#define N_BLOCKS 25					  // Number of blocks in the device
#define DEV_SIZE 460*1024 + 1// Device size, in bytes
//please note that all test must return SUCCESS, even in operations that are supposed to fail. In this case, failure to execute the function is considered a successful test.
int main()
{
	int ret;
	char * a [3200];
	memset(a, 'd', sizeof(a));

	ret=mkFS(460*1024-1);
	if(ret>=0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkFS (invalid parameters, under minimum size) ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkFS (invalid parameters, under minimum size)", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	
	ret=mkFS(600*1024+1);
	if(ret>=0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkFS (invalid parameters, over max size) ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkFS (invalid parameters, over max size)", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	
	
	ret = mkFS(DEV_SIZE);
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	///////

	ret = mountFS();
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mountFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mountFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	///////
	ret=createFile("");
	if (ret != -2)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile (empty name)", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile (empty name) ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	ret=createFile("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	if (ret !=-2)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile (too long name)", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile (too long name) ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	ret = createFile("/test.txt");
	
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	ret = createFile("/test.txt");
	
	if (ret == 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile (where file already exists) ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile (where file already exists) ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);



	ret=writeFile(0, a, 3000);
	if (ret >= 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST writeFile on a closed file ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST writeFile on a closed file", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);



	ret= openFile("/test.txt");
	int fd=ret;

	if(ret!=0){
		printf("error on open");
		return -1;
	}
	else{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	}

	
	lseekFile(fd, 10, FS_SEEK_CUR);
	ret=writeFile(fd, a, 3000);
	if(ret<0){
		printf("error on write");
		return -1;
	}

	else {

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST writeFile ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	}

	lseekFile(fd, 0, FS_SEEK_BEGIN);
	lseekFile(fd, 10, FS_SEEK_CUR);

	char  b [3000];
	memset(b, 'd', sizeof(b));

	char  c [3000];

	
	ret=readFile(fd, c, 3000);
	if(ret<0){
		printf("error on read");
		return -1;
	}

	else {

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST readFile ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	}

	if(sizeof(c)==sizeof(b) && memcmp(c, b, sizeof(c))==0){
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST readFile and writeFile consistency  ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	else{

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST readFile and writeFile consistency ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);

	}

	ret=closeFile(fd);

	if(ret==0){

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST closeFile ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	}

	else{

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST closeFile", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);

	}

	ret=includeIntegrity("/test.txt");

	if(ret==0){

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST includeIntegrity ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	}

	else{

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST includeIntegrity ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);

	}

	ret=openFileIntegrity("/test.txt");
	fd=ret;
	if(ret==0){

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openIntegrity (file has not been modified) ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	}

	else{

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openIntegrity(file has not been modified) ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);

	}

	closeFileIntegrity(fd);
	openFile("/test.txt");

	char  yikes [500];

	memset(yikes, 'b', sizeof(yikes));

	ret=writeFile(fd, yikes, 500);
	if(ret<0){
		return -1;
	}

	closeFile(fd);
	
	ret=openFileIntegrity("/test.txt");
	if(ret==-2){

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openIntegrity (file has been modified, should be corrupted) ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	}
	else{

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openIntegrity(file has not been modified, should be corrupted) ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);

	}

	
	createFile("/test2.txt");
	ret= openFileIntegrity("/test2.txt");
	if(ret<0){

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFileIntegrity (must return negative, as integrity has not been included yet) ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	}

	else{

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFileIntegrity (must return negative, as integrity has not been included yet)  ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);

	}
	ret=includeIntegrity("/test2.txt");
	if(ret<0){

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST includeIntegrity (must return negative, as file is empty) ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	}

	else{

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFileIntegrity (must return negative, as file is empty)  ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);

	}

	fd=openFile("/test2.txt");
	ret=writeFile(fd, yikes, 500);
	if(ret<0){
		return -1;
	}
	
	closeFile(fd);
	includeIntegrity("/test2.txt");
	memset(yikes, 'a', sizeof(yikes));
	fd=openFileIntegrity("/test2.txt");
		ret=writeFile(fd, yikes, 500);
	if(ret<0){
		return -1;
	}
	ret=closeFileIntegrity(fd);

	if(ret==0){

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST closeFileIntegrity ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	}

	else{

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST closeFileIntegrity ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);

	}


	createFile("linktest.txt");
	ret=createLn("linktest.txt", "link");
	if(ret==0){

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createLn ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	}

	else{

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createLn ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);

	}

	ret=openFile("link");
	fd=ret;

	if(ret>=0){

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile on a link ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	}

	else{

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile on a link ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);

	}

	ret=openFile("linktest.txt");
	if(ret<0){

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile on a file whose link has been opened(must return negative, file is open) ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	}

	else{

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile on a file whose link has been opened(must return negative, file is open) ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);

	}

	ret=closeFile(fd);
	if(ret==0){

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST closeFile on a file whose link has been opened", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	}

	else{

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile on a whose link has been opened", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);

	}

	ret=openFile("linktest.txt");
	if(ret>=0){

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile on a file whose link is no longer open ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	}

	else{

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile on a file whose link is no longer open ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);

	}
	fd=openFile("/test.txt");
	ret=removeFile("/test.txt");
		if(ret<0){

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST removeFile on an open file", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	}

	else{

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST removeFile on an open file ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);

	}
	closeFile(fd);
	ret=removeFile("/test.txt");
		if(ret==0){

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST removeFile ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	}

	else{

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST removeFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);

	}

	ret=removeFile("/test.txt");
		if(ret<0){

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST removeFile on an already removed file ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	}

	else{

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST removeFile on an already removed file", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);

	}
	ret=removeLn("link");
		if(ret==0){

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST removeLink ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	}

	else{

		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST removeLink", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);

	}


	//////////////////////////////////////////
	ret = unmountFS();
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST unmountFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST unmountFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	///////

	return 0;
}
