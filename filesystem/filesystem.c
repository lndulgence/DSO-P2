
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


//definición del bloque inicial

// el número del bloque inicial
#define INIT_BLOCK 1

// definir estructuras iniciales del sistema de archivos
struct SuperBlock superblock;
int masterDir; // cantidad de ficheros o directorios en master
int fileState[MAX_NUMBER_FILES];	// vector que define el estado (abierto o cerrado) de los ficheros
char iNodeNames[MAX_NUMBER_FILES][MAX_NAME_LENGHT];	// vector con los nombres de ficheros.
int levels[MAX_FILE_SIZE]; // el nivel que se encuentra cada fichero: 4 niveles en total




/*
 * @brief 	Generates the proper file system structure in a storage device, as designed by the student.
 * @return 	0 if success, -1 otherwise.
 */
int mkFS(long deviceSize)
{
	//Requisito NF6
	if( deviceSize < ( 460 * 1024 ) || deviceSize > (600*1024) ){
		printf("Error. Device size not suitable for file system.");
		return -1
	}
	
	// inicializar parámetros del sistema de archivo reflejados en el superbloque
	superblock.disk_size = deviceSize;
	superblock.magic_number = 38346; //Identifica este sistema de archivos de cara a un sistema de montado. En este caso, es parte del NIA de uno de los autores.
	superblock.inodeMap = 0;
	memset(superblock.block_allocation_map, 0, sizeof(superblock.block_allocation_map));
	
	
	//inicializar campos
	for(int i = 0; i < MAX_NUMBER_FILES; i++) {
		strcpy(iNodeNames[i], "");
		//ya que sólo hay 240 bloques válidos, se le asigna un valor out of bounds por defecto.
		superblock.inodes[i].block_numbers[0]=255;
		superblock.inodes[i].block_numbers[1]=255;
		superblock.inodes[i].block_numbers[2]=255;
		superblock.inodes[i].block_numbers[3]=255;
		superblock.inodes[i].block_numbers[4]=255;
		fileState[i] = CLOSED;
		superblock.inodes[i].name = iNodeNames[i];
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
	if (bwrite(DEVICE_IMAGE, 0, ((char *)&(sb))) == -1){
		printf("bwrite operation error on unmount\n");
		return -1;
	}

	return 0;

	
}

/*
 * @brief	Creates a new file, provided it it doesn't exist in the file system.
 * @return	0 if success, -1 if the file already exists, -2 in case of error.
 */
int createFile(char *fileName)
{
	error_code=checkFile(fileName;)
	if(error_code==-1){
		printf("Chosen name exceeds maximum length\n");
		return -2
	}
	
	if(error_code==-2){
		printf("File already exists\n");
		return -1;
	}

	if(error_code==-3){
		printf("file name can't be empty\n");
		return -2
	}
	
	for(int i=0; i<=MAX_NUMBER_FILES; i++){
		if(strcmp(iNodeNames[i], ""==0){
			strcpy(inodeNames[i], fileName);
			superblock.inodes[i].name=fileName;
			int blocknum=getFreeBlock();
			if(blocknum==-1){
				printf("error allocating block");
				return -2;
			}
			superblock.inodes[i].block_numbers[0]=blocknum;
			printf("File %s created with file descriptor %i", fileName, i);
			return 0;
		}
	}
	printf("File could not be created as it would exceed the maximum number of files in the system. Please delete some files before proceeding.");
	return -1;

}

/*
 * @brief	Deletes a file, provided it exists in the file system.
 * @return	0 if success, -1 if the file does not exist, -2 in case of error..
 */
int removeFile(char *fileName)
{
	return -2;
}

/*
 * @brief	Opens an existing file.
 * @return	The file descriptor if possible, -1 if file does not exist, -2 in case of error..
 */
int openFile(char *fileName)
{
	return -2;
}

/*
 * @brief	Closes a file.
 * @return	0 if success, -1 otherwise.
 */
int closeFile(int fileDescriptor)
{
	return -1;
}

/*
 * @brief	Reads a number of bytes from a file and stores them in a buffer.
 * @return	Number of bytes properly read, -1 in case of error.
 */
int readFile(int fileDescriptor, void *buffer, int numBytes)
{
	return -1;
}

/*
 * @brief	Writes a number of bytes from a buffer and into a file.
 * @return	Number of bytes properly written, -1 in case of error.
 */
int writeFile(int fileDescriptor, void *buffer, int numBytes)
{
	return -1;
}

/*
 * @brief	Modifies the position of the seek pointer of a file.
 * @return	0 if succes, -1 otherwise.
 */
int lseekFile(int fileDescriptor, long offset, int whence)
{
	return -1;
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
	for(int i=0; i<MAX_NUMBER_FILES; i++;){
		if (strcmp(superblock.inodes[i].name, fileName)==0){
			return -2;
		}
	}

	if(strcmp(fileName, "")==0){
		return -3;
	}

	return 0
    
}

/*
 * @brief	Include integrity on a file.
 * @return	0 if success, -1 if the file does not exists, -2 in case of error.
 */

int includeIntegrity (char * fileName)
{
    return -2;
}

/*
 * @brief	Opens an existing file and checks its integrity
 * @return	The file descriptor if possible, -1 if file does not exist, -2 if the file is corrupted, -3 in case of error
 */
int openFileIntegrity(char *fileName)
{

    return -2;
}

/*
 * @brief	Closes a file and updates its integrity.
 * @return	0 if success, -1 otherwise.
 */
int closeFileIntegrity(int fileDescriptor)
{
    return -1;
}

/*
 * @brief	Creates a symbolic link to an existing file in the file system.
 * @return	0 if success, -1 if file does not exist, -2 in case of error.
 */
int createLn(char *fileName, char *linkName)
{
    return -1;
}

/*
 * @brief 	Deletes an existing symbolic link
 * @return 	0 if the file is correct, -1 if the symbolic link does not exist, -2 in case of error.
 */
int removeLn(char *linkName)
{
    return -2;
}

int getFreeBlock()
{
	for(int i = 0; i< (MAX_FILES*(MAX_FILE_SIZE/BLOCK_SIZE)); i++ ){

		if(superblock.block_allocation_map[i]==0){
			superblock.block_allocation_map[i]=1;
			return i;
		}

	}

	return -1;
}
