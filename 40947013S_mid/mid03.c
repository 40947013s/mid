#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.1415926
typedef unsigned char  BYTE;  
typedef unsigned short WORD;  
typedef unsigned long  DWORD;  
typedef long LONG; 

typedef struct _sBmpheader
{
    char bm[2];
    uint32_t size;
    uint32_t reserve;
    uint32_t offset;
    uint32_t header_size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bpp;
    uint32_t compression;
    uint32_t bitmap_size;
    uint32_t hres;
    uint32_t vres;
    uint32_t used;
    uint32_t important;
}__attribute__((__packed__)) Bmpheader;

int main()
{
    FILE *fp_in , *fp_out;
    char input[128] , output[128];
    int32_t angle = 0;
    int32_t bytes = 0;
    Bmpheader header = {0};

    printf( "Please input a BMP file: " );
    fgets( input , 127 , stdin );
    if( input[strlen(input) - 1] == '\n' )
    {
        input[strlen(input) - 1] = 0;
    }
    fp_in = fopen( input , "r" );
    if( fp_in == NULL )
    {
        printf("File could not be opened!\n");
        return 0;
    }

    printf( "Please input the output BMP file name: " );
    fgets( output , 127 , stdin );
    if( output[strlen(output) - 1] == '\n' )
    {
        output[strlen(output) - 1] = 0;
    }

    printf( "Angle (0-360): " );
    scanf( "%d" , &angle );
    if( angle < 0 || angle > 360 )
    {
        printf( "Error!\n" );
        return 0;
    }

    fp_out = fopen( output , "w+" );
    fread( &header , sizeof(header) , 1 , fp_in );

   
    double Sin = sin( angle * PI / 180.0 );
    double Cos = cos( angle * PI / 180.0 );
    int32_t old_width = header.width;
    int32_t old_height = header.height;
    int32_t width = (int32_t)(old_width * Cos + old_height * Sin);
    int32_t height = (int32_t)(old_width * Sin + old_height * Cos);
    header.width = (uint32_t)width;
    header.height = (uint32_t)height;


    fwrite( &header , sizeof(header) , 1 , fp_out );
    int32_t p_times = old_width * 3 + old_width % 4;
    int32_t n_times = header.width * 3 + header.width % 4;

    int32_t rotateX = old_width / 2;
    int32_t rotateY = old_height / 2;
    int32_t a_rotateX = width / 2;
    int32_t a_rotateY = height / 2;

    int Original = old_height * p_times;
    int Later = height * n_times;
    BYTE *p = (BYTE *)malloc( Original );
    memset( p , 0 , Original );
    BYTE *a = (BYTE *)malloc( Later );
    memset( a , 0 , Later );   
    fread( p , 1 , Original , fp_in );

    for(int h = 0;h < height; h++)
    {    
        for (int w=0;w<width;w++)
        {
            
            int32_t index = h * n_times + w * 3;
            double d_h = (w - a_rotateX) * Sin + (h - a_rotateY) * Cos + rotateY;
            double d_w = (w - a_rotateX) * Cos - (h - a_rotateY) * Sin + rotateX;      
            if (d_h < 0 || d_h > old_height || d_w < 0 || d_w > old_width)
            {
                a[index] = 255; 
                a[index+1] = 255; 
                a[index+2] = 255;  
                continue;
            }
            else
            {
                int32_t i_h = d_h;
                int32_t i_w = d_w;
                double distance_to_a_X = d_w - i_w;
                double distance_to_a_Y = d_h - i_h;
                int32_t point_A = i_h * p_times +  i_w * 3;
                int32_t point_B = i_h * p_times + ( i_w + 1 ) * 3;
                int32_t point_C = ( i_h + 1 ) * p_times + i_w * 3;
                int32_t point_D = ( i_h + 1 ) * p_times + ( i_w + 1 ) * 3;
                if( i_w == old_width - 1 )
                {               
                    point_A = point_B;
                    point_C = point_D;
                }
                if(i_h == old_height - 1)
                {
                    point_C = point_A;
                    point_D = point_B;
                }
                a[index] = (1 - distance_to_a_X) * (1 - distance_to_a_Y) * p[point_A] + (1 - distance_to_a_X) * distance_to_a_Y * p[point_B] + distance_to_a_X * (1 - distance_to_a_Y) * p[point_C] + distance_to_a_X * distance_to_a_Y * p[point_D]; 
                a[index+1] = (1 - distance_to_a_X) * (1 - distance_to_a_Y) * p[point_A+1] + (1 - distance_to_a_X) * distance_to_a_Y * p[point_B+1] + distance_to_a_X * (1 - distance_to_a_Y) * p[point_C+1] + distance_to_a_X * distance_to_a_Y * p[point_D+1];
                a[index+2] = (1 - distance_to_a_X) * (1 - distance_to_a_Y) * p[point_A+2] + (1 - distance_to_a_X) * distance_to_a_Y * p[point_B+2] + distance_to_a_X * (1 - distance_to_a_Y) * p[point_C+2] + distance_to_a_X * distance_to_a_Y * p[point_D+2];
            }    
        }
    }
    fwrite(a, 1, Later, fp_out);
    fclose(fp_in);
    fclose(fp_out);
    return 0;
}