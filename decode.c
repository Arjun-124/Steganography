#include <stdio.h>
#include<string.h>
#include "decode.h"
#include "types.h"
#include "common.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *DecInfo)
{
    if(strlen(argv[2])>=4)         // check argv[2] is greater than 4 or not
    {
        if (strcmp(argv[2] + strlen(argv[2]) - 4, ".bmp") == 0)  //comapring it is  haveing b,p file or not
        {
            DecInfo->stego_image_fname=argv[2];
        }
        else{
        return e_failure;
        }
    }
    // if(argv[3])
    // {
    //     DecInfo->secret_fname=strtok(argv[3],".");
    // }
    strcpy(DecInfo->secret_fname, "output");
    return e_success;
    
}
Status open_dfiles(DecodeInfo *DecInfo)
{
    DecInfo->fptr_stego_image = fopen(DecInfo->stego_image_fname, "rb");// opening files in read mode
    // Do Error handling
    if (DecInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", DecInfo->stego_image_fname);

        return e_failure;
    }
    // No failure return e_success
    return e_success;
}
Status decode_magic_string(const char *magic_string, DecodeInfo *DecInfo)
{
    char buf[8];
    unsigned char data;
    for(int i=0;i<strlen(magic_string);i++)                  // decoding magic string
    {
        if(fread(buf,sizeof(char),8,DecInfo->fptr_stego_image)!=8)
        {
            return e_failure;
        }
        if(decode_byte_to_lsb(buf,&data)!=e_success)         // calling decode_byte_to_lsb to decode
        {
            return e_failure;
        }
        if(data!=magic_string[i])          //if data is not equal then magic string is mismatched
        {
            printf("magic string is mismatched");
            return e_failure;
        }
    }
    printf("magic string is matched\n");
    return e_success;


}
Status decode_byte_to_lsb( char *image_buffer,unsigned char *data)//write logic to decode_byte_to_lsb
{
    *data=0;

    for(int i = 0; i < 8; i++)
    {
        *data= (*data << 1) | (image_buffer[i] & 1);
    }
    return e_success;
}
Status decode_secret_file_size( DecodeInfo *DecInfo)//decoding the secert file size
{
    char buf[32];
    unsigned int data;
    if((fread(buf,sizeof(char),32,DecInfo->fptr_stego_image))!=32)
    {
        return e_failure;
    }
    if(decode_size_to_lsb(&data, buf)!=e_success)
    {
        return e_failure;
    }
    DecInfo->size_secret_file=data;
    return e_success;

}
Status decode_size_to_lsb(unsigned int *data, char *imageBuffer)
{
    // write logic to decode the int
    *data=0;
    for (int i = 0; i < 32; i++)
    {
        
    *data = (*data <<1) | (imageBuffer[i] & 1);
    }
    return e_success;

}
Status decode_secret_file_extn(DecodeInfo *DecInfo)
{
    char buf[8];
    unsigned char data;
    for(int i=0;i<DecInfo->size_secret_file_extn;i++)
    {
         fread(buf,sizeof(char),8,DecInfo->fptr_stego_image);
         if(decode_byte_to_lsb(buf,&data)!=e_success)
         {
            return e_failure;
         }
         DecInfo->extn_secret_file[i]=data;
    }
     DecInfo->extn_secret_file[DecInfo->size_secret_file_extn]='\0';
    // printf("Decoded extension = %s\n", DecInfo->extn_secret_file);
    //  printf("Extension size = %u\n", data);
    return e_success;

}
Status open_output_file(DecodeInfo *DecInfo)
{
    strcat(DecInfo->secret_fname, DecInfo->extn_secret_file);

    DecInfo->fptr_secret = fopen(DecInfo->secret_fname, "wb");

    if(DecInfo->fptr_secret == NULL)
    {
        return e_failure;
    }

    return e_success;
}
Status decode_secret_file_extn_size(DecodeInfo *DecInfo)
{
    char buf[32];
    unsigned int data;
    if((fread(buf,sizeof(char),32,DecInfo->fptr_stego_image))!=32)
    {
        return e_failure;
    }
    if(decode_size_to_lsb(&data,buf)!=e_success)
    {
        return e_failure;
    }
    DecInfo->size_secret_file_extn = data;
    
    return e_success;
    
}
Status decode_secret_file_data(DecodeInfo *DecInfo)           //decode secret file data
{
    char buf[8];
    unsigned char data;
    for(int i=0;i<DecInfo -> size_secret_file;i++)
       {
        if(fread(buf, 1, 8, DecInfo->fptr_stego_image) != 8)
        {
            return e_failure;
        }
        if(decode_byte_to_lsb(buf,&data)!=e_success)
        {
            return e_failure;
        }
        if(fwrite(&data,1,1,DecInfo->fptr_secret)!=1)
        {
            return e_failure;
        }
    }
    return e_success;
}
Status do_decoding(DecodeInfo *DecInfo)    //decoding step by step by calling the functions
{
    if(open_dfiles(DecInfo)!=e_success)
    {
        printf("error in the file opening\n");
        return e_failure;
    }
    fseek(DecInfo->fptr_stego_image, 54, SEEK_SET);
    if(decode_magic_string(MAGIC_STRING, DecInfo)!=e_success)
    {
        return e_failure;
    }
    if(decode_secret_file_extn_size(DecInfo)!=e_success)
    {
        return e_failure;
    }
    if(decode_secret_file_extn(DecInfo)!=e_success)
    {
        return e_failure;
    }
    if(open_output_file(DecInfo)!=e_success)
    {
        return e_failure;
    }
    if (decode_secret_file_size(DecInfo)!=e_success)
    {
        return e_failure;
    }
    if (decode_secret_file_data(DecInfo)!=e_success)
    {
        return e_failure;
    }
    printf("sucessfull");
    return e_success;
    
}