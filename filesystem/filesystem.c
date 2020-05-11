
/*
 *
 * Operating System Design / Diseño de Sistemas Operativos
 * (c) ARCOS.INF.UC3M.ES
 *
 * @file 	filesystem.c
 * @brief 	Implementation of the core file system funcionalities and auxiliary functions.
 * @date	Last revision 01/04/2020
 *
 */


#include "filesystem/filesystem.h" // Headers for the core functionality
#include "filesystem/auxiliary.h"  // Headers for auxiliary functions
#include "filesystem/metadata.h"   // Type and structure declaration of the file system

//definiciones para los estados de los ficheros
#define CLOSED 0
#define OPEN 1

#define LINK 1
#define FILE 0
// el número del bloque inicial
#define INIT_BLOCK 1

// definir estructuras iniciales del sistema de archivos
struct SuperBlock superblock;
int masterDir; // cantidad de ficheros o directorios en master
int fileState[MAX_FILES];	// vector que define el estado (abierto o cerrado) de los ficheros
char iNodeNames[MAX_FILES][MAX_NAME_LENGHT];	// vector con los nombres de ficheros.





/*
 * @brief 	Generates the proper file system structure in a storage device, as designed by the student.
 * @return 	0 if success, -1 otherwise.
 */
int mkFS(long deviceSize)
{	
	//Requisito NF6
	if( deviceSize < ( 460 * 1024 ) || deviceSize > (600*1024) ){
		printf("Error. Device size not suitable for file system.\n");
		return -1;
	}
	
	// inicializar parámetros del sistema de archivo reflejados en el superbloque
	superblock.disk_size = deviceSize;
	superblock.magic_number = 38346; //Identifica este sistema de archivos de cara a un sistema de montado. En este caso, es parte del NIA de uno de los autores.
	memset(superblock.block_allocation_map, 0, sizeof(superblock.block_allocation_map));
	
	
	//inicializar campos
	for(int i = 0; i < MAX_FILES; i++) {
		strcpy(iNodeNames[i], "");
		//ya que sólo hay 240 bloques válidos, se le asigna un valor out of bounds por defecto.
		superblock.inodes[i].block_numbers[0]=255;
		superblock.inodes[i].block_numbers[1]=255;
		superblock.inodes[i].block_numbers[2]=255;
		superblock.inodes[i].block_numbers[3]=255;
		superblock.inodes[i].block_numbers[4]=255;
		fileState[i] = CLOSED;
		superblock.inodes[i].name = iNodeNames[i];
		superblock.inodes[i].crc32=(uint32_t)0;
	}
	//finalmente se desmonta el sistema.
	return unmountFS();
}

/*
 * @brief 	Mounts a file system in the simulated device.
 * @return 	0 if success, -1 otherwise.
 */
int mountFS(void)
{
	// leer el bloque que contiene los metadatos. La expresión (char *)&(superblock) es una dirección de memoria que apunta al principio del superbloque
	if (bread(DEVICE_IMAGE, 0, ((char *)&(superblock))) == -1){
		printf("bread operation error on mount\n");
		return -1;
	}
	

	return 0;
	
}

/*
 * @brief 	Unmounts the file system from the simulated device.
 * @return 	0 if success, -1 otherwise.
 */
int unmountFS(void)
{

	// escribir el bloque que contiene los metadatos. La expresión (char *)&(superblock) es una dirección de memoria que apunta al principio del superbloque
	if (bwrite(DEVICE_IMAGE, 0, ((char *)&(superblock))) == -1){
		printf("bwrite operation error on unmount\n");
		return -1;
	}
	

	return 0;

	
}

int getFreeBlock(void)
{
	for(int i = 0; i< (MAX_FILES*(MAX_FILE_SIZE/BLOCK_SIZE)); i++ ){

		if(superblock.block_allocation_map[i]==0){
			superblock.block_allocation_map[i]=1;
			return i;
		}

	}

	return -1;
}


/*
 * @brief	Creates a new file, provided it it doesn't exist in the file system.
 * @return	0 if success, -1 if the file already exists, -2 in case of error.
 */
