#include <Rcpp.h>
#include <amqp.h>

/**
 * Recover amqp connection state from external pointer
 *
 * @param xptr the external pointer retrieved from R
 * @returns amqp connection state object
 */
amqp_connection_state_t_* get_connection_state(SEXP xptr);

/**
 * Free connection memory
 * 
 * @param conn connection object
 */
void amqp_finalize_connection(amqp_connection_state_t_* conn);

/**
 * Converts amqp message object to Rcpp::List
 * 
 * @param message the amqp message object
 * @returns Rcpp list object
 */
Rcpp::List rabbitr_message(amqp_message_t message);

/**
 * Converts amqp envelope object to Rcpp::List
 * 
 * @param message the amqp envelope object
 * @returns Rcpp list object
 */
Rcpp::List rabbitr_envelope(amqp_envelope_t envelope);

/**
 * Converts Rcpp string object to amqp bytes
 * 
 * @param a string to convert
 * @returns converted string to bytes
 */
amqp_bytes_t rstr_to_bytes(SEXP a);

/**
 * Converts Rcpp::List of properties 
 * to amqp message properties
 * 
 * @param r_props message properties in format of named list
 * @returns amqp propeties object
 */
amqp_basic_properties_t convert_properties(Rcpp::List r_props);

/**
 * Checks amqp_rpc_reply_t objects for exceptions
 */
void check_errors(amqp_rpc_reply_t reply);
