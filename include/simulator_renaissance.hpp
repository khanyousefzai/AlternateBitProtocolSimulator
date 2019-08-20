/** /brief This header file contains functional prototypes for simulator output modifications.
 * 
 * Function output_file_evolution modifies ABP simulator or test sets output file
 * to a user-friendly readable. Whereas, function output_time_statistics calculates the time delay statistics between 
 * the sender and receiver correspondence, and also calculates the max, min and avg time
 * delay. It uses output_file_evolution output file as an input file and generates another 
 * output file which contains the discussed statistics.
 * 
 * The format of ouput file generated from both the functions is in csv.
 * 
 * Sample output from output_file_evolution function's file is shown below:
 * 
 * Time,Value,Port,Component
 * 00:00:10:000,5,out,generator_con
 * 00:00:20:000,1,packetSentOut,sender1
 * 00:00:20:000,11,dataOut,sender1
 * 00:00:23:000,11,out,subnet1
 * 00:00:33:000,1,out,receiver1
 * 
 * Sample output from output_time_statistics function's file is shown below:
 * Value,Time
 * 11,0:0:16
 * 20,0:0:16
 * 31,0:0:46
 * max,0:0:46
 * min,0:0:16
 * avg,0:0:19:0
 * 
 */
/**
 * Hamza Sadruddin
 * Saad Hasan
 * Carleton University
 */
#ifndef __SIMULATOR_RENAISSANCE_HPP_
#define __SIMULATOR_RENAISSANCE_HPP_

using namespace std;

/**
 * @brief          Function modifies the input file to more readable format 
 *
 * @param[in]      input_file   (type string) The input file is the ouput file of ABP or test sets
 * @param[in]      output_file  (type string) The output file is modified to more readable format
 */
void output_file_evolution(char *, char *);

/**
 * @brief      String splitter
 * Function returns the  vector string after parsing the input string on the basis
 * of the given delimiter.
 *
 * @param[in]  s      String to be split
 * @param[in]  delim  The delimiter(eg. ",", ":") to be checked for
 *
 * @return     result String type vector
 */
vector<string> split(const string &, char *);

/**
 * @brief      Time delay statistics
 * The function calculates the time delay statistics between the sender and receiver correspondence
 *
 * @param      input_file   The input file is the ouput file of abp_mod_output function
 * @param      output_file  The output file is file containing statistics of time delay
 */
void output_time_statistics(char *, char *);



#endif /**< __SIMULATOR_RENAISSANCE_HPP_ */
