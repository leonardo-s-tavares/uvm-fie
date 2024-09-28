#include "converter_header.h"

int num_enums = 0;

// Function to get the list of "sc_in" that the user wants to convert.
void get_sc_in_names(char sc_in_name[100][100]) {
    int i;
    
    clean_matrix(sc_in_name);
    puts("Please inform the sc_in variable names you want to inject faults (type '0' to exit):");
    for(i = 0; i < 100 && sc_in_name[i-1][0] != '0'; i++) { // Ask name by name until the user types '0'.
        printf("sc_in[%d]: ", i); // Print the current sc_in index.
        scanf("%s", sc_in_name[i]); // Get the sc_in name.
    }
}

// Function to create "sc_signal" derived from a previous "sc_in".
void create_sc_signal_from_sc_in(FILE *input, FILE *output, char sc_in_name[100][100], int sc_in_present[100]) {
    char *line_ptr = NULL; // Pointer to the current line.
    size_t len = 0;
    char line[1000]; // Current line.
    char line_aux[1000]; // Auxiliary line.
    int new_line; // To make sure that the new line will be written only once.
    int sc_in_is_present = 0; // To inform the user if there's no sc_in to convert.
    int i;

    rewind(input); // Rewind to the beginning of the input file.
    while(getline(&line_ptr, &len, input) != -1) { // Go line by line until the end.
        strcpy(line, line_ptr); // Copy the line.
        fputs(line, output); // Write the line.
        if(strstr(line, "sc_in") != NULL && strstr(line, "sc_in_clk") == NULL) { // If the line has an sc_in but not an sc_in_clk:
            new_line = 0; // Initialize the flag.
            for(i = 0; sc_in_name[i][0] != '0'; i++) { // Search for the variable's names from the user in the line.
                if(strstr(line, sc_in_name[i]) != NULL) { // If the line has one of the signals informed by the user:
                    new_line = 1; // A new line with sc_signal(s) will be required.
                    sc_in_is_present = 1; // At least one of the sc_in's informed by the user is present.
                    str_replace(line, "sc_in", "sc_signal"); // Replace sc_in by sc_signal in the line.
                    strcpy(line_aux, sc_in_name[i]); // Do a copy of the variable's name to line_aux.
                    strcat(line_aux, "_fault_target_from_in"); // Create a new variable with the suffix '_fault_target_from_in'.
                    str_replace(line, sc_in_name[i], line_aux); // Add the new variable's name in the line.
                    sc_in_present[i] = 1; // Indicate that the sc_in listed is present.
                }
            }
            if(new_line == 1) { // If a new line with sc_signals is required:
                str_replace(line, ";", "; // Created by the Converter tool to get and send faults to an sc_in."); // Add a comment to identify the conversion.
                fputs(line, output); // Write the line.
            }
        }
    }
    if(!sc_in_is_present) { // If none of the sc_in's informed by the user was found:
        puts("None of the sc_in signals informed was declared."); // Print a message to the user saying that.
    }
}

// Function to connect the "sc_signal" crea ted to the original "sc_in".
void connect_sc_in_to_sc_signal(FILE *input, FILE *output, char sc_in_name[100][100], int sc_in_present[100]) {
    char *line_ptr = NULL; // Pointer to the current line.
    size_t len = 0;
    char line[1000]; // Current line.
    char line_aux[1000]; // Auxiliary line.
    int i;
    
    rewind(input); // Rewind to the beginning of the input file.
    while(getline(&line_ptr, &len, input) != -1) { // Go line by line until the end.
        strcpy(line, line_ptr); // Copy the line.
        if(strstr(line, "void adjust()") != NULL || strstr(line, "void connections()") != NULL || strstr(line, "void logic()") != NULL) { // If the line starts a function to connect signals:
            fputs(line, output); // Write the line.
            for(i = 0; sc_in_name[i][0] != '0'; i++) { // Search into the sc_in's names informed by the user.
                if(sc_in_present[i]) { // If the sc_in listed was previously declarated:
                    strcpy(line_aux, "        "); // Start a new line with identation.
                    strcat(line_aux, sc_in_name[i]); // Add the sc_signal preffix name.
                    strcat(line_aux, "_fault_target_from_in.write("); // Add the sc_signal suffix name and the function write().
                    strcat(line_aux, sc_in_name[i]); // Add the sc_in variable that will pass the value.
                    strcat(line_aux, ".read()); // Created by the Converter tool to get and send faults to an sc_in.\n"); // Add the function read() and a comment.
                    fputs(line_aux, output); // Write the line.
                }
            }
        } else { // If the line doesn't start a constructor:
            fputs(line, output); // Just write the line.
        }
    }
}

