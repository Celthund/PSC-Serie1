#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

int DEFAULT_SIZE = 10;

enum options {NO_OPTION, DELETE_COLUMN, FIND, NORMALIZE, REPLACE_SEPERATOR};

struct parsedArgs{
    char *output_file, *input_file;
    enum options option;
    char *optionArgs[2];
};

/**
 * Adds to the struct receive all arguments parsed.
 * @param parse struct to save the parsed arguments passed.
 * @param argc number of arguments passed.
 * @param argv array with the arguments passed.
 * @return
 */
int parseArgs(struct parsedArgs *parse, int argc, char *argv[]){
    parse->output_file = NULL;
    parse->input_file = NULL;
    parse->option = NO_OPTION;
    for(int i = 1; i < argc; i++){
        if (**(argv + i) == '-' && parse->option == NO_OPTION){
            switch (*(*(argv + i) + 1)) {
                case 'o':
                    parse->output_file = *(argv + ++i);
                    break;
                case 'd':
                    parse->option = DELETE_COLUMN;
                    parse->optionArgs[0] = *(argv + ++i);
                    break;
                case 'f':
                    parse->option = FIND;
                    parse->optionArgs[0] = *(argv + ++i);
                    parse->optionArgs[1] = *(argv + ++i);
                    break;
                case 'n':
                    parse->option = NORMALIZE;
                    break;
                case 's':
                    parse->option = REPLACE_SEPERATOR;
                    parse->optionArgs[0] = *(argv + ++i);
                    parse->optionArgs[1] = *(argv + ++i);
                    break;
            }
        } else {
            parse->input_file = *(argv + i);
        }

    }
    return 0;
}

/**
 * Parse the file considering it is in a csv file format where each cell is divided by a comma.
 * https://en.wikipedia.org/wiki/Comma-separated_values
 * @param csv
 * @param table
 * @param size_table
 * @return
 */
int parseCSV(FILE *csv, char ***table, size_t size_table){
    int default_sizes = DEFAULT_SIZE;
    size_t size_cell = default_sizes, size_line = default_sizes;
    // Reserve memory for array of char for saving every character and reserve memory to the line that will save each cell.
    char *cell = (char *) calloc(size_cell, sizeof(char)), **line = (char **) calloc(size_line, sizeof(table));
    // Counters for number of elements in each array.
    int c, counter_cell = 0, counter_line = 0, counter_table = 0;
    // Flag for quotation mark.
    int quotation = 0;
    while((c = fgetc(csv)) != EOF) {
        if (c == '"' && quotation == 0){
            quotation = 1;
        } else if (c == '"' && quotation == 1){
            quotation = 0;
        }
        if ((c == ',' || c == '\n') && quotation == 0){
            // Check if there is space for one more character in the cell.
            if (counter_cell >= size_cell){
                size_cell += 1;
                cell = realloc(cell, size_cell);
            }
            // Add 0 to the end the cell element to mark end of word.
            *(cell + counter_cell) = 0;
            // Check if there is space for more cells in the line.
            if (counter_line >= size_line){
                size_line *= 2;
                line = realloc(line, size_line);
            }
            // Add cell to the line.
            *(line + counter_line++) = cell;
            // Reset cell
            size_cell = default_sizes;
            counter_cell = 0;
            cell = (char *) calloc(size_cell, sizeof(char));
            if (c == '\n'){
                // Check if there is space for one more character in the line.
                if (counter_line >= size_line){
                    size_line += 1;
                    line = realloc(line, size_line);
                }
                // Add 0 to the end the cell element to mark end of the line.
                *(line + counter_line) = 0;
                // Check if there is space for more lines in the table.
                if (counter_table >= size_table){
                    size_table *= 2;
                    table = realloc(table, size_table);
                }
                // Add line to the table.
                *(table + counter_table++) = line;
                // Reset line.
                size_line = default_sizes;
                counter_line = 0;
                line = (char **) calloc(size_line, sizeof(table));
            }
        } else {
            // Add letter to cell element to create a word.
            if (counter_cell >= size_cell){
                size_cell *= 2;
                cell = realloc(cell, size_cell);
            }
            *(cell + counter_cell++) = (char)c;
        }
    }
    // free unused cell and line
    free(cell);
    free(line);
    return counter_table;
}

/**
 * Parse a string to a integer.
 * @param string to parse
 * @return int
 */
int stringToInt(char *string){
    int num = 0, i = 0;
    while (*(string + i) != 0){
        num = num * 10 + (*(string + i) - '0') ;
        i++;
    }
    return num;
}

/**
 * Return number of the elements until a NULL element.
 * @param line array to check
 * @return int
 */
int countLineSize(char **line){
    int column = 0;
    while((*(line + column)) != NULL){
        column++;
    }
    return column;
}

/**
 * Return the number of elements in the longest line, element wise, in the table.
 * @param table contains all the lines.
 * @param number_lines number of lines in the table.
 * @return int
 */
int countMaxColumns(char ***table, size_t number_lines){
    int max_count = 0, column;
    for (int i = 0; i < number_lines; i++){
        column = countLineSize((*(table + i)));
        if (column > max_count){
            max_count = column;
        }
    }
    return max_count;
}

