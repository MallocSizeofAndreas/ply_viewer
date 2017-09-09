#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 128

typedef struct HeaderData {
    int vertex;
    int face;
} HeaderData;

typedef struct Point {
    float x;
    float y;
    float z;
} Point;

int read_header(FILE *fp, HeaderData *hd)
{
    char    kwrd1[BUFFER_SIZE], kwrd2[BUFFER_SIZE];
    char    *line = NULL;
    int     value, line_number = 0;
    size_t  line_size;

    while (!feof(fp)) {
        getline(&line, &line_size, fp);
        sscanf(line, "%s %s %d", kwrd1, kwrd2, &value);

        if( line_number == 0 && strcmp(kwrd1, "ply") !=0 ){
            printf("ERROR: Not a Ply file\n");
            return 1;
        }

        if(!strcmp(kwrd1, "element") && !strcmp(kwrd2, "vertex")){
            hd->vertex = value;
        }

        if(!strcmp(kwrd1, "element") && !strcmp(kwrd2, "face")){
            hd->face = value;
        }

        if (!strcmp(kwrd1, "end_header")) break;

        line_number++;
    }

    if (line) free(line);

    return 0;
}

Point *read_points(FILE *fp, int Np)
{
    int     i, line_number = 0;
    size_t  line_size;
    char    *line = NULL;
    Point   *points;

    points = malloc(Np*sizeof(Point));

    while (!feof(fp) && line_number < Np){
        Point p;
        getline(&line, &line_size, fp);
        sscanf(line, "%f %f %f", &p.x, &p.y, &p.z);
        points[line_number] = p;
        line_number++;
    }

    if (line) free(line);

    return points;
}

void print_points(Point *points, int Np)
{
    int i;
    for(i = 0; i < Np; i++){
        printf("%i: %10.5f\t%10.5f\t%10.5f\n",
                i+1, points[i].x, points[i].y, points[i].z);
    }
}

int main (void) {
    Point   *points;
    FILE    *fp;

    HeaderData hd;

    hd.vertex = 0;
    hd.face = 0;

    fp = fopen("cube.ply", "r");

    if (fp == NULL) {
        printf ("Problem opening the file");
        return 1;
    }

    read_header(fp, &hd);
    // printf("%d\n", hd.vertex);

    points = read_points(fp, hd.vertex);
    print_points(points, hd.vertex);

    // TODO: Draw Points

    // TODO: Read Faces
    // TODO: Draw Faces

    free(points);

    fclose (fp);
    return 0;
}