// Function to replace the sc_in signals by the new sc_signal signals during assignments.
void replace_sc_in_on_assigns(FILE *input, FILE *output, char sc_in_name[100][100]) {
    char *line_ptr = NULL; // Pointer to the current line.
    size_t len = 0;
    char line[1000]; // Current line.
    char line_aux_1[1000], line_aux_2[1000], line_aux_3[1000]; // Auxiliary lines.
    int i;
    
    rewind(input); // Rewind to the beginning of the input file.
    while(getline(&line_ptr, &len, input) != -1) { // Go line by line until the end.
        strcpy(line, line_ptr); // Copy the line.
        for(i = 0; sc_in_name[i][0] != '0'; i++) { // Search into the sc_in's names informed by the user.
            if(strstr(line, "->") != NULL && strstr(line, "(") != NULL && strstr(line, sc_in_name[i]) != NULL) { // If the line has a "->", a "(" and a sc_in name, we assume it is a port connection.
                strcpy(line_aux_1, "("); // To put the sc_in name around parenthesis.
                strcat(line_aux_1, sc_in_name[i]); // To put the sc_in name around parenthesis.
                strcat(line_aux_1, ")"); // To put the sc_in name around parenthesis.
                strcpy(line_aux_2, "("); // To put the new sc_signal name around parenthesis.
                strcat(line_aux_2, sc_in_name[i]); // To put the new sc_signal name around parenthesis.
                strcat(line_aux_2, "_fault_target_from_in)"); // To put the new sc_signal name around parenthesis.
                strcpy(line_aux_3, line); // To see if the next code will change the line.
                str_replace(line, line_aux_1, line_aux_2); // To replace the correct sc_in by the corresponding sc_signal.
                if(strcmp(line, line_aux_3) != 0) { // To only add the comment if the converter made some change.
                    if(strstr(line, "// Created") == NULL) { // To ensure that the comment won't show up twice.
                        str_replace(line, ";", "; // Created by the Converter tool to get and send faults to an sc_in."); // Add a comment.
                    }
                }
                break;
            }
        }
        fputs(line, output); // Write the line.
    }
}

// Function to replace the sc_in signals by the new sc_signal on if's.
void replace_sc_in_on_ifs(FILE *input, FILE *output, char sc_in_name[100][100]) {
    char *line_ptr = NULL; // Pointer to the current line.
    size_t len = 0;
    char line[1000]; // Current line.
    char line_aux[1000]; // Auxiliary lines.
    int i;
    
    rewind(input); // Rewind to the beginning of the input file.
    while(getline(&line_ptr, &len, input) != -1) { // Go line by line until the end.
        strcpy(line, line_ptr); // Copy the line.
        for(i = 0; sc_in_name[i][0] != '0'; i++) { // Search into the sc_in's names informed by the user.
            if((strstr(line, "if(") != NULL || strstr(line, "if (") != NULL) && strstr(line, sc_in_name[i]) != NULL) { // If the line has an if and a sc_in name:
                strcpy(line_aux, sc_in_name[i]); // Get the sc_signal preffix name.
                strcat(line_aux, "_fault_target_from_in"); // Add the sc_signal suffix name.
                str_replace(line, sc_in_name[i], line_aux); // Replace the sc_in by the corresponding sc_signal.
                if(strstr(line, "// Created") == NULL) { // To ensure that the comment won't show up twice.
                    str_replace(line, "\n", " // Created by the Converter tool to get and send faults to an sc_in.\n"); // Add a comment.
                }
                break;
            }
        }
        fputs(line, output); // Write the line.
    }
}

