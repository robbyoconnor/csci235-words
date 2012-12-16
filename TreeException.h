/**
 * Title: TreeException.h
 * Name: Robert O'Connor
 * Created: 12/12/12
 * Description: Exception class for the BST implementation.
 *
 */

#include <stdexcept>
#include <string>

using namespace std;

/** @class TreeException
 *  Exception class for the ADT binary tree */
class TreeException : public logic_error
{
public:
   TreeException(const string & message = "")
                        : logic_error(message.c_str())
   { }  // end constructor
}; // end TreeException