int createFile(char *fileName)
{
	int error_code=checkFile(fileName);
	if(error_code==-1){
		printf("Chosen name exceeds maximum length\n");
		return -2;
	}
	
	if(error_code==-2){
		printf("File already exists\n");
		return -1;
	}

	if(error_code==-3){
		printf("file name can't be empty\n");
		return -2;
	}
	
	for(int i=0; i<=MAX_FILES; i++){
		if(strcmp(iNodeNames[i], "")==0){
			strcpy(iNodeNames[i], fileName);
			superblock.inodes[i].name=iNodeNames[i];
			int blocknum = getFreeBlock();
			if(blocknum==-1){
				printf("error allocating block\n");
				return -2;
			}
			superblock.inodes[i].block_numbers[0]=blocknum;
			printf("File %s created with file descriptor %i\n", fileName, i);
			return 0;
		}
	}
	
	printf("File could not be created as it would exceed the maximum number of files in the system. Please delete some files before proceeding.\n");
	return -1;

}

/*
 * @brief	Deletes a file, provided it exists in the file system.
 * @return	0 if success, -1 if the file does not exist, -2 in case of error..
 */
int removeFile(char *fileName)
{

	if(strcmp(fileName, "")==0){
		printf("Please enter a valid file name\n");
		return -2;
	}

	for(int i = 0; i<MAX_FILES;i++){
		if(strcmp(iNodeNames[i], fileName)==0 && fileState[i]==OPEN){

			printf("Please close the file before attempting removal\n");
			return -1;

		}

		if(strcmp(iNodeNames[i], fileName)==0){
			strcpy(iNodeNames[i], "");
			superblock.inodes[i].block_numbers[0]=255;
			superblock.inodes[i].block_numbers[1]=255;
			superblock.inodes[i].block_numbers[2]=255;
			superblock.inodes[i].block_numbers[3]=255;
			superblock.inodes[i].block_numbers[4]=255;
			superblock.inodes[i].size=0;
			superblock.inodes[i].pointer=0;
			return 0;
		}

	}
	printf("File does not exist in the system\n");
	return -1;
}

/*
 * @brief	Opens an existing file.
 * @return	The file descriptor if possible, -1 if file does not exist, -2 in case of error..
 */
int openFile(char *fileName)
{	
	
	int fd=-1;
	for(int i=0; i< MAX_FILES; i++){
		if(strcmp(iNodeNames[i], fileName)==0){
			fd=i;
		}
	}

	if(fd==-1){
		printf("File does not exist in the system.\n");
		return -1;
	}

	if(superblock.inodes[fd].type==LINK){
		fd=superblock.inodes[fd].fd;
	}

	if(fileState[fd]==OPEN){

		printf("File is already open \n");
		return -2;
	}
	fileState[fd]=OPEN;
	
	if(lseekFile(fd, 0, FS_SEEK_BEGIN)==-1){
		printf("error upon setting pointer.\n");
		return -2;
	}

	return fd;
}

/*
 * @brief	Closes a file.
 * @return	0 if success, -1 otherwise.
 */
int closeFile(int fileDescriptor)
{
	if (fileDescriptor < 0 || fileDescriptor > MAX_FILES-1){
		printf("Invalid fd\n");
		return -1;
	}

	if(fileState[fileDescriptor]==CLOSED){
		printf("File not open\n");
		return -1;
	}

	if(superblock.inodes[fileDescriptor].openint==1){
		printf("file open with integrity; must be closed with integrity as well");
		return -1;
	}

	fileState[fileDescriptor]=CLOSED;
	unmountFS();
	return 0;
}

/*
 * @brief	Reads a number of bytes from a file and stores them in a buffer.
 * @return	Number of bytes properly read, -1 in case of error.
 */
