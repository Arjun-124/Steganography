#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "types.h"

/*
 * Structure to store information required for
 * decoding secret data from stego image.
 */

typedef struct DecodeInfo
{
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* Output Secret File Info */
    char secret_fname[100];
    FILE *fptr_secret;
    long size_secret_file_extn;
    char extn_secret_file[5];
    char secret_data[100];
    long size_secret_file;

} DecodeInfo;


/* Read and validate decode arguments */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *DecInfo);


/* Perform decoding */
Status do_decoding(DecodeInfo *DecInfo);


/* Open input and output files */
Status open_dfiles(DecodeInfo *DecInfo);


/* Decode magic string */
Status decode_magic_string(const char *magic_string, DecodeInfo *DecInfo);


/* Decode one byte from LSB of image data */
Status decode_byte_to_lsb(char *image_buffer,unsigned char *data);


/* Decode size from LSB */
Status decode_size_to_lsb(unsigned int *data, char *imageBuffer);


/* Decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *DecInfo);


/* Decode secret file extension */
Status decode_secret_file_extn_size(DecodeInfo *DecInfo);

Status open_output_file(DecodeInfo *DecInfo);


/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *DecInfo);


/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *DecInfo);

#endif