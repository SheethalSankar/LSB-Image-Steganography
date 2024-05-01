#ifndef ENCODE_H
#define ENCODE_H

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

typedef struct _EncodeInfo
{
    /* Source Image info */
    char *src_image_fname; //name of source image file
    FILE *fptr_src_image;  //address of opened file
    uint image_capacity; ///store size of beautiful.bmp file
    //uint bits_per_pixel;
    //char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char *secret_fname;   //soure data file name
    FILE *fptr_secret;    //Address of souce date file(file pointer)
    char extn_secret_file[MAX_FILE_SUFFIX];  //secret file extension
    char secret_data[MAX_SECRET_BUF_SIZE];//macro check above size of extension is max 4, to store extension of secret 
    long size_secret_file; //size of secret file (source date) no of characters in secret data

    /* Stego Image Info */
    char *stego_image_fname; //name of output image file
    FILE *fptr_stego_image; //address of output image file(file pointer)

} EncodeInfo;


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
int get_file_size(FILE *fptr);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* Store Magic String */
Status encode_magic_string(char *magic_string, FILE *fptr_src_image,FILE *fptr_stego_image);

/* Encode secret file extenstion */
Status encode_secret_file_extn(char *extn,FILE *fptr_src_image,FILE *fptr_stego_image);
/*ENcode secret file extension size*/
Status encode_secret_file_extn_size(int extn_size,FILE *fptr_src_image,FILE *fptr_stego_image);//extension size is stored in array using strlen find len
/* Encode secret file size */
Status encode_secret_file_size(int file_size,FILE *fptr_src_image,FILE *fptr_stego_image);

/* Encode secret file data*/ // Status encode_secret_file_data(EncodeInfo *encInfo);
Status encode_secret_file_data(FILE *fptr_secret,FILE *fptr_src_image,FILE *fptr_stego_image);
/* Encode function, which does the real encoding */
Status encode_data_to_image(char *data, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(unsigned char data, char *image_buffer);

/* Encode a size into LSB of image data array */
Status encode_size_to_lsb(unsigned int size, char *image_buffer);

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

#endif
