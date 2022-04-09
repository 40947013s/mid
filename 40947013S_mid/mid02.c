#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

char line[10000];
char country[600][100], Country[100];
char code[600][100];
char id_name[600][100];
char id_code[600][100];
long int data[600][41] = {0};
int start, end, l = 0;

long int my_strtol(char *str)
{
    long int ans = 0;
    for(int i = 0; i < strlen(str); i++)
    {
        ans += (str[i]-48)*pow(10, strlen(str)-i-1);
    }
    return ans;
}

int choice()
{
    int choice = 0;
    if(scanf("%d",&choice)==-1)
    {
        return -1;
    }
    else if(choice<1||choice>8)
    {
        return -1;
    }
    return choice;
}

void menu()
{
    printf("--- Menu ---\n1) Which country has the largest population growth?\n2) Which country has the largest population growth rate?\n3) Which country has the smallest population growth , including negative growth?\n4) Which country has the smallest population growth rate, including negativegrowth?\n5) Which country has the largest population standard deviation?\n6) Which country has the smallest population standard deviation?\n7) Given a country name (case -insensitive), please use linear least squares method to predict the country's population in 2021.\n8) Exit\n");
}

void population(int flag)
{
    long int largest = -10000000, smallest = 10000000;
    double l_rate = -100, s_rate = 100, l_d = -1000000, s_d;
    double rate[l-5], d[l-5];
    for(int i = 0; i < l-5; i++)
    {
        if(data[i][end] - data[i][start] > largest)
            largest = data[i][end] - data[i][start];
        if(data[i][end] - data[i][start] < smallest)
            smallest = data[i][end] - data[i][start];
        rate[i] = 0.0, d[i] = 0.0;
        for(int j = start; j <= end; j++)
            rate[i] += data[i][j];
        rate[i] /= (end-start);
        
        if((data[i][end]-data[i][start])/rate[i] > l_rate)
            l_rate = (data[i][end]-data[i][start])/rate[i];
        if((data[i][end]-data[i][start])/rate[i] < s_rate)
            s_rate = (data[i][end]-data[i][start])/rate[i];
        for(int j = start; j <= end; j++)
        {
            double k = data[i][j]-rate[i];
            if(k == 0) continue;
            d[i] += pow((data[i][j]-rate[i]), 2)/(end-start);
        }
        d[i] = sqrt(d[i]);
        s_d = d[0];        
        //printf("%lf\n", d[i]);
        if(d[i] > l_d) l_d = d[i];
        if(d[i] < s_d) l_d = d[i];
    }
        
    for(int i = 0; i < l-5; i++)
    {
        if(flag == 1 && data[i][end]-data[i][start] == largest)
            printf("%s\n", country[i]);
        if(flag == 2 && (data[i][end]-data[i][start])/rate[i] == l_rate)
            printf("%s\n", country[i]);
        if(flag == 3 && data[i][end]-data[i][start] == smallest)   
            printf("%s\n", country[i]);
        if(flag == 4 && (data[i][end]-data[i][start])/rate[i] == s_rate)
            printf("%s\n", country[i]);   
        if(flag == 5 && d[i] == l_d)
            printf("%s\n", country[i]);
        if(flag == 6 && d[i] == s_d)
            printf("%s\n", country[i]);        
    }    
    return;
}



int main() 
{
    FILE *pFile = NULL;
    char name[127];
    printf("Please open a season record: ");
    scanf("%s",name);
    if(( pFile = fopen( name, "r" ) ) == NULL)
    {
        printf( "File could not be opened!\n" );
        return 0;
    }

    printf("Please enter the analysis start time: ");
    scanf("%d", &start); start -= 1960;

    printf("Please enter the analysis end time: ");
    scanf("%d", &end); end -= 1960;

    if(end - start <= 0) 
    {
        printf("Wrong time. Exit.\n");
        return 0;
    }
    
    char *result = NULL, str[4];
    str[0] = 34, str[1] = ',', str[2] = 34, str[3] = 0;
    while(fgets(line,10000,pFile) != NULL)
    {
        result = strtok(line, str);
        int i = 0;
        if(l < 5)
        {
            l++; continue;
        }
        while(result != NULL) 
        {
            if(i == 0)
                strncpy(country[l-5], result, strlen(result));                
            else if(i == 1)
                strncpy(code[l-5], result, strlen(result));
            else if(i == 2)
                strncpy(id_name[l-5], result, strlen(result));
            else if(i == 3)
                strncpy(id_code[l-5], result, strlen(result));
            else
                data[l-5][i-5] = my_strtol(result);        
            i++;
            result = strtok(NULL, str);
        }
        //printf("%s %ld\n", country[l-5], data[l-5][0]);
        l++;
    }
    //printf(" %s %ld\n", country[0], data[0][start]);

    menu();
    while(1)
    {
        printf("Your choice: ");
        int c = choice(), C;
        char *t;
        switch(c)
        {
            case 1:
                population(1); break;
            case 2:
                population(2); break;
            case 3:
                population(3); break;
            case 4:
                population(4); break;
            case 5:
                population(5); break;
            case 6:
                population(6); break;
            case 7:
                printf("Country: ");
                scanf("%s", Country);
                printf("%ld\n", data[67][31]); break;
            case 8:
                printf("Exit\n");
                return 0;
            default:
                printf("Again\n");
                break;
        }
        printf("\n");
    }        

    fclose(pFile);
    return 0;
}