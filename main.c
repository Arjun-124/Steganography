#include <stdio.h>
#include<string.h>
#include "encode.h"
#include"decode.h"
#include "types.h"
/**
 * @brief Main function of the Image Steganography project.
 *
 * Checks the command-line operation and performs either
 * encoding or decoding based on the user input.
 *
 * Encoding:
 *     ./a.out -e source.bmp secret.txt
 *
 * Decoding:
 *     ./a.out -d stego.bmp
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 *
 * @return e_success if the operation is completed successfully.
 * @return e_failure if validation or encoding/decoding fails.
 */

OperationType check_operation_type(char *);

int main(int argc, char *argv[]) // .aout -e <source_file> <secret_data_file> <optional>
{
    //step 1 -> check check_operation_type(argv[1]) is returning e_encode or not
        // yes -> EncodeInfo encInfo;
            // check read_and_validate_encode_args(argv, &encInfo) is returning e_success or e_failure
                // failure -> print error msg and stop
                // success -> 
                // check do_encoding(&encInfo) is returning e_success or e_failure
                                    // failure -> print error msg and stop
                                    // success -> print success msg and stop
                
    if((check_operation_type(argv[1])==e_encode))
    {
        EncodeInfo encInfo;
        if(read_and_validate_encode_args(argv, &encInfo)!=e_success)
        {
            printf("enter the validate data \n");
            return e_failure;
        }
        if(do_encoding(&encInfo)!=e_success)
        {
            return e_failure;
        }
        return e_success;
        
    }
    else if((check_operation_type(argv[1])==e_decode))
    {
        DecodeInfo DecInfo;
        if(read_and_validate_decode_args(argv, &DecInfo)!=e_success)
        {
            printf("enter the validate data \n");
            return e_failure;
        }
        if(do_decoding(&DecInfo)!=e_success)
        {
            return e_failure;
        }
        return e_success;
        
    }
}

OperationType check_operation_type(char *symbol)
{
    //step 1 -> check symbol is -e or not
        // yes -> return e_encode

    if((strcmp(symbol,"-e"))==0)
    {
        return e_encode;
    }
    else if((strcmp(symbol,"-d"))==0)
    {
        return e_decode;
    }
    else{
        return e_unsupported;
    }
    
    // step 2 -> check symbol is -d or not
        // yes -> return e_decode

    // return e_unsupported
}
