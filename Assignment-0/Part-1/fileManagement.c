 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <sys/stat.h>
 #include <unistd.h>
 #include <ctype.h>
 
 #ifdef _WIN32
     #include <conio.h>
     #include <windows.h>
     #define TTY_DEVICE "CON"
 #else
     #include <termios.h>
     #include <fcntl.h>
     #define TTY_DEVICE "/dev/tty"
 #endif
 
 #define MAX_LINE_LENGTH 1024
 #define MAX_COMMAND_LENGTH 1024
 #define MAX_HISTORY 100
 
 // Command history structure
 typedef struct {
     char commands[MAX_HISTORY][MAX_COMMAND_LENGTH];
     int count;
     int current_index;
 } CommandHistory;
 
 CommandHistory history = {0};
 
 // Function prototypes
 int create_directory(const char *dir_name);
 void create_file(const char *path);
 void append_to_file(const char *path, const char *text);
 void print_file(const char *path);
 void print_first_n(const char *path, int n);
 int count_lines(const char *path);
 void print_last_n(const char *path, int n);
 void add_to_history(const char *command);
 void print_history(void);
 void clear_history(void);
 char* get_command_with_history(void);
 
 // Helper: create directory
 int create_directory(const char *dir_name) {
 #ifdef _WIN32
     return mkdir(dir_name);
 #else
     return mkdir(dir_name, 0755);
 #endif
 }
 
 // Helper: create empty file
 void create_file(const char *path) {
     FILE *fp = fopen(path, "w");
     if (fp) fclose(fp);
 }
 
 // Append a string to file
 void append_to_file(const char *path, const char *text) {
     FILE *fp = fopen(path, "a");
     if (fp) {
         fprintf(fp, "%s", text);
         fclose(fp);
     }
 }
 
 // Print entire file content
 void print_file(const char *path) {
     FILE *fp = fopen(path, "r");
     if (!fp) return;
     int c;
     while ((c = fgetc(fp)) != EOF) {
         printf("%c", c);
     }
     fclose(fp);
 }
 
 // Print first n lines
 void print_first_n(const char *path, int n) {
     FILE *fp = fopen(path, "r");
     if (!fp) return;
     char line[MAX_LINE_LENGTH];
     int count = 0;
     while (count < n && fgets(line, sizeof(line), fp)) {
         printf("%s", line);
         count++;
     }
     fclose(fp);
 }
 
 // Count total lines
 int count_lines(const char *path) {
     FILE *fp = fopen(path, "r");
     if (!fp) return 0;
     int lines = 0, c;
     while ((c = fgetc(fp)) != EOF) {
         if (c == '\n') lines++;
     }
     fclose(fp);
     return lines;
 }
 
 // Print last n lines
 void print_last_n(const char *path, int n) {
     int total = count_lines(path);
     int start = (total > n) ? total - n : 0;
     FILE *fp = fopen(path, "r");
     if (!fp) return;
     char line[MAX_LINE_LENGTH];
     int count = 0;
     while (fgets(line, sizeof(line), fp)) {
         if (count++ >= start) {
             printf("%s", line);
         }
     }
     fclose(fp);
 }
 
 // Add command to history
 void add_to_history(const char *command) {
     if (strlen(command) == 0) return;
     
     // Don't add duplicate consecutive commands
     if (history.count > 0 && 
         strcmp(history.commands[(history.count - 1) % MAX_HISTORY], command) == 0) {
         history.current_index = history.count;
         return;
     }
     
     strncpy(history.commands[history.count % MAX_HISTORY], command, MAX_COMMAND_LENGTH - 1);
     history.commands[history.count % MAX_HISTORY][MAX_COMMAND_LENGTH - 1] = '\0';
     history.count++;
     history.current_index = history.count;
 }
 
 // Print command history
 void print_history(void) {
     if (history.count == 0) {
         printf("No commands in history.\n");
         return;
     }
     
     int start = (history.count > MAX_HISTORY) ? history.count - MAX_HISTORY : 0;
     int display_count = (history.count > MAX_HISTORY) ? MAX_HISTORY : history.count;
     
     for (int i = 0; i < display_count; i++) {
         int index = (start + i) % MAX_HISTORY;
         printf("%d: %s\n", start + i + 1, history.commands[index]);
     }
 }
 
 // Clear command history
 void clear_history(void) {
     history.count = 0;
     history.current_index = 0;
     printf("Command history cleared.\n");
 }
 
 #ifdef _WIN32
 // Windows implementation for command input with history
 char* get_command_with_history(void) {
     static char buffer[MAX_COMMAND_LENGTH];
     int pos = 0;
     int temp_index = history.current_index;
     int ch;
     
     memset(buffer, 0, sizeof(buffer));
     
     while (1) {
         ch = _getch();
         
         if (ch == '\r') { // Enter key
             buffer[pos] = '\0';
             printf("\n");
             return buffer;
         }
         else if (ch == '\b') { // Backspace
             if (pos > 0) {
                 pos--;
                 printf("\b \b");
                 buffer[pos] = '\0';
             }
         }
         else if (ch == 3) { // Ctrl+C
             pos = 0;
             memset(buffer, 0, sizeof(buffer));
             printf("\n^C\nEnter command:");
         }
         else if (ch == 224) { // Extended key prefix
             ch = _getch();
             if (ch == 72) { // Up arrow
                 if (temp_index > 0) {
                     temp_index--;
                     // Clear current line
                     while (pos > 0) {
                         printf("\b \b");
                         pos--;
                     }
                     // Copy from history
                     strcpy(buffer, history.commands[temp_index % MAX_HISTORY]);
                     pos = strlen(buffer);
                     printf("%s", buffer);
                 }
             }
             else if (ch == 80) { // Down arrow
                 if (temp_index < history.count) {
                     temp_index++;
                     // Clear current line
                     while (pos > 0) {
                         printf("\b \b");
                         pos--;
                     }
                     if (temp_index == history.count) {
                         memset(buffer, 0, sizeof(buffer));
                         pos = 0;
                     } else {
                         strcpy(buffer, history.commands[temp_index % MAX_HISTORY]);
                         pos = strlen(buffer);
                         printf("%s", buffer);
                     }
                 }
             }
         }
         else if (ch == 16) { // Ctrl+P (previous)
             if (temp_index > 0) {
                 temp_index--;
                 while (pos > 0) {
                     printf("\b \b");
                     pos--;
                 }
                 strcpy(buffer, history.commands[temp_index % MAX_HISTORY]);
                 pos = strlen(buffer);
                 printf("%s", buffer);
             }
         }
         else if (ch == 14) { // Ctrl+N (next)
             if (temp_index < history.count) {
                 temp_index++;
                 while (pos > 0) {
                     printf("\b \b");
                     pos--;
                 }
                 if (temp_index == history.count) {
                     memset(buffer, 0, sizeof(buffer));
                     pos = 0;
                 } else {
                     strcpy(buffer, history.commands[temp_index % MAX_HISTORY]);
                     pos = strlen(buffer);
                     printf("%s", buffer);
                 }
             }
         }
         else if (ch >= 32 && ch <= 126 && pos < MAX_COMMAND_LENGTH - 1) { // Printable characters
             buffer[pos] = ch;
             pos++;
             printf("%c", ch);
         }
     }
 }
 #else
 // Unix/Linux implementation for command input with history
 char* get_command_with_history(void) {
     static char buffer[MAX_COMMAND_LENGTH];
     int pos = 0;
     int temp_index = history.current_index;
     int ch;
     
     // Set terminal to raw mode
     struct termios old_termios, new_termios;
     tcgetattr(STDIN_FILENO, &old_termios);
     new_termios = old_termios;
     new_termios.c_lflag &= ~(ICANON | ECHO);
     tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
     
     memset(buffer, 0, sizeof(buffer));
     
     while (1) {
         ch = getchar();
         
         if (ch == '\n' || ch == '\r') { // Enter key
             buffer[pos] = '\0';
             printf("\n");
             break;
         }
         else if (ch == 127 || ch == '\b') { // Backspace
             if (pos > 0) {
                 pos--;
                 printf("\b \b");
                 buffer[pos] = '\0';
             }
         }
         else if (ch == 3) { // Ctrl+C
             pos = 0;
             memset(buffer, 0, sizeof(buffer));
             printf("\n^C\nEnter command:");
         }
         else if (ch == 27) { // Escape sequence
             ch = getchar();
             if (ch == '[') {
                 ch = getchar();
                 if (ch == 'A') { // Up arrow
                     if (temp_index > 0) {
                         temp_index--;
                         // Clear current line
                         while (pos > 0) {
                             printf("\b \b");
                             pos--;
                         }
                         strcpy(buffer, history.commands[temp_index % MAX_HISTORY]);
                         pos = strlen(buffer);
                         printf("%s", buffer);
                     }
                 }
                 else if (ch == 'B') { // Down arrow
                     if (temp_index < history.count) {
                         temp_index++;
                         // Clear current line
                         while (pos > 0) {
                             printf("\b \b");
                             pos--;
                         }
                         if (temp_index == history.count) {
                             memset(buffer, 0, sizeof(buffer));
                             pos = 0;
                         } else {
                             strcpy(buffer, history.commands[temp_index % MAX_HISTORY]);
                             pos = strlen(buffer);
                             printf("%s", buffer);
                         }
                     }
                 }
             }
         }
         else if (ch == 16) { // Ctrl+P
             if (temp_index > 0) {
                 temp_index--;
                 while (pos > 0) {
                     printf("\b \b");
                     pos--;
                 }
                 strcpy(buffer, history.commands[temp_index % MAX_HISTORY]);
                 pos = strlen(buffer);
                 printf("%s", buffer);
             }
         }
         else if (ch == 14) { // Ctrl+N
             if (temp_index < history.count) {
                 temp_index++;
                 while (pos > 0) {
                     printf("\b \b");
                     pos--;
                 }
                 if (temp_index == history.count) {
                     memset(buffer, 0, sizeof(buffer));
                     pos = 0;
                 } else {
                     strcpy(buffer, history.commands[temp_index % MAX_HISTORY]);
                     pos = strlen(buffer);
                     printf("%s", buffer);
                 }
             }
         }
         else if (ch >= 32 && ch <= 126 && pos < MAX_COMMAND_LENGTH - 1) { // Printable characters
             buffer[pos] = ch;
             pos++;
             printf("%c", ch);
         }
     }
     
     // Restore terminal mode
     tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
     return buffer;
 }
 #endif
 
 int main() {
     char dir[256], content_file[512], logs_file[512];
     char *command;
     char input_line[MAX_LINE_LENGTH];
     int n;
 
     pid_t pid = getpid();
     sprintf(dir, "folder_%d", pid);
     create_directory(dir);
 
     sprintf(content_file, "%s/content.txt", dir);
     sprintf(logs_file, "%s/logs.txt", dir);
 
     create_file(content_file);
     create_file(logs_file);
 
     while (1) {
         printf("Enter command:");
         command = get_command_with_history();
         
         if (strlen(command) == 0) continue;
 
         // Add to history (except for HISTORY and CLEAR commands)
         if (strcmp(command, "HISTORY") != 0 && strcmp(command, "CLEAR") != 0) {
             add_to_history(command);
         }
 
         // Reject if command starts with space
         if (command[0] == ' ') {
             printf("Unknown command.\n");
             continue;
         }
 
         if (strcmp(command, "INPUT") == 0) {
             append_to_file(logs_file, "INPUT\n");
             printf("INPUT -> ");
             fgets(input_line, sizeof(input_line), stdin);
             // Remove newline if present
             input_line[strcspn(input_line, "\n")] = 0;
             append_to_file(content_file, input_line);
             append_to_file(content_file, "\n");
         }
         else if (strcmp(command, "PRINT") == 0) {
             append_to_file(logs_file, "PRINT\n");
             print_file(content_file);
         }
         else if (sscanf(command, "FIRST %d", &n) == 1 &&
                  strncmp(command, "FIRST ", 6) == 0 && n > 0) {
             // Check for exactly one space between FIRST and number
             char *space_pos = strchr(command, ' ');
             if (space_pos && *(space_pos + 1) != ' ' && strchr(space_pos + 1, ' ') == NULL) {
                 char log_entry[64];
                 sprintf(log_entry, "FIRST %d\n", n);
                 append_to_file(logs_file, log_entry);
                 print_first_n(content_file, n);
             } else {
                 printf("Unknown command.\n");
             }
         }
         else if (sscanf(command, "LAST %d", &n) == 1 &&
                  strncmp(command, "LAST ", 5) == 0 && n > 0) {
             // Check for exactly one space between LAST and number
             char *space_pos = strchr(command, ' ');
             if (space_pos && *(space_pos + 1) != ' ' && strchr(space_pos + 1, ' ') == NULL) {
                 char log_entry[64];
                 sprintf(log_entry, "LAST %d\n", n);
                 append_to_file(logs_file, log_entry);
                 print_last_n(content_file, n);
             } else {
                 printf("Unknown command.\n");
             }
         }
         else if (sscanf(command, "LOG %d", &n) == 1 &&
                  strncmp(command, "LOG ", 4) == 0 && n > 0) {
             // Check for exactly one space between LOG and number
             char *space_pos = strchr(command, ' ');
             if (space_pos && *(space_pos + 1) != ' ' && strchr(space_pos + 1, ' ') == NULL) {
                 print_last_n(logs_file, n);
             } else {
                 printf("Unknown command.\n");
             }
         }
         else if (strcmp(command, "HISTORY") == 0) {
             print_history();
         }
         else if (strcmp(command, "CLEAR") == 0) {
             clear_history();
         }
         else if (strcmp(command, "STOP") == 0) {
             append_to_file(logs_file, "STOP\n");
             break;
         }
         else {
             printf("Unknown command.\n");
         }
     }
 
     return 0;
 }