// Function to replace the sc_in signals by the new sc_signal on sensitive lists.
void replace_sc_in_on_sensitive_lists(FILE *input, FILE *output, char sc_in_name[100][100]) {
    char *line_ptr = NULL; // Pointer to the current line.
    size_t len = 0;
    char line[1000]; // Current line.
    char line_aux[1000]; // Auxiliary lines.
    int i;
    
    rewind(input); // Rewind to the beginning of the input file.
    while(getline(&line_ptr, &len, input) != -1) { // Go line by line until the end.
        strcpy(line, line_ptr); // Copy the line.
        for(i = 0; sc_in_name[i][0] != '0'; i++) { // Search into the sc_in's names informed by the user.
            if(strstr(line, "sensitive") != NULL && strstr(line, sc_in_name[i]) != NULL) { // If the line has a sensitive list and a sc_in name:
                strcpy(line_aux, sc_in_name[i]); // Get the sc_signal preffix name.
                strcat(line_aux, "_fault_target_from_in"); // Add the sc_signal suffix name.
                str_replace(line, sc_in_name[i], line_aux); // Replace the sc_in by the corresponding sc_signal.
                if(strstr(line, "// Created") == NULL) { // To ensure that the comment won't show up twice.
                    str_replace(line, "\n", " // Created by the Converter tool to get and send faults to an sc_in.\n"); // Add a comment.
                }
            }
        }
        fputs(line, output); // Write the line.
    }
}

// Function to find SC_METHOD's.
void find_sc_methods(FILE *input, FILE *output, char sc_in_name[100][100], char sc_method[100]) {
    char *line_ptr = NULL; // Pointer to the current line.
    size_t len = 0;
    char line[1000]; // Current line.
    int sc_ctor_is_present = 0; // To check if an SC_CTOR is present.
    char *sc_method_ptr = NULL; // Ponter to the sc_method string.
    
    rewind(input); // Rewind to the beginning of the input file.
    while(getline(&line_ptr, &len, input) != -1) { // Go line by line until the end.
        strcpy(line, line_ptr); // Copy the line.
        if(strstr(line, "SC_CTOR") != NULL) { // If the line starts a constructor:
            sc_ctor_is_present = 1; // Flag that the SC_CTOR is present.
        }
        if(sc_ctor_is_present && strstr(line, "SC_METHOD") != NULL) { // If the line has a sc_method:
            sc_method_ptr = extract(line, "SC_METHOD(", ");"); // Extract the name of the method.
            strcpy(sc_method, sc_method_ptr); // Put in the sc_method string.
            strcat(sc_method, "("); // Concatenate with a "(" to identify as a function.
        }
        fputs(line, output); // Write the line.
    }
    
}