int readFile(int fileDescriptor, void *buffer, int numBytes)
{
	if(fileState[fileDescriptor]==CLOSED){
		printf("File is not open\n");
		return -1;
		}
	if(superblock.inodes[fileDescriptor].pointer+numBytes>superblock.inodes[fileDescriptor].size){
			numBytes=MAX_FILE_SIZE-superblock.inodes[fileDescriptor].pointer;
		}
	int ptr=superblock.inodes[fileDescriptor].pointer;
	int initial_blocknum= (int)(ptr/2048);
	int final_blocknum = (int)((ptr+numBytes)/2048);
	int blockoffset= (int)(ptr%2048);
	int otheroffset=(2048*((final_blocknum-initial_blocknum)+1))-(blockoffset+numBytes);
	int size=blockoffset+numBytes+otheroffset;
	char truebuffer [blockoffset+numBytes+otheroffset];
	void * address=truebuffer;
	
	for(int i=size; i>0; i-=2048){
		if(bread("disk.dat", INIT_BLOCK+superblock.inodes[fileDescriptor].block_numbers[initial_blocknum], address)==-1){
			printf("bread error on readFile\n");
			return -1;
		}
		address+=2048;
		initial_blocknum++;
	}
	address=truebuffer+blockoffset;
	memcpy(buffer, address, numBytes);
	if(lseekFile(fileDescriptor, numBytes, FS_SEEK_CUR)==-1){
		printf("error setting pointer\n");
		return -1;
	}
	return numBytes;
}

/*
 * @brief	Writes a number of bytes from a buffer and into a file.
 * @return	Number of bytes properly written, -1 in case of error.
 */
int writeFile(int fileDescriptor, void *buffer, int numBytes)
{			
	

	if(fileState[fileDescriptor]==CLOSED){
	printf("File is not open\n");
	return -1;
	}

	if(superblock.inodes[fileDescriptor].pointer+numBytes>MAX_FILE_SIZE){
		numBytes=MAX_FILE_SIZE-superblock.inodes[fileDescriptor].pointer;
	}

	int ptr=superblock.inodes[fileDescriptor].pointer;
	int initial_blocknum= (int)(ptr/2048);
	int final_blocknum = (int)((ptr+numBytes)/2048);
	if(superblock.inodes[fileDescriptor].block_numbers[initial_blocknum-1]==255){
				superblock.inodes[fileDescriptor].block_numbers[initial_blocknum-1]=getFreeBlock();
	}
	int blockoffset= (int)(ptr%2048);
	int otheroffset=(2048*((final_blocknum-initial_blocknum)+1))-(blockoffset+numBytes);

	char buffer1[2048];
	if(bread("disk.dat", INIT_BLOCK+superblock.inodes[fileDescriptor].block_numbers[initial_blocknum-1], buffer1)==-1){
			
		return -1;
	}

	char buffer2[2048];
	if(bread("disk.dat", INIT_BLOCK+superblock.inodes[fileDescriptor].block_numbers[final_blocknum-1], buffer2)==-1){
			
		return -1;
	}

	//////////////
	char truebuffer [blockoffset+numBytes+otheroffset];
	int size=blockoffset+numBytes+otheroffset;
	memcpy(truebuffer, buffer1,blockoffset);
	void *addr=truebuffer+blockoffset;
	memcpy(addr, buffer, numBytes);
	addr+=numBytes;
	void * buffer2off=buffer2+(2048-otheroffset);
	memcpy(addr, buffer2off, otheroffset);
	int j=initial_blocknum;


	if(size<=BLOCK_SIZE){
		if(bwrite("disk.dat", INIT_BLOCK+superblock.inodes[fileDescriptor].block_numbers[initial_blocknum], truebuffer)==-1){
			printf("bwrite error in writeFile\n");
			return -1;
		}
	}
	
	else{
		void * address=truebuffer;
		for(int i=size; i>0;i-=2048){

			if(i>=2048){

				if(bwrite("disk.dat",INIT_BLOCK+superblock.inodes[fileDescriptor].block_numbers[initial_blocknum], address)==-1){
					printf("bwrite error in writeFile\n");
					return -1;
				}
			initial_blocknum++;
			if(superblock.inodes[fileDescriptor].block_numbers[initial_blocknum]==255){
				superblock.inodes[fileDescriptor].block_numbers[initial_blocknum]=getFreeBlock();
			}

			address+=2048;
			}

			else{

				if(bwrite("disk.dat",INIT_BLOCK+superblock.inodes[fileDescriptor].block_numbers[initial_blocknum], address)==-1){
					printf("bwrite error in writeFile\n");
					return -1;
				}			

			}

		}
		

	}
	if(lseekFile(fileDescriptor, numBytes, FS_SEEK_CUR)==-1){
		printf("error setting pointer\n");
		return -1;
	}

	for(int i=0; i<j;i++){
		if(superblock.inodes[fileDescriptor].block_numbers[i]==255){
			superblock.inodes[fileDescriptor].block_numbers[i]=getFreeBlock();
		}
	}

	if(superblock.inodes[fileDescriptor].pointer>superblock.inodes[fileDescriptor].size){
		superblock.inodes[fileDescriptor].size+=superblock.inodes[fileDescriptor].pointer+superblock.inodes[fileDescriptor].size;

	}

	return numBytes;
}

