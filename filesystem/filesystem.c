
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

//definiciones para los tipos de i-nodos
#define FILE 0
#define DIR 1

//definición del bloque inicial

// el número del bloque inicial
#define INIT_BLOCK 1

// definir estructuras iniciales del sistema de archivos
struct SuperBlock superblock;
int masterDir; // cantidad de ficheros o directorios en master
int fileState[MAX_NUMBER_FILES];	// vector que define el estado (abierto o cerrado) de los ficheros
char iNodeNames[MAX_NUMBER_FILES][MAX_NAME_LENGHT];	// vector con los nombres de ficheros y directorios
char preDir[MAX_NUMBER_FILES][MAX_NAME_LENGHT]; // array para guardar los nombres de los directorios predecesor del fichero/directorio
int suDir[MAX_NUMBER_FILES]; // array para guardar la cantidad de ficheros o directoios contenido en un directorio
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
	
	//inicializar campos
	for(int i = 0; i < MAX_NUMBER_FILES; i++) {
		strcpy(iNodeNames[i], "");
		fileState[i] = CLOSED;
		superblock.inodes[i].name = iNodeNames[i];
		levels[i] = 0;
		strcpy(preDir[i], "")

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
	return -2;
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
    return -2;
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
