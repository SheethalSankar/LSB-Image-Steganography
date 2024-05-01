/*
Name: Sheethal Sankar
Date: 15-02-2024
Description: STEGANOGRAPHY PROJECT
*/

#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
int main(int argc,char *argv[])
{
	EncodeInfo encInfo;
	DecodeInfo decInfo;
	//checking the arguement count
	if(argc==1)
	{
		printf("Please pass valid arguments.\nINFO : Encoding - minimun 4 arguments.\nINFO : Decoding - minimum 3 arguments\n");
		return e_failure;
	}
	int res = check_operation_type(argv);
        //checking the value of res	
	if(res == 0)
	{
	        //checking the arguement count 2 or 3
		if(argc==2 || argc==3)
		{
			printf("INFO : For encoding please pass minimum 4 arguments like ./a.out -e source_image secret_data_file\n");
			return e_failure;
		}
		printf("Encoding data\n");
		//checking the function output,if it is 0
		if(read_and_validate_encode_args(argv, &encInfo )== 0)
		{
		        //calling do_encoding function
			do_encoding(&encInfo);
		}
		else
		{
			printf("Encoding failed\n");
		}
	}
	//if res is not equal to 0
	else if(res==1)
	{
	        //checking the arguement count
		if(argc==2)
		{
			printf("INFO : For decoding please pass minimum 3 arguments like ./a.out -d source_image\n");
			return e_failure;
		}
		printf("Deconding data\n");
		//checking the read_and_validate_decode_args function
		if(read_and_validate_decode_args(argv,&decInfo)==0)
		{
			do_decoding(&decInfo);
		}
		else
			printf("Decoding failed\n");
	}
	else
		printf("Unsupported\n");

	
}
//OperationType function defnition
OperationType check_operation_type(char *argv[])
{
        //checking the argv[1] is "-e" or not
	if(!strcmp(argv[1], "-e"))
	{
		return e_encode;
	}
	//checking the argv[2] is "-d" or not
	else if(!strcmp(argv[1], "-d"))
	{
		return e_decode;
	}
	else
		return e_unsupported;
}