/*
 * @brief	Modifies the position of the seek pointer of a file.
 * @return	0 if succes, -1 otherwise.
 */
int lseekFile(int fileDescriptor, long offset, int whence)
{
	if (fileDescriptor < 0 || fileDescriptor > MAX_FILES-1){
		printf("Invalid fd\n");
		return -1;
	}

	if (strcmp(superblock.inodes[fileDescriptor].name,"")==0){
		printf("chosen file does not exist\n");
		return -1;
	}

	if(whence==FS_SEEK_CUR &&( (superblock.inodes[fileDescriptor].pointer + offset)<0 || (superblock.inodes[fileDescriptor].pointer+offset)> MAX_FILE_SIZE))
	{
		printf("Position out of bounds\n");
		return -1;
	}

	switch(whence){
		//Si se desea cambiar el puntero desde la posición anterior, se añade el número de bytes que se desee moveer
		case FS_SEEK_CUR:
			superblock.inodes[fileDescriptor].pointer += offset;
			break;
		//Si se desea cambiar a la posición inicial, se pone a 0.
		case FS_SEEK_BEGIN:
			superblock.inodes[fileDescriptor].pointer = 0;
			break;
		//Si se desea cambiar a la posición final, se pone al tamaño del bloque-1.
		case FS_SEEK_END:
			superblock.inodes[fileDescriptor].pointer = superblock.inodes[fileDescriptor].size-1;
			break;
	}

	return 0;
}

/*
 * @brief	Checks the integrity of the file.
 * @return	0 if success, -1 if the file is corrupted, -2 in case of error.
 */

int checkFile (char * fileName)
{	
	
	if(strlen(fileName)>MAX_NAME_LENGHT){
		return -1;
	}

	for(int i=0; i<MAX_FILES;i++){
		if(strcmp(superblock.inodes[i].name, "")!=0 && strcmp(superblock.inodes[i].name, fileName)==0){
			return -2;
		}
	}

	if(strcmp(fileName, "")==0){
		return -3;
	}



	return 0;
    
}

/*
 * @brief	Include integrity on a file.
 * @return	0 if success, -1 if the file does not exists, -2 in case of error.
 */

int includeIntegrity (char * fileName)
{
	int fd=openFile(fileName);
	if (fd<0){
		return fd;
	}
	if(superblock.inodes[fd].size==0){
		printf("can't add integrity to an empty file\n");
		closeFile(fd);
		return -2;
	}
	if(superblock.inodes[fd].crc32!=0){
		printf("File already has integrity\n");
		return -2;
	}
	unsigned char buffer [superblock.inodes[fd].size];

	if(readFile(fd, buffer, superblock.inodes[fd].size)<0){
		return -2;
	}
	closeFile(fd);
	superblock.inodes[fd].crc32= CRC32(buffer, sizeof(buffer));
	
	
    return 0;
}

/*
 * @brief	Opens an existing file and checks its integrity
 * @return	The file descriptor if possible, -1 if file does not exist, -2 if the file is corrupted, -3 in case of error
 */
