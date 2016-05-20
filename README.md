CPSC 457
Assignment 1 - README
Johnny Phuong Chung

DESCRIPTION:
 - The purpose of the gls program is to provide a graphical display of all files in a directory, and recursively display all files in any subdirectories, for any *nix system 

COMPILE AND RUN:
In terminal:
- use command $cd to navigate to directory containing gls.c and Makefile
- use command $make to have Makefile compile gls.c with appropriate flags and libraries
- use command $./gls <dir_path> to run program

USAGE:
	-$./gls
		- zero arguments will run program for current directory
	-$./gls <dir_path> 
		- takes single argument, path of desired directory to run program for

ASSUMPTIONS:
- Files and directories are displayed in alphabetical order, as that is how the UNIX utility command $ls displays files and directories
- Parent directories and hidden files are omitted from the file display and size calculations
- File and directory sizes are calculated in bytes, and directory sizes are recursively defined by contained file sizes
- Command line argument passed is a syntactically correct name for an existing directory


SOURCES:
- getStatSize() is modified from the following source code:
	http://stackoverflow.com/questions/3138600/correct-use-of-stat-on-c (Author: Anjum Kaiser)

- getCheckSum() is modified from the following source code: 
	http://stackoverflow.com/questions/10324611/how-to-calculate-the-md5-hash-of-a-large-file-in-c (Author: askovpen)


- alphaListDir() is modified from the following source codes:
	http://stackoverflow.com/questions/8436841/how-to-recursively-list-directories-in-c-on-linux (Author: lloydm)
	http://stackoverflow.com/questions/5102863/how-to-sort-files-in-some-directory-by-the-names-on-linux (Author: hipe)
	http://linux.die.net/man/3/readlink
	http://www.informit.com/articles/article.aspx?p=23618&seqNum=12 (Author: Mark L. Mitchell)

- sizeDir() is modifified from the following source code:
	http://stackoverflow.com/questions/8436841/how-to-recursively-list-directories-in-c-on-linux (Author: lloydm)
	http://stackoverflow.com/questions/5102863/how-to-sort-files-in-some-directory-by-the-names-on-linux (Author: hipe)

