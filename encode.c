#include<stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include <string.h>
int secret_file_size;
/* Function Definitions */
Status read_and_validate_encode_args(char *argv[],EncodeInfo *encInfo)
{
        //checking the argv[2] is a .bmp or not
	if(!strcmp(strstr(argv[2],"."),".bmp"))
	{
	        //if yes storing into src_img_fname
		encInfo->src_image_fname=argv[2];
	}
	else
	{
		printf("INFO:Read and validation failed\n\n");
		return e_failure;
	}
	//checking the argv[3] is .txt or not
	if(!strcmp(strstr(argv[3],"."),".txt"))
	{       //if yes storing in secret_fname
		encInfo->secret_fname= argv[3];
		//And storing it into extn_secret_file
		strcpy(encInfo->extn_secret_file, ".txt");
	}
	else
	{
		printf("INFO:Read and validation failed\n\n");
		return e_failure;
	}
	//checking that the argv[4] NULL or not
	if(argv[4]!=NULL)
	{
                //if argv[4] not NULL,checking its a .bmp or not	    
		if(!strcmp(strstr(argv[4],"."),".bmp"))
		{
		        //if it is a .bmp,store it into stego_image_fname
			encInfo->stego_image_fname=argv[4];
			printf("INFO:Read and validation is Successfully executed");
			return e_success;
		}

	}
	else
	{
	        //if argv[4] is NULL,create a file 
		char *newfilename="Output.bmp";
		encInfo->stego_image_fname=newfilename;
		printf("\nINFO:Output file not mentioned.Default file added\n\n");
		printf("INFO:Read and validation executed successfully\n\n");
	}
	return e_success;
}
//function defnition
Status do_encoding(EncodeInfo *encInfo)
{
        //checking the output of open_files function
	if(open_files(encInfo)==e_success)
	{
		printf("Info:Files opened successfully\n\n");
	}
	else
	{
		printf("Info:files opening failed\n\n");
		return e_failure;
	}
	//checking the output of check_capacity
	if(check_capacity(encInfo)==e_success)
	{
		printf("INFO:Enough space available\n\n");
	}
	else
	{
		printf("INFO:Not enough space to encode\n\n");
		return e_failure;;
	}
	//checking the copy_bmp_header
	if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
	{
		printf("INFO:Header copied successfully\n\n");
	}
	else
	{
		printf("INFO:Header not copied\n\n");
		return e_failure;
	}
	//checking the output of encode_magic_string funtion
	if(encode_magic_string(MAGIC_STRING,encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
	{
		printf("INFO:Magic string encoded successfully\n\n");
	}
	else
	{
		printf("INFO:Magic string not encoded\n\n");
		return e_failure;
	}
	//cheking the output of encode_secret_file_extn_size
	if(encode_secret_file_extn_size(strlen(encInfo -> extn_secret_file),encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
	{
		printf("INFO:Secret file extn size is Encoded successfully\n\n");
	}
	else
	{
		printf("INFO:Secret file not encoded\n\n");
		return e_failure;
	}
	//checking the encode_secret_file_extn
	if(encode_secret_file_extn(encInfo -> extn_secret_file,encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
	{
		printf("INFO:Secret file extension encoded successfully\n\n");
	}
	else
	{
		printf("INFO:Secret file extension not encoded\n\n");
		return e_failure;
	}
        //checking encode_secret_file_size
	if(encode_secret_file_size(secret_file_size,encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
	{
		printf("INFO:Secret file size encoded successfully\n\n");
	}
	else
	{
		printf("INFO:Secret file size not encoded\n\n");
		return e_failure;
	}
	//checking the encode_secret_file_data
	if(encode_secret_file_data(encInfo->fptr_secret,encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
	{
		printf("INFO:Secret file data encoded successfully\n\n");
	}
	else
	{
		printf("INFO:Secret file data not encoded\n\n");
		return e_failure;
	}
	//checkin the function copy_remaining_img_data
	if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
	{
		printf("INFO:Coping remaining data is successfully completed\n\n");
	        printf("\nENCODING COMPLETED\n");
		return e_success;
	}
	else
	{
		printf("INFO:Coping remaining data failed\n\n");
		return e_failure;
	}
}
//check_caapacity function defnition
Status check_capacity(EncodeInfo *encInfo)
{
	//find the size of source image and then  store it into image_capacity
	encInfo->image_capacity= get_image_size_for_bmp(encInfo->fptr_src_image);
	secret_file_size=get_file_size(encInfo->fptr_secret) -1;
	int size=(strlen(MAGIC_STRING)*8)+32+(strlen(encInfo->extn_secret_file))*8+secret_file_size+(secret_file_size*8);
	//checking the size of source image is greater than the size
	if(encInfo->image_capacity > size)
	{
		return e_success;;
	}
	else
	{
		return e_failure;
	}
}
//copy_bmp_header function defnition
Status copy_bmp_header(FILE *fptr_src_image,FILE *fptr_stego_image)
{
	char buffer[54];
	//rewind the source image file pointer
	rewind(fptr_src_image);
	//read 54 byte of data from src image
	fread(buffer,54 ,1, fptr_src_image);
	//write 54 byte of data into stego_image
	if(fwrite(buffer,1, 54, fptr_stego_image)==54)
	{
		return e_success;
	}
}
//encode_magic_string function defnition
Status encode_magic_string(char *magic_string,FILE *fptr_src_image,FILE *fptr_stego_image)
{
	//first data to image then we have to encode .txt
	int res=encode_data_to_image(magic_string,fptr_src_image,fptr_stego_image);
	if(res==e_success)
	{
		return e_success;
	}


}
//encode_data to_image function defnition
Status encode_data_to_image(char *data,FILE *fptr_src_image,FILE *fptr_stego_image)
{
	char image_buffer[8];
	int i=0,flag=0;
	//checking the data value
	while(data[i]!='\0')
	{       //reading 8 byte of data from src_image and then store that in image_buffer
		fread(image_buffer,8,1,fptr_src_image);
		//function calling
		encode_byte_to_lsb(data[i],image_buffer);
		fwrite(image_buffer, 1,8,fptr_stego_image);
		
		i++;
	}
	return e_success;

}
//function defnition of encode_byte to lsb
Status encode_byte_to_lsb(unsigned char ch,char *buffer)
{
	unsigned char bit,clear;
	int i;
	//Encoding
	for(i=0;i<=7;i++)
	{
	        //get a bit from the ch
		bit=(ch&(1<<i))>>i;
		//clear the lsb from buffer[i]
		clear=buffer[i]&(~(1));
		//replace lsb of buffer[i] with bit
		buffer[i]=clear|bit;
	}
}
//encode_secret_file_extn_size function defnition
Status encode_secret_file_extn_size(int extn_size,FILE *fptr_src_image,FILE *fptr_stego_image)
{
	char image_buffer[32];
	//reading 32 byte of data from source image
	fread(image_buffer,1,32,fptr_src_image);
	//Calling the encode_size_to_lsb function
	encode_size_to_lsb(extn_size,image_buffer);
	//write 32 byte to stego_image_fname
	if(fwrite(image_buffer,1,32,fptr_stego_image)==32)
	{
		return e_success;
	}
}
//encode_secret_file_extn function defnitioln
Status encode_secret_file_extn(char *extn,FILE *fptr_src_image,FILE *fptr_stego_image)
{
        //function calling
	encode_data_to_image(extn, fptr_src_image,fptr_stego_image);
	return e_success;
}
//encode_secret_file_size function defnition
Status encode_secret_file_size(int file_size,FILE *fptr_src_image,FILE *fptr_stego_image)
{
	char buffer[32];
	fread(buffer,1,32,fptr_src_image);
	encode_size_to_lsb(file_size,buffer);
	if(fwrite(buffer,1,32,fptr_stego_image)==32)
	{
		return e_success;
	}
}
//encode_size_to_lsb function defnition
Status encode_size_to_lsb(unsigned int size,char *buffer)
{
	unsigned char bit,clear;
	int i;
	for(i=0;i<32;i++)
	{
		bit=(size&(1<<i))>>i;
		clear=buffer[i]&~(1);
		buffer[i]=clear|bit;
	}
}
Status encode_secret_file_data(FILE *fptr_secret,FILE *fptr_src_image,FILE *fptr_stego_image)
{
	char buffer[secret_file_size];
	rewind(fptr_secret);
	int i;
	for( i=0;i < secret_file_size; i++)
	{
		fread(&buffer[i],1,1,fptr_secret);
	}	buffer[i] = '\0';
	if(encode_data_to_image(buffer,fptr_src_image,fptr_stego_image)==e_success)
	{
		return e_success;
	}
}
Status copy_remaining_img_data(FILE *fptr_src_image,FILE *fptr_stego_image)
{
	char ch;
	while(fread(&ch,1,1,fptr_src_image)!=0)
	{
		fwrite(&ch,1,1,fptr_stego_image);
	}
	return e_success;
}
uint get_image_size_for_bmp(FILE *fptr_image)
{
	uint width, height;
	// Seek to 18th byte
	fseek(fptr_image, 18, SEEK_SET);

	// Read the width (an int)
	fread(&width, sizeof(int), 1, fptr_image);
	printf("width = %u\n\n", width);

	// Read the height (an int)
	fread(&height, sizeof(int), 1, fptr_image);
	printf("height = %u\n\n", height);

	// Return image capacity
	return width * height * 3;
}

int get_file_size(FILE *fptr_secret)
{
	fseek(fptr_secret,0,SEEK_END);
	return ftell(fptr_secret);
}

//open_files function defnition
Status open_files(EncodeInfo *encInfo)
{
	//opening the .bmp file in read mode and store it into fptr_src_image file pointer
	encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
	//checking the file pointer is NULL or not,Do error handling
	if (encInfo->fptr_src_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
		return e_failure;
	}
	//opening the secret file in read mode and store it into fptr_secret file pointer
	encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
	// Do Error handling
	if (encInfo->fptr_secret == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

		return e_failure;
	}
	//opening the output.bmp in write mode and store into a file pointer
	encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
	// Do Error handling
	if (encInfo->fptr_stego_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

		return e_failure;
	}
	return e_success;
}