// Function to replace all sc_in's informed by the user inside a SC_METHOD.
void replace_sc_in_on_sc_methods(FILE *input, FILE *output, char sc_in_name[100][100], char sc_method[100]) {
    char *line_ptr = NULL; // Pointer to the current line.
    size_t len = 0;
    char line[1000]; // Current line.
    char line_aux[1000]; // Auxiliary lines.
    int left_braces_counter = 0, right_braces_counter = 0; // To identify the '}' that ends the function.
    int i;

    rewind(input); // Rewind to the beginning of the input file.
    while(getline(&line_ptr, &len, input) != -1) { // Go line by line until the end.
        strcpy(line, line_ptr); // Copy the line.
        if(strstr(line, sc_method) != NULL) { // If the line has the sc_method:
            fputs(line, output); // Write the line.
            if(strstr(line, "{") != NULL) { // If the first '{' was found:
                left_braces_counter++; // Count the first occurrence of '{'.
            }
            while((left_braces_counter != right_braces_counter) || (left_braces_counter == 0)) { // Search until the last '}' is found.
                getline(&line_ptr, &len, input); // Go line by line of the function.
                strcpy(line, line_ptr); // Copy the line.
                for(i = 0; sc_in_name[i][0] != '0'; i++) { // Search into the sc_in's names informed by the user.
                    strcpy(line_aux, sc_in_name[i]); // Get the sc_signal preffix name.
                    strcat(line_aux, "_fault_target_from_in"); // Add the sc_signal suffix name.
                    if(strstr(line, sc_in_name[i]) != NULL && strstr(line, line_aux) == NULL) { // If the line has an unconverted sc_in name:
                        str_replace(line, sc_in_name[i], line_aux); // Replace the sc_in by the corresponding sc_signal.
                        if(strstr(line, "// Created") == NULL) { // To ensure that the comment won't show up twice.
                            str_replace(line, "\n", " // Created by the Converter tool to get and send faults to an sc_in.\n"); // Add a comment.
                        }
                        break;
                    }
                }
                left_braces_counter += count_occurrences(line, "{"); // Count the number of occurrences of '{'.
                right_braces_counter += count_occurrences(line, "}"); // Count the number of occurrences of '}'.
                if((left_braces_counter != right_braces_counter) || (left_braces_counter == 0)) { // If the last '}' was not found yet:
                    fputs(line, output); // Write the line again.
                }
            }
        }
        fputs(line, output); // Write the line.
    }
}

// Function to replace "sc_signal<sc_bv" by "sc_signal<sc_uint" on sc_signals created by the tool.
void replace_bv_by_uint_on_signals_created(FILE *input, FILE *output, char sc_in_name[100][100]) {
    char *line_ptr = NULL; // Pointer to the current line.
    size_t len = 0;
    char line[1000]; // Current line.
    int i;
    
    rewind(input); // Rewind to the beginning of the input file.
    while(getline(&line_ptr, &len, input) != -1) { // Go line by line until the end.
        strcpy(line, line_ptr); // Copy the line.
        for(i = 0; sc_in_name[i][0] != '0'; i++) { // Search into the sc_in's names informed by the user.
            if(strstr(line, "sc_signal<sc_bv<") != NULL && strstr(line, sc_in_name[i]) != NULL) { // If the line has an "sc_signal<sc_bv<" and an sc_in name:
                str_replace(line, "sc_signal<sc_bv", "sc_signal<sc_uint"); // Replace "sc_signal<sc_bv" by "sc_signal<sc_uint".
                if(strstr(line, "// Created") == NULL) { // To ensure that the comment won't show up twice.
                    str_replace(line, "\n", " // Created by the Converter tool to get and send faults to an sc_in.\n"); // Add a comment.
                }
            }
        }
        fputs(line, output); // Write the line.
    }
}

// Function to print to the user and add a comment where there's an sc_in not handled. 
void warn_about_missing_cases(FILE *input, FILE *output, char sc_in_name[100][100]) {
    char *line_ptr = NULL; // Pointer to the current line.
    size_t len = 0;
    char line[1000]; // Current line.
    int i;
    
    rewind(input); // Rewind to the beginning of the input file.
    while(getline(&line_ptr, &len, input) != -1) { // Go line by line until the end.
        strcpy(line, line_ptr); // Copy the line.
        for(i = 0; sc_in_name[i][0] != '0'; i++) { // Search into the sc_in's names informed by the user.
            if(strstr(line, sc_in_name[i]) != NULL && strstr(line, "// Created") == NULL && strstr(line, "sc_in") == NULL) { // If the line has an sc_in name but not a "// Created..." comment nor a sc_in declaration, we assume that is a not handled line.
                printf("Line not handled: %s", line); //  Print the line.
                str_replace(line, "\n", " // This line was not handled by the converter and may have to be manually handled.\n"); // Add a comment.
                break;
            }
        }
        fputs(line, output); // Write the line.
    }
}