int openFileIntegrity(char *fileName)
{	

	int fd=openFile(fileName);
	if(fd<0){
		return fd;
	}
	if(superblock.inodes[fd].crc32==0){
		printf("File integrity has not been included\n");
		fileState[fd]=CLOSED;
		return -3;
	}
	
	
	unsigned char buffer [superblock.inodes[fd].size];

	if(readFile(fd, buffer, superblock.inodes[fd].size)<0){
		return -3;
	}
	closeFile(fd);

	uint32_t comp= CRC32(buffer, sizeof(buffer));


	if(comp!= superblock.inodes[fd].crc32){
		printf("file is corrupted\n");
		return -2;
	}
	if(lseekFile(fd, 0, FS_SEEK_BEGIN)==-1){
		printf("error upon setting pointer.\n");
		return -2;
	}
	fileState[fd]=OPEN;
	superblock.inodes[fd].openint=1;
	return fd;
}

/*
 * @brief	Closes a file and updates its integrity.
 * @return	0 if success, -1 otherwise.
 */
int closeFileIntegrity(int fileDescriptor)
{	
	if(fileState[fileDescriptor]==CLOSED){
		printf("file is not open\n");
		return -2;
	}
	if(superblock.inodes[fileDescriptor].openint==0){
		printf("file has been opened without integrity; must be closed without it.\n");
		return -2;
	}
	else{
		superblock.inodes[fileDescriptor].openint=0;
	}
	if(lseekFile(fileDescriptor, 0, FS_SEEK_BEGIN)==-1){
		printf("error upon setting pointer.\n");
		return -1;
	}

	unsigned char buffer [superblock.inodes[fileDescriptor].size];

	if(readFile(fileDescriptor, buffer, superblock.inodes[fileDescriptor].size)<0){
		return -1;
	}
	int ret=closeFile(fileDescriptor);
	if(ret<0){
		return ret;
	}

	uint32_t comp= CRC32(buffer, sizeof(buffer));
	superblock.inodes[fileDescriptor].crc32=comp;
	return 0;
    
}

/*
 * @brief	Creates a symbolic link to an existing file in the file system.
 * @return	0 if success, -1 if file does not exist, -2 in case of error.
 */
int createLn(char *fileName, char *linkName)
{	
	int fd=openFile(fileName);
	if(fd<0){
		printf("error opening file");
	}
	closeFile(fd);
	int error_code=checkFile(linkName);
	if(error_code==-1){
		printf("Chosen name exceeds maximum length\n");
		return -2;
	}
	
	if(error_code==-2){
		printf("File already exists\n");
		return -1;
	}

	if(error_code==-3){
		printf("file name can't be empty\n");
		return -2;
	}

	for(int i=0; i<MAX_FILES; i++){
		if(strcmp(iNodeNames[i], "")==0){
			strcpy(iNodeNames[i], linkName);
			superblock.inodes[i].name=iNodeNames[i];
			superblock.inodes[i].type=LINK;
			superblock.inodes[i].fd=fd;
		}
	}

    return 0;
}

/*
 * @brief 	Deletes an existing symbolic link
 * @return 	0 if the file is correct, -1 if the symbolic link does not exist, -2 in case of error.
 */
int removeLn(char *linkName)
{	
	if(strcmp(linkName, "")==0){
		printf("Please enter a valid link name\n");
		return -2;
	}

	for(int i = 0; i<MAX_FILES;i++){

		if(strcmp(iNodeNames[i], linkName)==0){
			strcpy(iNodeNames[i], "");
			superblock.inodes[i].block_numbers[0]=255;
			superblock.inodes[i].block_numbers[1]=255;
			superblock.inodes[i].block_numbers[2]=255;
			superblock.inodes[i].block_numbers[3]=255;
			superblock.inodes[i].block_numbers[4]=255;
			superblock.inodes[i].size=0;
			superblock.inodes[i].pointer=0;
			superblock.inodes[i].fd=49;
			superblock.inodes[i].type=FILE;
			return 0;
		}

	}
	printf("File does not exist in the system\n");
	return -1;
}