/**
 * Checks if a pattern is somewhere in a string.
 * @param string
 * @param pattern
 * @return 1 if found 0 otherwise
 */
int patternExists(char *string, char *pattern){
    int i = 0, j = 0, bool = 0;
    char s, p;
    while((s = *(string + i)) != 0){
        if(s == *(pattern + j)){
            bool = 1;
            while ((p = *(pattern + j) )!= 0){
                if(*(string + i + j) != p || *(string + i + j) == 0) {
                    bool = 0;
                    break;
                }
                j++;
            }
        }
        if(bool) break;
        i++;
    }
    return bool;
}

/**
 * Given a table, will write to a file the whole table without the given column_number.
 * @param output file to output.
 * @param table table to print.
 * @param number_lines number of lines in table.
 * @param column_number column to remove.
 */
void deleteColumn(FILE* output, char ***table, size_t number_lines, int column_number){
    char *string;
    int column = 0;
    for (int i = 0; i < number_lines; i++){
        while((string = *((*(table + i)) + column)) != NULL){
            if (column != column_number - 1) {
                fputs(string, output);
                if (*((*(table + i)) + column + 1) == NULL || (column + 1 == column_number - 1 && *((*(table + i)) + column + 2) == NULL))
                    fputc('\n', output);
                else
                    fputc(',', output);
            }
            column++;
        }
        column = 0;
    }
}

/**
 * Writes to a file the table with each cell separated with the arguement seperator.
 * @param output file to output.
 * @param table table to print.
 * @param number_lines number of lines in table.
 * @param seperator char to be used as seperator
 */
void replaceSeparator(FILE* output, char ***table, size_t number_lines, char seperator){
    char *string;
    int column = 0;
    for (int i = 0; i < number_lines; i++){
        while((string = *((*(table + i)) + column)) != NULL){
            fputs(string, output);
            if (*((*(table + i)) + column + 1)  == NULL){
                fputc('\n', output);
            }
            else
                fputc(seperator, output);
            column++;
        }
        column = 0;
    }
}

/**
 * Writes to a file the table with each line with the same number of cells choosing the longest line, element wise
 * as the number of cells to be used.
 * @param output file to output.
 * @param table table to print.
 * @param number_lines number of lines in table.
 */
void normalize(FILE* output, char ***table, size_t number_lines){
    int max_columns = countMaxColumns(table, number_lines), column = 0;
    char *string;
    for (int i = 0; i < number_lines; i++){
        while((string = *((*(table + i)) + column)) != NULL){
            fputs(string, output);
            if (*((*(table + i)) + column + 1)  == NULL){
                if (column < max_columns - 1){
                    for(int j = 0; j < (max_columns - 1 - column); j++){
                        fputc(',', output);
                    }
                }
                fputc('\n', output);
            }
            else
                fputc(',', output);
            column++;
        }
        column = 0;
    }
}

/**
 * Writes to a file the lines in table that match the pattern in a specific column.
 * @param output file to output.
 * @param table table to print.
 * @param number_lines number of lines in table.
 * @param column_number column to search the pattern.
 * @param pattern pattern to be searched.
 */
void find(FILE* output, char ***table, size_t number_lines, int column_number, char *pattern){
    int line_size, column = 0;
    char *string, **line;
    for(int i = 0; i < number_lines; i++){
        line_size = countLineSize(*table + i);
        if (column_number < line_size){
            line = *(table + i);
            if(patternExists(*(line + column_number), pattern) == 1){
                while((string = *(line + column)) != NULL){
                    fputs(string, output);
                    if (*((*(table + i)) + column + 1)  == NULL){
                        fputc('\n', output);
                    }
                    else
                        fputc(',', output);
                    column++;
                }
                column = 0;
            }
        }
    }
}

int main (int argc, char *argv[]){
    struct parsedArgs args;
    parseArgs(&args, argc, argv);
    if (args.input_file == NULL) {
        printf("%s", "No input file given.\n");
        return 0;
    }
    if (args.output_file == NULL) {
        printf("%s", "No output file given.\n");
        return 0;
    }
    FILE *input = fopen(args.input_file, "r");;
    FILE *output = fopen(args.output_file, "w");
    int size_table = DEFAULT_SIZE;
    char ***table = (char ***) malloc(size_table);
    int number_lines = parseCSV(input, table, size_table);
    if (args.option == DELETE_COLUMN){
        deleteColumn(output, table, number_lines,stringToInt(args.optionArgs[0]));
    }
    else if (args.option == FIND){
        find(output, table, number_lines, stringToInt(args.optionArgs[0]), args.optionArgs[1]);
    }
    else if (args.option == NORMALIZE){
        normalize(output, table, number_lines);
    }
    else if (args.option == REPLACE_SEPERATOR){
        replaceSeparator(output, table, number_lines, *args.optionArgs[1]);
    }
    else {
        printf("%s", "No valid operation given.\n");
    }
    fclose(input);
    fclose(output);
    return 0;
}