// Function to get the list of "enum" that the user wants to convert.
void get_enum_names(char enum_name[100][100]) {
    int i;
    
    clean_matrix(enum_name);
    puts("Please inform the enum variable names you want to inject faults (type '0' to exit):");
    for(i = 0; i < 100 && enum_name[i-1][0] != '0'; i++) { // Ask name by name until the user types '0'.
        printf("enum[%d]: ", i); // Print the current enum index.
        scanf("%s", enum_name[i]); // Get the enum name.
    }
}

// Function to comment enums in the code.
void comment_enums (FILE *input, FILE *output, char enum_name[100][100]) {
    char *line_ptr = NULL; // Pointer to the current line.
    size_t len = 0;
    char line[1000]; // Current line.
    int i;
    
    rewind(input); // Rewind to the beginning of the input file.
    while(getline(&line_ptr, &len, input) != -1) { // Go line by line until the end.
        strcpy(line, line_ptr); // Copy the line.
        for(i = 0; enum_name[i][0] != '0'; i++) { // Search into the enum's names informed by the user.    
            if(strstr(line, "enum ") != NULL && strstr(line_ptr, enum_name[i]) != NULL) { // If the line has an enum informed by the user:
                fputs("//", output); // Turn the line into a comment.
                fputs(line_ptr, output); // Write the line again.
                while(strstr(line_ptr, "};") == NULL) { // Search to the end of the enum.
                    getline(&line_ptr, &len, input); // Go line by line of the enum.
                    fputs("//", output); // Turn the line into a comment.
                    fputs(line_ptr, output); // Write the line again.
                }
            } else if(i == 0) { // If the loop was executed only once:
                fputs(line_ptr, output); // Write the line as is.
            }
        }
        if(enum_name[0][0] == '0') { // If no enum was informed by the user:
            fputs(line_ptr, output); // Write the line as is.
        }
    }
}

// Function to replace sc_signal<enum> by sc_signal<sc_uint<32>>.
int replace_enums (FILE *input, FILE *output, char enum_name[100][100]) {
    char *line_ptr = NULL; // Pointer to the current line.
    size_t len = 0;
    char line[1000]; // Current line.
    char enum_under_brackets[100]; // To store the enum's name under brackets.
    int i;
    
    rewind(input); // Rewind to the beginning of the input file.
    while(getline(&line_ptr, &len, input) != -1) { // Go line by line until the end.
        strcpy(line, line_ptr); // Copy the line.
        if(strstr(line, "sc_signal<") != NULL) { // If the line has an sc_signal:
            for(i = 0; enum_name[i][0] != '0'; i++) { // Check in the list of enums.
                strcpy(enum_under_brackets, "<");
                strcat(enum_under_brackets, enum_name[i]);
                strcat(enum_under_brackets, ">");
                if(strstr(line, enum_under_brackets) != NULL) { // If is an sc_signal<enum>:
                    fputs("//", output); // Turn the line into a comment.
                    fputs(line, output); // Write the line again.
                    str_replace(line, enum_name[i], "sc_uint<32>"); // Replace the enum by sc_uint<32>.
                }
            }
        }
        fputs(line, output); // Write the line.
    }
}

