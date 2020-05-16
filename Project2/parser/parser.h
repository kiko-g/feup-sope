
#include "../client/ClientArgs.h"
#include "../server/ServerArgs.h"


/**
 * @brief Parses the command line arguments of the client process (U)
 * 
 * @param argc number of arguments
 * @param argv array of argument string
 * @param clientArgs struct that holds the parsed results
 * @return int 0 if successful, anything else if otherwise
 */
int parse_client_args(int argc, char *argv[], struct ClientArgs * clientArgs);

/**
 * @brief Parses the command line arguments of the server process (Q)
 * 
 * @param argc number of arguments
 * @param argv array of argument string
 * @param serverArgs struct that holds the parsed results
 * @return int 0 if successful, anything else if otherwises
 */
int parse_server_args(int argc, char *argv[],struct ServerArgs * serverArgs);