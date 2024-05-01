#include<stdio.h>
#include "decode.h"
#include "types.h"
#include "common.h"
#include <string.h>
int s_file_extn_size;
int s_file_size;
//int decoded_size;
char dec_file_extn[5];

//function used to check if all arguments given are valid 
Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo)
{ 
        //checking the argv[2]
        if (strstr(argv[2], ".bmp") == NULL || strcmp(strstr(argv[2], ".bmp"), ".bmp"))
	{
		printf("INFO:Read and validation failed\n\n");
		return e_failure;
	}
	else
	{
		decInfo->stego_image_fname=argv[2];
	}
	//checking the argv[3]
	if(argv[3]!=NULL)
	{
		strcpy(decInfo->secret_fname,argv[3]);
	}
	else
	{
		strcpy(decInfo->secret_fname,"def");
		printf("\nINFO:Output file not mentioned.Default file added\n\n");
		printf("INFO:Read and validation executed successfully\n\n");
	}
	return e_success;
}
//do_decoding function defnition
Status do_decoding(DecodeInfo *decInfo)
{
	//checking the open_dec_files
	if(open_dec_files(decInfo)==e_success)
	{
		printf("Info:Files opened successfully\n\n");
	}
	else
	{
		printf("Info:files opening failed\n\n");
		return e_failure;
	}
	//checking the output of decode_magic_string
	if(decode_magic_string(decInfo->fptr_stego_image)==e_success)
	{
		printf("INFO:Magic string matched  successfully\n\n");
	}
	else
	{
		printf("INFO:Magic string not matched\n\n");
		return e_failure;
	}
	//checking decode_secret_file_extn_size
	if(decode_secret_file_extn_size(decInfo->fptr_stego_image)==e_success)
	{
		printf("INFO:Secret file extn size is decoded successfully\n\n");
	}
	else
	{
		printf("INFO:Secret file not decoded\n\n");
		return e_failure;
	}
	//checking the  decode_secret_file_extn  
	if(decode_secret_file_extn(decInfo->fptr_stego_image,decInfo)==e_success)
	{
		printf("INFO:Secret file extension decoded successfully\n\n");
	}
	else
	{
		printf("INFO:Secret file extension not encoded\n\n");
		return e_failure;
	}
	//checking the output of dec_open_output_file
	if(dec_open_output_file(decInfo)==e_success)
	{
		printf("INFO:Output file opened successfully\n\n");
	}
	else
	{
		printf("INFO:Output file failed to open\n\n");
		return e_failure;
	}
	//checking the decode_secret_file_size
	if(decode_secret_file_size(decInfo->fptr_stego_image)==e_success)
	{
		printf("INFO:Secret file size decoded\n\n");
	}
	else
	{
		printf("INFO:Secret file not decoded\n\n");
		return e_failure;
	}
	//checking the output of decode_secret_file_data
	if(decode_secret_file_data(decInfo->fptr_secret,decInfo->fptr_stego_image)==e_success)
	{
		printf("INFO:Secret message decoded successfully\n\n\n");
		printf("\nDECODING COMPLETED\n");
		return e_success;
	}
	else
	{
		printf("INFO:Secret message not decoded\n");
		return e_failure;
	}
}
//function defnition
Status open_dec_files(DecodeInfo *decInfo)
{
        //opening the stego_image_fname in read mode and store in file pointer 
	decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
	// Do Error handling
	if (decInfo->fptr_stego_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

		return e_failure;
	}

	// No failure return e_success
	return e_success;
}
//function defnition
Status decode_magic_string(FILE *fptr_stego_image)
{
	int len=strlen(MAGIC_STRING);
	char dec_data[len+1];
	fseek(fptr_stego_image,54,SEEK_SET);
	//function call
	decode_data_from_image(fptr_stego_image,dec_data,len);
	dec_data[len+1]='\0';
	if(strcmp(dec_data,MAGIC_STRING)==0)
	{
		return e_success;
	}
}
//function defnition
Status decode_data_from_image(FILE *fptr_stego_image,char *dec_data,int len)
{
	char image_buffer[8];
	int i=0;
	for(i=0;i<len;i++)
	{
		//read 8 byte of data from stego image and store it into the image buffer
		fread(image_buffer,8,1,fptr_stego_image);
		//function call
		decode_byte_from_lsb(&dec_data[i],image_buffer);
	        printf("dec data:%s\n",dec_data);
	}
	return e_success;
}
//function defnition
Status decode_byte_from_lsb(char *dec_buffer,char *image_buffer)
{
	*dec_buffer=0;
	for(int i=0;i<8;i++)
	{
	       unsigned char bit=image_buffer[i]&1;
		*dec_buffer= *dec_buffer | (bit<<i);
	} 
	printf("decoded byte:%s\n",dec_buffer);
}
//function defnition
Status decode_secret_file_extn_size(FILE *fptr_stego_image)
{
	char buffer[32];
	//rerading 32 byte of data fptr_stego_image and storing int the buffer
	fread(buffer,32,1,fptr_stego_image);
	int decoded_size=0;
	//function call
	decode_size_from_lsb(buffer,&decoded_size);//&
	s_file_extn_size=decoded_size;
	printf("%d\n",decoded_size);

	return e_success;
}
//function defnition
Status decode_size_from_lsb(char buffer[],int *decoded_size)
{
	*decoded_size=0;
	for(int i=0;i<32;i++)
	{
		int bit = buffer[i]&(1);
		*decoded_size = *decoded_size | (bit<<i);
	}
	printf("decoded size:%d\n",*decoded_size);
}
//function defnition
Status decode_secret_file_extn(FILE *fptr_stego_image,DecodeInfo *decInfo)
{
	char buffer[32];
	int len=s_file_extn_size;
	printf("s_file_extn_size:%d",len);
	//calling decode_data_from_image function
	decode_data_from_image(fptr_stego_image,buffer,len);
	buffer[len]='\0';
	printf("secret file extn:%s\n",buffer);
	strcat(decInfo->secret_fname,buffer);
	return e_success;
}
//function defnition
Status dec_open_output_file(DecodeInfo *decInfo)
{
	char buffer[200];
	decInfo->fptr_secret=fopen(decInfo->secret_fname,"wb+");
	if(decInfo->fptr_secret == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->secret_fname);
		return e_failure;
	}
	return e_success;
}
//function defnition
Status decode_secret_file_size(FILE *fptr_stego_image)
{
	char buffer[32];
	int decoded_size=0;
	//reading 32 byte from the fptr_stego_image and store into buffer	
	fread(buffer,32,1,fptr_stego_image);
	//function call
	decode_size_from_lsb(buffer,&decoded_size);
	s_file_size=decoded_size;
	printf("s_file_size in file size:%d\n",s_file_size);
	return e_success;
}
//function defnition
Status decode_secret_file_data(FILE *fptr_secret,FILE *fptr_stego_image)
{
	int len=s_file_size;
	printf("s_file_size in file data:%d\n",s_file_size);
	unsigned char dec_mes[len];
	//calling function
	decode_data_from_image(fptr_stego_image,dec_mes,len);
	fwrite(dec_mes,len,1,fptr_secret);
	return e_success;
}















