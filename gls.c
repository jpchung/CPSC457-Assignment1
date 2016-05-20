#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <openssl/md5.h>
#include <unistd.h>
#include <errno.h>

/*
CSPC 457
Assignment 1- Part A
Johnny Phuong Chung

The purpose of the gls program is to provide a graphical display of all files in a directory, and recursively display all files in any subdirectories, for any *nix system 
 */


/*use stat library to determine size of passed file */
int getStatSize(char *fileName) {

	//stat will define properties of given file in struct stat
	struct stat fileBuffer;
	stat(fileName, &fileBuffer);

	//size of file as determined by stat
	int size = fileBuffer.st_size;
	
	return size;				
}


/*use dirent to determine and return string of file type*/
char* getDirType(struct dirent *currentFile) {

	if (currentFile->d_type == DT_REG)
		return "regular file";
	else if (currentFile->d_type == DT_DIR)
		return "directory";
	else if (currentFile->d_type == DT_LNK)
		return "symbolic link";
	else if (currentFile->d_type == DT_FIFO)
		return "fifo (named pipe)";
	else if (currentFile->d_type == DT_BLK)
		return "block special device";
	else if (currentFile->d_type == DT_CHR)
		return "character special device";
	else
		return "unknown file type";

}

/*calculate directory size recursively via size of contained files */
void sizeDir(const char *dirName, int *total) {

	//stream and struct pointers for directory, and counter for directory scan
	int scanCount = -1;
	DIR *dirStream;
	struct dirent **dirList;
		
	//open dirent file, scan and sort contents alphabetically
	dirStream = opendir(dirName);
	scanCount = scandir(dirName, &dirList, 0, alphasort);

	//if scan unsuccessful, then directory doesn't exist - print error and return
	if(scanCount < 0) {
 		perror(dirName);
 		return;
	}

	//get instance of current directory to return to
	DIR *currentStream = opendir(".");
	//change to passed directory
	chdir(dirName);

	//loop: size of directory via the size of contained files
	int i = 0;
	for(i = 0; i < scanCount; i++)  {

		char *fileName = dirList[i]->d_name;
		
		//skip hidden files
		if(fileName[i] == '.') {
			free(dirList[i]);
			continue;
		}
		
		//check file type
		else {
			if(dirList[i]->d_type == DT_DIR) {

				//skip self and parent directories for size determination
				if(strcmp(fileName, ".") == 0 || strcmp(fileName, "..") == 0) {
					free(dirList[i]);
					continue;
				}
				
				//recursive call for subdirectories
				sizeDir(fileName, total);
			}
			
			else if(dirList[i]->d_type == DT_REG) {
				//get size of file
				int size  = getStatSize(fileName);
	
				//add regular file size to parent directory size via pointer
				(*total) += size;
				free(dirList[i]);
				continue;
			}
			
			
		}
		//when done with struct entry, free its memory
		free(dirList[i]);
	}
	//change back to and close current directory stream once done
	fchdir(dirfd(currentStream));
	closedir(currentStream);

	//free direct struct memory
	free(dirList);
	return;

}


/*calculate and return MD5 checksum of passed file */
void getChecksum(char *fileName, unsigned char *hash) {
	
	//open file stream
	FILE *inFile = fopen(fileName, "rb");

	//context and data array for MD5
	MD5_CTX mdContext;
   	int bytes;
   	unsigned char data[1024];

	//check if file can be opened
	if (inFile == NULL) {
        	printf ("%s can't be opened.\n", fileName);
        	return;
	}

	//initialize and calculate checksum
	MD5_Init (&mdContext);
	while ((bytes = fread (data, 1, 1024, inFile)) != 0)
        	MD5_Update (&mdContext, data, bytes);

	//store checksum in desired hash array
	MD5_Final (hash,&mdContext);

	//close file stream
	fclose (inFile);

}


