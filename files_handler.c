#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


// The function receives a string and an argument title. It checks if the string is a non negative integer.
// If it does, returns the number as long integer, otherwise prints a relevant message (using the argument title) and aborts.
long string_to_non_negative_integer(char* alleged_number_string, char* argument_title)
{
	
	// The string as an integer.
	int num;
	
	// No real need. Required for strtol.
	char* pointer;

	// Check if there are non-digit characters in the received string.
	for (int i=0; i<strlen(alleged_number_string); i++)
	{

		if (!(isdigit(alleged_number_string[i])))
		{
			printf("%s must be a number, please check your input and try again.\n", argument_title);
			printf("Aborting files handler.");
			abort();
		}
	}
	
	// Cast it to long integer.
	num = strtol(alleged_number_string, &pointer, 10);

	// Make sure it is a non-negative number.
	if (num < 0)
	{
		printf("%s must be a non-negative number, the received offset is: %d.\n", alleged_number_string, num);
		printf("Aborting files handler.");
		abort();
	}
	
	// This is a natural number (0 included), return it.
	return num;
}


void file_handler(int read, int write, const char* file_path, const char* writing_data, int writing_data_length, int offset, int delta)
{
	
	// The pointer to the file.
	FILE* file_pointer;
	
	// The tool reads the file char by char.
	char current_char;

	// Will contain the content of the file up to the offset.
	char file_content_before_offset[offset]; 

	// Will contain the content of the file up after the offset.
	char file_content_after_offset[delta]; 

	// The index on the current character in the file.
	int file_content_index = 0;

	// Open the file for reading, make sure that the file was opened successfully.
	if ((file_pointer = fopen(file_path, "r")) == NULL)
	{
		// Tell the user something went wrong.
		printf("Something went wrong trying to open the file. Please check again the file path.\n");
		
		// Exit the tool.
		printf("Aborting files handler.");
		abort();
	}

	// Read the characters of the file. Save them in two arrays: before and after the offset.
	while ((current_char = fgetc(file_pointer)) != EOF && file_content_index < offset + delta)
	{	
		// Before the offset.
		if (file_content_index < offset) {file_content_before_offset[file_content_index] = current_char;}
		
		// After the offset.
		else {file_content_after_offset[file_content_index - offset] = current_char;}
		
		// Point on the next cell.
		++file_content_index;
	}

	// Check if the offset exceeds the length of the file.
	if (file_content_index < offset) {file_content_before_offset[file_content_index] = '\0';}
	
	// An '\0' wasn't assigned to the before offset string, fix it.
	else {file_content_before_offset[offset] = '\0';}
	
	// Check if the file content is greater than offset + delta.
	if ( ! (file_content_index < offset + delta) ) {file_content_after_offset[delta] = '\0';} 

	// Finished reading the file.
	fclose(file_pointer);

	// The user wants to write data to the file.
	if (write)
	{
		
		// Open the file for writing, make sure that the file was opened successfully.
		if ((file_pointer = fopen(file_path, "w")) == NULL)
		{
			// Tell the user something went wrong.
			printf("Something went wrong trying to open the file. Please check again the file path.\n");
			
			// Exit the tool.
			printf("Aborting files handler.");
			abort();
		}
		
		// Now erase the file, write the characters before the offset, the received data to write and lastly the characters after the offset.
		fprintf(file_pointer, "%s", file_content_before_offset);
		fprintf(file_pointer, "%s", writing_data);
		fprintf(file_pointer, "%s", file_content_after_offset);
		
		// That's it. Close the file.
		fclose(file_pointer);
		
		// The user also wants to read the file.
		if (read) {printf("%s%s%s", file_content_before_offset, writing_data, file_content_after_offset);}
	}
	
	// The user wants to read the file.
	else if (read)
	{
		printf("%s", file_content_after_offset);
	}
}


int main(int argc, char **argv)
{
	// Flags. Indicates weather the user wants to read or write to the received file.
	int read = 0, write = 0;
	
	// The data to write.
	char* writing_data;
	
	// The received length of the data to write.
	int writing_data_length;
	
	// The cursor offset.
	char* s_offset; int is_offset = 0; long offset = 0;
	
	// The amount of characters to read.
	char* s_delta; int is_delta = 0; long delta = 10000;
	
	// Required for the arguments handling.
	int c;
	
	// The path of the desired file.
	char* file_path;
	
	// Unpack the arguments, to understand what the user wants to do.
	while ( (c = getopt(argc, argv, "hrw:o:d:")) != -1)
	{
		switch (c)
		{	
			// The user wants help.
			case 'h':
				
				// Explain the user how to use the tool.
				printf("\n--- Welcome to file handler ---\n");
				printf("\nWith file handler you can read data from a file and write data to a file.\n");
				printf("\nUsage: files_handler -<options> <file_path>\n");
				printf("\n!!! Important note: if the file contains more than 10,000 characters you must define delta (with -d) + offset to be greater than the characters amount !!!\n");
				printf("\nOptions:\n");
				printf("-r: Grant reading permission. With no more options specified, prints the content of the file.\n");
				printf("-w <data>: Grant writing permission. Write to the file the received data in the cursor location (default set to the beginning of the file). Max data length: 10,000 characters.\n");
				printf("-o <number>: Specify the cursor offset on the file.\n\t* With the reading option on, prints the content of the file from this offset.\n\t* With the writing permission on, writes to the file starting here.\n");
				printf("-d <number>: Specify the amount of characters to read from the file. Relevant only if the reading permission is on, and if the characters amount in the file exceeds 10,000. Default set to maximum of 10,000.\n");
				printf("\nExamples:\n");
				printf("files_handler -r -w 'hello world!' -o 5 'c:/my_file.txt'\n");
				printf("files_handler -r -o 10 -d 50 'c:/my_file.txt'\n");
				
				// Exit the tool.
				abort();
				
			// The user wants reading access.
			case 'r':
				
				read = 1;
				break;
			
			// The user wants writing access.
			case 'w':
				
				write = 1;
				writing_data_length = sizeof(optarg) / sizeof(optarg[0]);
				writing_data = optarg;
				break;
				
			// The user wants to set an offset to the cursor.
			case 'o':
				
				s_offset = optarg;
				is_offset = 1;
				break;
				
			// The user wants to read a limited amount of characters.
			case 'd':

				s_delta = optarg;
				is_delta = 1;
				break;
			
			// The user entered w/o/d without value.
			case ':':
                    
				printf("Option -%c requires a value\n", optopt);
				break;
			
			// Something went wrong, see if can specify the problem.
			case '?':
				
				// The user entered an undefined argument.
				printf("Unknown option '%c'.\n", optopt);
				
				// abort the tool.
				printf("aborting the files handler.\n");
				abort();
		}		
	}
	
	// If specified, check that the received offset and delta are non negative integers, and cast them.
	if (is_offset) {offset = string_to_non_negative_integer(s_offset, "Offset");}
	if (is_delta) {delta = string_to_non_negative_integer(s_delta, "Delta");}
	
	// Extract the path of the file. Abort if was not provided by the user.
	if (optind < argc) {file_path = argv[optind];}
	else {printf("Please enter the path to the file, right after the r/w arguments.\nAborting the files_handler.\n"); abort();}
		
	// An unwanted argument was received. Notify the user and abort.
	if (optind + 1 < argc) {printf("Received undefined argument: %s.\nAborting the files handler.\n", argv[optind + 1]); abort();}
	
	// Now we have all the required input, lets run the file handler with it.
	file_handler(read, write, file_path, writing_data, writing_data_length, offset, delta);
}