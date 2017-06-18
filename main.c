/*
 * C language application for replacing strings within a file
 * 
 * Purpose: This is a C language application for replacing strings within a file, 
 * using a simple command line utility called "replace",
 * 
 * ex: replace OPT <from> <to> -- <filename> [<filename>]*
 * 
 * where OPT can be zero or more of -b, -f, -l, -i
 * -b: if specified, the replace utility creates a backup copy of each file on  
 * which a replace operation is performed before modifying it.
 * 
 * -f: if specified, the replace utility only replaces the first occurrence of 
 * string from in each file.
 * 
 * -l: if specified, the replace utility only replaces the last occurrence of 
 * string from in each file.
 * 
 * -i: if specified, the replace utility performs the search for string from  
 * in a case insensitive way.
 * 
 * from: string to be replaced with string to.
 * to: string that will replace string from.
 * filename: the file(s) on which the replace operation has to be performed.
 */

/**
 * Limitations: 
 * max command line characters number <= 800
 * max number of files <= 10
 * max number of character in the single file <= 1024 char
 */

/* 
 * File:   main.c
 * Author: Amr Tarek
 *
 * Created on June 15, 2017, 10:43 AM
 * Version: 1.0
 */

/* 
 * The libraries include
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * The global constant
 */
#define MAX_NUM_OF_CMD_LINE 800 // replace (8), -b(3), <from> (50), <to> (50), -- (3), filename.txt (50)*[10] = 800 char.
#define MAX_NUM_OF_BUFFER_SIZE 1024

/*
 * The global variable
 */

typedef struct {
    int total_data_length; // Number of spaces
    char replace[10]; // the replace word
    char opt[5][5]; // the OPT parameter -- max [5]
    int opt_length; // Number of operations
    char from[100]; // from word
    char to[100]; // to word
    char filenames[10][100]; // the filenames [10 files]
    int files_length; // number of files
} the_parsed_data_type;

typedef struct {
    int the_occur_index [MAX_NUM_OF_BUFFER_SIZE];
    int the_occur_index_length;
} all_match_substring_type;

typedef struct {
    char buffer [MAX_NUM_OF_BUFFER_SIZE];
    int buffer_length;
} buffer_type;

/*
 Global function
 */

/**
 * Read data from user and put it in string
 * @param cmd_line
 * @return string of data
 */
void read_cmd_line(char * cmd_line) {
    // start the console with introduction.
    printf(" --------- Replacing string within a file ---------\n"
            " This is a C language application for replacing strings within a file, \n"
            " using a simple command line utility called replace\n"
            " ex: replace OPT <from> <to> -- <filename> [<filename>]*\n");
    // the command line data array
    int cmd_char;
    int cmd_count = 0;

    // save the command into the array
    cmd_char = getchar();
    // don't go over the array size!
    while ((cmd_count < (MAX_NUM_OF_CMD_LINE - 1)) && (cmd_char != '\n')) {
        cmd_line[cmd_count] = cmd_char;
        ++cmd_count;
        //get another character
        cmd_char = getchar();
    }
    // ending the array by null character.
    cmd_line[cmd_count] = '\0';
}

/**
 * Get the user string and parse it to its parameters, operations and validate it.
 * @param cmd_line
 * @param the_data_type_pointer
 * @return pointer to the_parsed_data_type structure
 */
