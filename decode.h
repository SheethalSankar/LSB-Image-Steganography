#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *src_image_fname; //name of source image file
    FILE *fptr_src_image;  //address of opened file
    uint image_capacity; ///store size of beautiful.bmp file
    //uint bits_per_pixel;
    //char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char secret_fname[20];   //soure data file name
    FILE *fptr_secret;    //Address of souce date file(file pointer)
    char extn_secret_file[MAX_FILE_SUFFIX];  //secret file extension
    char secret_data[MAX_SECRET_BUF_SIZE];//macro check above size of extension is max 4, to store extension of secret 
    long size_secret_file; //size of secret file (source date) no of characters in secret data

    /* Stego Image Info */
    char *stego_image_fname; //name of output image file
    FILE *fptr_stego_image; //address of output image file(file pointer)

} DecodeInfo;


/* Decoding function prototype */


/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_dec_files(DecodeInfo *decInfo);


/* Store Magic String */
Status decode_magic_string(FILE *fptr_stego_image);

/* Encode secret file extenstion */
Status decode_secret_file_extn(FILE *fptr_stego_image,DecodeInfo *decInfo);
/*ENcode secret file extension size*/
Status decode_secret_file_extn_size(FILE *fptr_stego_image);//extension size is stored in array using strlen find len
/* Encode secret file size */
Status decode_secret_file_size(FILE *fptr_stego_image);

/* Encode secret file data*/
Status decode_secret_file_data(FILE *fptr_secret,FILE *fptr_stego_image);

/* Encode function, which does the real encoding */
Status decode_data_from_image(FILE *fptr_stego_image,char dec_data[],int len);

/* Encode a byte into LSB of image data array */
Status decode_byte_from_lsb(char *data, char *image_buffer);

Status decode_size_from_lsb(char buffer[],int *dec_size);

Status dec_open_output_file(DecodeInfo *decInfo);


#endif
