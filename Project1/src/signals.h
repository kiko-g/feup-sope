#include <stdbool.h>

/**
 * @brief Install the signal handlers
 * 
 */
void install_sigactions(); 

/**
 * @brief Handler for SIGINT
 * 
 * @param signo signal number
 */
void sigint_handler(int signo);

/**
 * @brief Handler for SIGCONT
 * 
 * @param signo signal number
 */
void sigcont_handler(int signo);

/**
 * @brief Handler for SIGTERM
 * 
 * @param signo signal number
 */
void sigterm_handler(int signo);

/**
 * @brief Asks the user to terminate the program,
 * using async-friendly write and read
 * 
 * @return True if to terminate, false if otherwise
 */
bool askTerminateProgram();