void cmd_parse_data(char * cmd_line, the_parsed_data_type* the_data_type_pointer) {
    // get the spaces and separate the string into substring
    char * tmp;
    char cmd_data_parsed[MAX_NUM_OF_CMD_LINE][100]; // make array of string to save the cmd data
    // split the string by spaces
    tmp = strtok(cmd_line, " ");
    int i = 0;
    while (tmp != NULL) {
        // copy the result to another array
        int indx = 0;
        while (tmp[indx] != '\0') {
            cmd_data_parsed[i][indx] = tmp[indx];
            // add null terminator
            cmd_data_parsed[i][++indx] = '\0';
        }
        tmp = strtok(NULL, " ");
        i++; // number of parsing
    }
    the_data_type_pointer->total_data_length = i;
    // validate the input and arrange it
    if (strcmp(cmd_data_parsed[0], "replace") == 0) { // replace is exist
        strcpy(the_data_type_pointer->replace, cmd_data_parsed[0]);
        // get all operation mode
        int j = 0, opt_count = 0; // number of OPT
        while (j < 5 && j < i) { /* max number of opt 5 or the max parsing data */
            if (((strstr(cmd_data_parsed[j + 1], "0")) != NULL) ||
                    ((strstr(cmd_data_parsed[j + 1], "-b")) != NULL) ||
                    ((strstr(cmd_data_parsed[j + 1], "-f")) != NULL) ||
                    ((strstr(cmd_data_parsed[j + 1], "-l")) != NULL) ||
                    ((strstr(cmd_data_parsed[j + 1], "-i")) != NULL)) {
                strcpy(the_data_type_pointer->opt[opt_count], cmd_data_parsed[j + 1]);
                opt_count++;
            }
            j++;
        }
        the_data_type_pointer->opt_length = opt_count;
        if (!opt_count > 0) {
            printf("Sorry, You have to input a valid operation : 0 , -b, -f, -l, -i.\n");
            exit(0);
        } else {
            // check if there is <from> and <to> words
            if (i > (opt_count + 2)) {
                strcpy(the_data_type_pointer->from, cmd_data_parsed[opt_count + 1]); // from
                strcpy(the_data_type_pointer->to, cmd_data_parsed[opt_count + 2]); // to
                // check it there is file name and do not more than 10
                if ((i > (opt_count + 3)) && (i < 20) && (((strstr(cmd_data_parsed[opt_count + 3], "--")) != NULL))) {
                    if (i > (opt_count + 4) && i < 20) {
                        int k = 0;
                        while (k < 10 && k < i) {
                            strcpy(the_data_type_pointer->filenames[k], cmd_data_parsed[opt_count + 4 + k]);
                            k++;
                        }
                        the_data_type_pointer->files_length = (i - (opt_count + 4));
                    } else {
                        printf("Sorry, You have to input a filename with maximum 10 filenames.\n");
                        exit(0);
                    }
                } else {
                    printf("Sorry, You have to input:  -- <filename> [<filename>]*.\n");
                    exit(0);
                }
            } else {
                printf("Sorry, You have to input a valid <from> and <to>.\n");
                exit(0);
            }
        }
    } else {
        printf("Sorry, can't recognize the input data\n"
                "please enter appropriate input\n"
                "e.g: replace OPT <from> <to> -- <filename> [<filename>]*\n");
        exit(0);
    }
}

/**
 * Open the file and read it and save it in buffer
 * @param the file
 * @param buffer_type
 * @return Buffer
 */
void open_file(char * the_file_name, buffer_type * buffer_pointer) {
    FILE * file_pointer = fopen(the_file_name, "r");
    if (file_pointer != NULL) {
        //Read Text from File
        char file_data[MAX_NUM_OF_BUFFER_SIZE + 1];
        int file_data_length;
        fgets(file_data, MAX_NUM_OF_BUFFER_SIZE, file_pointer);
        file_data_length = strlen(file_data);
        file_data[file_data_length] = '\0';
        strcpy(buffer_pointer->buffer, file_data);
        buffer_pointer->buffer_length = file_data_length;

    } else {
        printf("Error, can't find the file: %s", the_file_name);
        exit(0);
    }
}

/**
 * Get the index of all substring in string
 * @param string
 * @param string length
 * @param substring
 * @param substring length
 * @return array of index and its length
 */
void find_all_match_substring_in_string(char * the_string, int the_string_length,
        char * the_substring, int the_substring_length,
        all_match_substring_type * all_match_substring_pointer, int case_sensitive) {

    int i = 0, k = 0, count = 0, match = 0;
    if (case_sensitive) {
        while (i < the_string_length) {
            while (the_string[i] == the_substring[k]) {
                i++;
                k++;
                count++;
                if (count == the_substring_length) {
                    all_match_substring_pointer->the_occur_index[match] = (i - (the_substring_length - 1));
                    match++;
                }
            }
            k = 0;
            count = 0;
            i++;
        }
    } else {
        while (i < the_string_length) {
            while ((the_string[i] | 32) == (the_substring[k] | 32)) {
                i++;
                k++;
                count++;
                if (count == the_substring_length) {
                    all_match_substring_pointer->the_occur_index[match] = (i - (the_substring_length - 1));
                    match++;
                }
            }
            k = 0;
            count = 0;
            i++;
        }
    }
    all_match_substring_pointer->the_occur_index_length = match;
}

/**
 * Replace substring in string
 * @param string
 * @param string length
 * @param substring
 * @param substring length
 * @param index of substring in the string
 * @return the modified string
 */
void replace_substring_in_string(char * the_string, int the_string_length,
        char * the_substring_from, int the_substring_from_length,
        char * the_substring_to, int the_substring_to_length,
        int index_of_sub, buffer_type * buffer_pointer) {

    char new_string[MAX_NUM_OF_BUFFER_SIZE];

    // break the string to two strings, first before the substring and the second after substring.
    // first string
    const int length_of_first_string = (index_of_sub - 1);
    char first_string[length_of_first_string];
    int i = 0;
    for (i = 0; i < length_of_first_string; i++) {
        first_string[i] = the_string[i];
    }
    first_string[i] = '\0';
    if (i > 0) {
        strcpy(new_string, first_string);
    }
    // the from string
    strcat(new_string, the_substring_to);
    // second string
    const int length_of_second_string = ((the_string_length - 1) - ((index_of_sub - 1) + (the_substring_from_length - 1)));
    char second_string[length_of_second_string];
    int j = 0, k = ((index_of_sub - 1) + (the_substring_from_length));
    for (j = 0; j < length_of_second_string; k++, j++) {
        second_string[j] = the_string[k];
    }
    second_string[j] = '\0';

    if (j > 0) {
        strcat(new_string, second_string);
    }
    strcpy(buffer_pointer->buffer, new_string);
    buffer_pointer->buffer_length = strlen(new_string);
}