/*print directory contents alphabetically and recursively*/
void alphaListDir(char *dirName, int level) {

	//stream and struct pointers for directory, and counter for directory scan	
	int scanCount = -1;
	DIR *dirStream;
	struct dirent **dirList;

	//open dirent file, scan and sort contents alphabetically
	dirStream = opendir(dirName);
	scanCount = scandir(dirName, &dirList, 0, alphasort);

	//if scan unsuccessful, then directory doesn't exist - print error and return
	if(scanCount < 0) {
 		perror(dirName);
 		return;
	}

	//get instance of current directory to return to
	DIR *currentStream = opendir(".");

	//change to passed directory
	chdir(dirName);

	//loop: print sorted directory contents and file descriptions
	int i = 0;
	for(i = 0; i < scanCount; i++)  {

		char *fileName = dirList[i]->d_name;
		char *fileType = getDirType(dirList[i]);
		
		//skip hidden files
		if(fileName[i] == '.') {
			free(dirList[i]);
			continue;
		}
		
		//check file types
		else {
			if(dirList[i]->d_type == DT_DIR) {


				//skip self and parent directories for print
				if(strcmp(fileName, ".") == 0 || strcmp(fileName, "..") == 0) {
					free(dirList[i]);
					continue;
				}

				//calculate size of current directory
				int currentDirSize = 0;
				sizeDir(fileName, &currentDirSize);

				//print directories (with dashed indent if a subdirectory)
				if(level == 0)
					printf("%*s|[%s] (%s - %d)\n", level*3, "", fileName, fileType, currentDirSize);
				else
					printf("%*s|[%s] (%s - %d)\n", level*3, "---", fileName, fileType, currentDirSize);
				
				//recursive call for printing subdirectory contents 
				alphaListDir(fileName, level + 1);
			}
			
			else {
				 if(dirList[i]->d_type == DT_LNK){

					//absolute and relative paths of symbolic link
					char rels_path[256];
					char abs_path[256];

					/* Attempt to read the target of the symbolic link. */
					int lenPath = readlink(fileName, rels_path, sizeof(rels_path)-1);

					if (lenPath == -1) {
							/* The call failed. */
  							if (errno == EINVAL)
  								/* It's not a symbolic link; report that. */
   								fprintf (stderr, "%s is not a symbolic link\n", fileName);
  							else
   								/* Some other problem occurred; print the generic message. */
  								perror ("readlink");
 					}
					else {
						//call successful, print relative and absolute path of symbolic link target
						rels_path[lenPath] = '\0';
						realpath(rels_path, abs_path);
						printf("%*s|%s (%s - points to '%s' absolute path: '%s')\n", level*3, "", fileName, fileType, rels_path,abs_path);
					}

					free(dirList[i]);
					continue;
				}
				else if (dirList[i]->d_type == DT_REG) {


					//get size of file 
		 			int size  = getStatSize(fileName);


					//calculate md5 checksum of regular file 
					unsigned char hash[MD5_DIGEST_LENGTH];
					getChecksum(fileName, hash);

					//print regular file (with size and checksum)
					printf("%*s|%s (%s - %d - ", level*3, "", fileName, fileType, size);
					for(int i = 0; i < MD5_DIGEST_LENGTH; i++)
						printf("%02x", hash[i]);
					printf(")\n");
			
					free(dirList[i]);
					continue;
				}
				else {
					//generic print for other data types
					printf("%*s|%s (%s)\n", level*3, "", fileName, fileType);
				}
			}
			
			
		}
		//when done with struct entry, free its memory
		free(dirList[i]);
	}
	//change back to and close current directory stream once done
	fchdir(dirfd(currentStream));
	closedir(currentStream);

	//free direct struct memory
	free(dirList);
	return;
}



int main(int argc, char **argv)
{
	//check number of command line arguments for program usage
	if(argc == 1)
		alphaListDir(".", 0);
	 else if (argc == 2) {
		char *filename = argv[1];

		alphaListDir(filename, 0);
	}
	else
		printf("usage: ./gls <dir_path>\n");

	return 0;
}