// Function to replace the identifiers of the enums by numbers in the code body.
void replace_identifiers (FILE *input, FILE *output, char enum_name[100][100]) {
    char *line_ptr = NULL; // Pointer to the current line.
    size_t len = 0;
    char c; // Current character.
    char id_name[100][100]; // Array of strings to store the names of the identifiers.
    int enum_is_present = 0; // Flag to know if an enum informed by the user is present in the file.
    int i, j, n;

    rewind(input); // Rewind to the beginning of the input file.
    while(getline(&line_ptr, &len, input) != -1) { // Go line by line until the end.
        for(i = 0; enum_name[i][0] != '0'; i++) { // Check in the list of enums.
            if(strstr(line_ptr, enum_name[i]) != NULL) { // If there's an enum in that line:
                enum_is_present = 1; // Raise flag to indicate that an enum informed by the user is present.
                break; // End the loop since the flag was already raised.
            }
        }
    }
    
    rewind(input); // Rewind to the beginning of the input file.
    if(enum_is_present) {
        c = fgetc(input); // Get the first char.
        while(c != -1) { // Go char by char until the end.
            fputc(c, output); // Put the char in the output file.
            if(c == 'e') { // String found until here: "e".
                c = fgetc(input); // Get the next char.
                fputc(c, output); // Put the char in the output file.
                if(c == 'n') { // String found until here: "en".
                    c = fgetc(input); // Get the next char.
                    fputc(c, output); // Put the char in the output file.
                    if(c == 'u') { // String found until here: "enu".
                        c = fgetc(input); // Get the next char.
                        fputc(c, output); // Put the char in the output file.
                        if(c == 'm') { // String found until here: "enum".
                            c = fgetc(input); // Get the next char.
                            fputc(c, output); // Put the char in the output file.
                            if(c == ' ') { // String found until here: "enum ".
                                do {
                                    c = fgetc(input); // Get the next char.
                                    fputc(c, output); // Put the char in the output file.
                                } while(c != '{');  // To find the  beginning ('{') of the enum.
                                clean_matrix(id_name); // Clean the matrix.
                                i = 0; // Reset the identifier counter.
                                j = 0; // Reset the char counter.
                                do {
                                    do {
                                        c = fgetc(input); // Get the char inside the enum.
                                        fputc(c, output); // Put the char in the output file.
                                        if(c == ',') { // If is ',' then the identifier name ended:
                                            i++; // Increment the identifier counter.
                                            j = 0; // Reset the char counter.
                                        }
                                    } while(c == ',' || c == ' ' || c == '\n' || c == '/'); // Ignore any of these chars.
                                    if(c == '}') { // If is '}' then the enum ended:
                                        i++; // Last increment in the identifier counter.
                                        break; // Get out of the loop discharding '}'.
                                    }
                                    id_name[i][j] = c; // Put the char into the matrix.
                                    j++; // Increment the char counter.
                                } while (c != '}'); // Find the end ('}') of the enum.
                                c = fgetc(input); // Get the char ';'.
                                fputc(c, output); // Put the char ';' in the output file.
                                for(n = 0; n < i; n++) { // Create one #define for each identifier.
                                    fprintf(output, "\n#define %s %d // Created by the Converter tool to get and send faults to an enum.", id_name[n], n); // Replace the identifiers by #defines in the code, with values 0, 1, 2, 3...
                                }
                            }
                        }
                    }
                }
            }
            c = fgetc(input);
        } // Stop when reach the end of the file.
    } else { // If there's no enum in the file:
        while(getline(&line_ptr, &len, input) != -1) { // Go line by line until the end.
            fputs(line_ptr, output); // Just re-write the line.
        }
    }
}

// Function to replace occurrences like "switch(some_signal)" by "switch(some_signal.read())".
void add_read_method (FILE *input, FILE *output, char enum_name[100][100]) {
    char *line_ptr = NULL; // Pointer to the current line.
    size_t len = 0;
    char *pos = NULL; // Pointer to the current position in the line.
    char line[1000]; // Current line.
    char enum_name_with_read[100]; // String with the signal_name.read().
    int i;

    rewind(input); // Rewind to the beginning of the input file.
    while(getline(&line_ptr, &len, input) != -1) { // Go line by line until the end.
        strcpy(line, line_ptr); // Copy the line.
        if(strstr(line, "switch") != NULL) { // If the line has a switch:
            for(i = 0; enum_name[i][0] != '0'; i++) { // Search into the enum's names informed by the user. { // Check in the list of signals.
                if(strstr(line, enum_name[i]) != NULL) { // If is a switch(signal):
                    fputs("//", output); // Turn the line into a comment.
                    fputs(line, output); // Write the line again.
                    strcpy(enum_name_with_read, enum_name[i]); // Copy the signal name to an aux string.
                    strcat(enum_name_with_read, ".read()"); // Concatenate the signal name with ".read()".
                    str_replace(line, enum_name[i], enum_name_with_read); // Replace the signal by signal.read().
                    if(strstr(line, "// Created") == NULL) { // To ensure that the comment won't show up twice.
                        str_replace(line, "\n", " // Created by the Converter tool to get and send faults to an enum.\n"); // Add a comment.
                    }
                }
            }
        }
        fputs(line, output); // Write the line.
    }
}