/**
 * Edit the buffer only without saving it again
 * @param the string
 * @param the string length
 * @param the substring
 * @param the substring length
 * @param the edit mode
 * @return the edited buffer and the new length
 */
void search_and_replace_the_buffer(char * the_string, int the_string_length,
        char * the_substring_from, int the_substring_from_length,
        char * the_substring_to, int the_substring_to_length,
        char * the_mode, buffer_type * buffer_struct_ptr) {

    // get the mode char
    char * ptr;
    all_match_substring_type all_match_substring_struct;
    all_match_substring_type * all_match_substring_pointer = &all_match_substring_struct;
    strcpy(buffer_struct_ptr->buffer, the_string);
    buffer_struct_ptr->buffer_length = the_string_length;

    /*replace all exist in case sensitive way*/
    if ((ptr = strstr(the_mode, "0")) != NULL) {
        // 1- find all substring and but its total number and index in the (all_match_substring_type) structure
        find_all_match_substring_in_string(
                the_string, the_string_length,
                the_substring_from, the_substring_from_length,
                all_match_substring_pointer, 1);

        // 2- replace_substring_in_string
        //        printf("the old buffer: %s, new length: %d\n", the_string, the_string_length);
        int buffer_offset = 0;
        int new_buffer_length = the_string_length;
        int i = 0;
        // replace all exist
        while (i < all_match_substring_pointer->the_occur_index_length) {
            // due to iteration it will be an offset in occur_index from the old string and new one
            buffer_offset = new_buffer_length - the_string_length;
            int occur_index = all_match_substring_pointer->the_occur_index[i] + buffer_offset;
            replace_substring_in_string(
                    buffer_struct_ptr->buffer, buffer_struct_ptr->buffer_length,
                    the_substring_from, the_substring_from_length,
                    the_substring_to, the_substring_to_length, occur_index, buffer_struct_ptr);

            new_buffer_length = buffer_struct_ptr->buffer_length;
            i++;
        }
        //        printf("the last buffer: %s, new length: %d\n", buffer_struct_ptr->buffer, buffer_struct_ptr->buffer_length);
    } else /*replace the first only in case sensitive way*/
        if ((ptr = strstr(the_mode, "-f")) != NULL) {
        // 1- find all substring and but its total number and index in the (all_match_substring_type) structure
        find_all_match_substring_in_string(
                the_string, the_string_length,
                the_substring_from, the_substring_from_length,
                all_match_substring_pointer, 1);

        // 2- replace_substring_in_string
        //        printf("the old buffer: %s, new length: %d\n", the_string, the_string_length);

        // replace the last one only
        replace_substring_in_string(
                buffer_struct_ptr->buffer, buffer_struct_ptr->buffer_length,
                the_substring_from, the_substring_from_length,
                the_substring_to, the_substring_to_length, all_match_substring_pointer->the_occur_index[0], buffer_struct_ptr);

        //        printf("the last buffer: %s, new length: %d\n", buffer_struct_ptr->buffer, buffer_struct_ptr->buffer_length);
    } else /*replace the last only in case sensitive way*/
        if ((ptr = strstr(the_mode, "-l")) != NULL) {
        // 1- find all substring and but its total number and index in the (all_match_substring_type) structure
        find_all_match_substring_in_string(
                the_string, the_string_length,
                the_substring_from, the_substring_from_length,
                all_match_substring_pointer, 1);

        // 2- replace_substring_in_string
        //        printf("the old buffer: %s, new length: %d\n", the_string, the_string_length);

        // replace the last one only
        replace_substring_in_string(
                buffer_struct_ptr->buffer, buffer_struct_ptr->buffer_length,
                the_substring_from, the_substring_from_length,
                the_substring_to, the_substring_to_length, all_match_substring_pointer->the_occur_index[all_match_substring_pointer->the_occur_index_length - 1], buffer_struct_ptr);

        //        printf("the last buffer: %s, new length: %d\n", buffer_struct_ptr->buffer, buffer_struct_ptr->buffer_length);
    } else /*replace all exist in non case sensitive way*/
        if ((ptr = strstr(the_mode, "-i")) != NULL) {
        // 1- find all substring and but its total number and index in the (all_match_substring_type) structure
        find_all_match_substring_in_string(
                the_string, the_string_length,
                the_substring_from, the_substring_from_length,
                all_match_substring_pointer, 0);

        // 2- replace_substring_in_string
        //        printf("the old buffer: %s, new length: %d\n", the_string, the_string_length);
        int buffer_offset = 0;
        int new_buffer_length = the_string_length;
        int i = 0;
        //replace in non case sensitive way
        while (i < all_match_substring_pointer->the_occur_index_length) {
            // due to iteration it will be an offset in occur_index from the old string and new one
            buffer_offset = new_buffer_length - the_string_length;
            int occur_index = all_match_substring_pointer->the_occur_index[i] + buffer_offset;
            replace_substring_in_string(
                    buffer_struct_ptr->buffer, buffer_struct_ptr->buffer_length,
                    the_substring_from, the_substring_from_length,
                    the_substring_to, the_substring_to_length, occur_index, buffer_struct_ptr);
            new_buffer_length = buffer_struct_ptr->buffer_length;
            i++;
        }
        //        printf("the last buffer: %s, new length: %d\n", buffer_struct_ptr->buffer, buffer_struct_ptr->buffer_length);
    }

}

