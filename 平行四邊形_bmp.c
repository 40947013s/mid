#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#define PI 3.1415926

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
    FILE *pFile , *pFile2;
    char input[127] , output[127];
    int32_t angle = 0;
    Bmpheader header;
    printf("Please input a BMP file: ");
    fgets(input,127,stdin);
    if( input[strlen(input) - 1] == '\n' )
    {
        input[strlen(input) - 1] = 0;
    }
    pFile = fopen(input,"r");
    if( pFile == NULL )
    {
        printf("File could not be opened!\n");
        return 0;
    }

    printf("Please input the output BMP file name: ");
    fgets(output,127,stdin);
    if( output[strlen(output) - 1] == '\n' )
    {
        output[strlen(output) - 1] = 0;
    }

    printf("Angle (0-90): ");
    scanf("%d",&angle);
    if( angle < 0 || angle > 90 )
    {
        printf("Error!\n");
        return 0;
    }

    pFile2 = fopen(output, "w");
    if( pFile2 == NULL )
    {
        printf("File could not be opened!\n");
        return 0;
    }
    
    if( angle == 0 )
    {
        fread(&header,sizeof(header),1,pFile);
        header.width = header.width * 2;
        header.height = 1;
        fwrite(&header,sizeof(header),1, pFile2);
        uint8_t *color;
        int32_t times = 0;
        times = header.width * 3 + header.width % 4;
        color = (uint8_t*)malloc(sizeof(uint8_t) * times);
        for(int i=0;i<times;i++)
        {
            color[i] = 0;
        }
        fwrite(color,1,times,pFile2);
        return 0 ;
    }

    if( angle == 90 )
    {
        fread(&header,sizeof(header),1,pFile);
        fwrite(&header,sizeof(header),1,pFile2);
        while(!feof(pFile))
        {
            uint8_t color[1024] = {0};
            int32_t count = fread(color,1,1024, pFile);
            fwrite(color,count,1,pFile2);
        }
        return 0;
    }

    double slope = tan( angle * PI / 180.0 );
    int32_t newW;
    uint8_t * p_color , * n_color;
    int32_t p_times = 0 , n_times = 0;

    fread(&header,sizeof(header),1,pFile);
    newW = (int32_t)(header.height * (1.0 / slope));
    header.width += newW;
    fwrite(&header,sizeof(header),1,pFile2);

    p_times = (header.width - newW) * 3 + (header.width - newW) % 4;
    n_times = (header.width) * 3 + (header.width) % 4;
    p_color = (uint8_t*)malloc(sizeof(uint8_t) * p_times);
    n_color = (uint8_t*)malloc(sizeof(uint8_t) * n_times);

    for(int i=0;i<header.height;i++)
    {
        fread(p_color,1,p_times,pFile);

        for(int j=0;j<n_times;j++)
        {
            n_color[j] = 255;
        }

        for(int k=0;k<n_times;k+=3)
        {
            if( slope * k / 3 - i >= 0.0 )
            {
                for(int l=0;l<p_times;l++)
                {
                    n_color[k+l] = p_color[l];
                }
                fwrite(n_color,1,n_times,pFile2);
                break;
            }
        }
    }

    fclose( pFile );
    fclose( pFile2 );
    return 0;
}