// Function to get the list of "sc_bv" that the user wants to convert.
void get_sc_bv_names(char sc_bv_name[100][100]) {
    int i;
    
    clean_matrix(sc_bv_name);
    puts("Please inform the sc_bv variable names you want to inject faults (type '0' to exit):");
    for(i = 0; i < 100 && sc_bv_name[i-1][0] != '0'; i++) { // Ask name by name until the user types '0'.
        printf("sc_bv[%d]: ", i); // Print the current sc_bv index.
        scanf("%s", sc_bv_name[i]); // Get the sc_bv name.
    }
}

// Function to replace "sc_signal<sc_bv" by "sc_signal<sc_uint".
void replace_bv_by_uint (FILE *input, FILE *output, char sc_bv_name[100][100]) {
    char *line_ptr = NULL; // Pointer to the current line.
    size_t len = 0;
    char line[1000]; // Current line.
    int i;
    
    rewind(input); // Rewind to the beginning of the input file.
    while(getline(&line_ptr, &len, input) != -1) { // Go line by line until the end.
        strcpy(line, line_ptr); // Copy the line.
        if(strstr(line, "sc_signal<sc_bv<") != NULL) { // If the line has an sc_signal:
            for(i = 0; sc_bv_name[i][0] != '0'; i++) { // Search into the sc_bv's names informed by the user.
                if(strstr(line, sc_bv_name[i]) != NULL) { // If the line has an sc_bv name:
                    fputs("//", output); // Turn the line into a comment.
                    fputs(line, output); // Write the line again.
                    str_replace(line, "sc_signal<sc_bv", "sc_signal<sc_uint"); // Replace "sc_signal<sc_bv" by "sc_signal<sc_uint".
                    if(strstr(line, "// Created") == NULL) { // To ensure that the comment won't show up twice.
                        str_replace(line, "\n", " // Created by the Converter tool to get and send faults to an sc_uint.\n"); // Add a comment.
                    }
                }
            }
        }
        fputs(line, output); // Write the line.
    }
}