/**
 * Get the string (buffer) and save it or backup before save
 * @param Buffer
 * @param the Buffer length
 * @return Buffer
 */
void save_file(char * the_file_name, buffer_type * buffer_pointer) {
    FILE * file_pointer = fopen(the_file_name, "w");
    if (file_pointer != NULL) {
        fputs(buffer_pointer->buffer, file_pointer);
        fclose(file_pointer);
    } else {
        printf("Error, can't find the file: %s", the_file_name);
        exit(0);
    }
}

/*
 * Run and control the operation of the program.
 */
int main(int argc, char** argv) {

    char cmd_line[MAX_NUM_OF_CMD_LINE];
    char backup_filename[MAX_NUM_OF_BUFFER_SIZE];

    the_parsed_data_type cmd_data_parsed_struct;
    the_parsed_data_type * cmd_data_parsed_struct_ptr = &cmd_data_parsed_struct;
    buffer_type buffer_struct;
    buffer_type * buffer_struct_ptr = &buffer_struct;

    // read the command line input and put it in array.
    read_cmd_line(cmd_line);

    //parse and validate the data    
    cmd_parse_data(cmd_line, cmd_data_parsed_struct_ptr);

    // handling the command line arguments
    // iterate until finishing all files
    int index_files = 0;
    int index_opt = 0;

    //check for existence of all file before continue
    char * not_exist[MAX_NUM_OF_CMD_LINE];
    int not_exist_counter = 0;
    while (index_files < cmd_data_parsed_struct_ptr->files_length) {
        if (fopen(cmd_data_parsed_struct_ptr->filenames[index_files], "r") == NULL) {
            not_exist[not_exist_counter] = cmd_data_parsed_struct_ptr->filenames[index_files];
            not_exist_counter++;
        }
        index_files++;
    }
    if (not_exist_counter > 0) {
        printf("Error, can't find the files: ");
        index_files = 0;
        while (index_files < not_exist_counter) {
            printf("%s ", not_exist[index_files]);
            index_files++;
        }
        exit(0);
    }

    index_files = 0;
    while (index_files < cmd_data_parsed_struct_ptr->files_length) {

        //1- open and read the file
        open_file(cmd_data_parsed_struct_ptr->filenames[index_files], buffer_struct_ptr);

        //2- check for backup
        index_opt = 0;
        while (index_opt < cmd_data_parsed_struct_ptr->opt_length) {
            if ((strstr(cmd_data_parsed_struct_ptr->opt[index_opt], "-b")) != NULL) {
                strcat(backup_filename, "backup_of_");
                strcat(backup_filename, cmd_data_parsed_struct_ptr->filenames[index_files]);
                save_file(backup_filename, buffer_struct_ptr);
            }
            index_opt++;

        }

        //3- handle the replace arguments
        index_opt = 0;
        while (index_opt < cmd_data_parsed_struct_ptr->opt_length) {
            search_and_replace_the_buffer(buffer_struct_ptr->buffer, buffer_struct_ptr->buffer_length,
                    cmd_data_parsed_struct_ptr->from, strlen(cmd_data_parsed_struct_ptr->from),
                    cmd_data_parsed_struct_ptr->to, strlen(cmd_data_parsed_struct_ptr->to),
                    cmd_data_parsed_struct_ptr->opt[index_opt], buffer_struct_ptr);
            index_opt++;
        }

        //printf("the new buffer: %s, new length: %d \n", buffer_struct_ptr->buffer, buffer_struct_ptr->buffer_length);
        // last saving the file
        save_file(cmd_data_parsed_struct_ptr->filenames[index_files], buffer_struct_ptr);

        // then get the next file
        index_files++;
    }
    printf("Success\n");
    return (EXIT_SUCCESS);
}
