# Project Title

C application for replacing strings within a file

## Purpose

This is a C language application for replacing strings within a file, 
using a simple command line utility called "replace",

	ex: replace OPT <from> <to> -- <filename> [<filename>]*

where OPT can be zero or more of -b, -f, -l, -i

* **0**: if specified, the replace utility performs the search for string from 
  in a case sensitive way.
 	
* **-b**: if specified, the replace utility creates a backup copy of each file on
 which a replace operation is performed before modifying it.

* **-f**: if specified, the replace utility only replaces the first occurrence of 
 string from in each file.
  
* **-l**: if specified, the replace utility only replaces the last occurrence of 
  string from in each file.
  
* **-i**: if specified, the replace utility performs the search for string from 
  in a case insensitive way.
  
* **from**: string to be replaced with string to.
   
* **to**: string that will replace string from.
  
* **filename**: the file(s) on which the replace operation has to be performed.

### Prerequisites

```
Netbeans IDE with c/c++ compiler (GCC)
```

### Installing and Run


```
1- Download the zip file, and unzip it.
2- Open the Netbeans (IDE).
3- File -> Open Project, then select the project (C application for replacing strings within a file).
5- create files with data in it to replace and put it in the same directory of main.c
4- run it
```

## Test Example

You can input in form : replace OPT <from> <to> -- <filename> [<filename>]*
```
replace -i Howdy Hello -- file1.txt file2.txt file3.txt

replace -b -f Bill William -- file1.txt file2.txt

replace -f -l abc ABC -- file1.txt

```


## Versioning

version 1.0. 

## Authors

* **Amr Tarek Hassan** - *web site* - [AmrTarek](http://amrtarek.com/)