int main () {

    FILE *input = fopen("test_files/input.h", "r"); // Input file.
    FILE *temp_1 = fopen("test_files/temp_1.h", "w+"); // Temporary file.
    FILE *temp_2 = fopen("test_files/temp_2.h", "w+"); // Temporary file.
    FILE *temp_3 = fopen("test_files/temp_3.h", "w+"); // Temporary file.
    FILE *temp_4 = fopen("test_files/temp_4.h", "w+"); // Temporary file.
    FILE *temp_5 = fopen("test_files/temp_5.h", "w+"); // Temporary file.
    FILE *temp_6 = fopen("test_files/temp_6.h", "w+"); // Temporary file.
    FILE *temp_7 = fopen("test_files/temp_7.h", "w+"); // Temporary file.
    FILE *temp_8 = fopen("test_files/temp_8.h", "w+"); // Temporary file.
    FILE *temp_9 = fopen("test_files/temp_9.h", "w+"); // Temporary file.
    FILE *temp_10 = fopen("test_files/temp_10.h", "w+"); // Temporary file.
    FILE *temp_11 = fopen("test_files/temp_11.h", "w+"); // Temporary file.
    FILE *temp_12 = fopen("test_files/temp_12.h", "w+"); // Temporary file.
    FILE *temp_13 = fopen("test_files/temp_13.h", "w+"); // Temporary file.
    FILE *output = fopen("test_files/output.h", "w"); // Output file.
    char sc_in_name[100][100]; // List of sc_in's to convert.
    char enum_name[100][100]; // List of enum's to convert.
    char sc_bv_name[100][100]; // List of sc_bv's to convert.
    int sc_in_present[100] = {0}; // List of sc_in's listed that are present in the code.
    char sc_method[100];
    
    if(input == NULL) {
        puts("Input file not found.");
        return 1;
    }
    
    // TASK 1: CREATE SC_SIGNAL TO SEND FAULTS TO SC_IN.
    get_sc_in_names(sc_in_name); // Get the list of sc_in signals to convert.
    create_sc_signal_from_sc_in(input, temp_1, sc_in_name, sc_in_present); // Create the sc_signal.
    connect_sc_in_to_sc_signal(temp_1, temp_2, sc_in_name, sc_in_present); // Connect the sc_in to the sc_signal.
    replace_sc_in_on_assigns(temp_2, temp_3, sc_in_name); // Replace the sc_in by the new sc_signal during assignments.
    replace_sc_in_on_ifs(temp_3, temp_4, sc_in_name); // Replace the sc_in by the new sc_signal on if's.
    replace_sc_in_on_sensitive_lists(temp_4, temp_5, sc_in_name); // Replace the sc_in by the new sc_signal on sensitive lists.
    find_sc_methods(temp_5, temp_6, sc_in_name, sc_method); // Find SC_METHOD's to explore.
    replace_sc_in_on_sc_methods(temp_6, temp_7, sc_in_name, sc_method); // Replace the sc_in by the new sc_signal on SC_METHOD's.
    replace_bv_by_uint_on_signals_created(temp_7, temp_8, sc_in_name); // Replace "sc_signal<sc_bv" by "sc_signal<sc_uint" on sc_signals created by the tool.
    warn_about_missing_cases(temp_8, temp_9, sc_in_name); // Warn about cases not handled by the converter.
    
    // TASK 2: REPLACE ENUMS BY DEFINES.
    get_enum_names(enum_name);
    comment_enums(temp_9, temp_10, enum_name); // Step 1: Comment all the enums.
    replace_enums(temp_10, temp_11, enum_name); // Step 2: Replace enums by sc_uint<32>.
    replace_identifiers(temp_11, temp_12, enum_name); // Step 3: Replace identifiers by numbers.
    add_read_method(temp_12, temp_13, enum_name); // Step 4: Add ".read()" into switches.
    
    // TASK 3: REPLACE SC_BV BY SC_UINT.
    get_sc_bv_names(sc_bv_name); // Get the list of sc_bv signals to convert.
    replace_bv_by_uint(temp_13, output, sc_bv_name); // Replace "sc_signal<sc_bv" by "sc_signal<sc_uint".
    
    fclose(input); // Close input file.
    fclose(temp_1); // Close temporary file.
    fclose(temp_2); // Close temporary file.
    fclose(temp_3); // Close temporary file.
    fclose(temp_4); // Close temporary file.
    fclose(temp_5); // Close temporary file.
    fclose(temp_6); // Close temporary file.
    fclose(temp_7); // Close temporary file.
    fclose(temp_8); // Close temporary file.
    fclose(temp_9); // Close temporary file.
    fclose(temp_10); // Close temporary file.
    fclose(temp_11); // Close temporary file.
    fclose(temp_12); // Close temporary file.
    fclose(temp_13); // Close temporary file.
    fclose(output); // Close output file.
    remove("test_files/temp_1.h"); // Remove temporary file.
    remove("test_files/temp_2.h"); // Remove temporary file.
    remove("test_files/temp_3.h"); // Remove temporary file.
    remove("test_files/temp_4.h"); // Remove temporary file.
    remove("test_files/temp_5.h"); // Remove temporary file.
    remove("test_files/temp_6.h"); // Remove temporary file.
    remove("test_files/temp_7.h"); // Remove temporary file.
    remove("test_files/temp_8.h"); // Remove temporary file.
    remove("test_files/temp_9.h"); // Remove temporary file.
    remove("test_files/temp_10.h"); // Remove temporary file.
    remove("test_files/temp_11.h"); // Remove temporary file.
    remove("test_files/temp_12.h"); // Remove temporary file.
    remove("test_files/temp_13.h"); // Remove temporary file.
    return 0;
    
}
