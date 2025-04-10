#include "ECDA.h"

void generate_elipse_points(int starting_point, int order)
{
    float pointElipse[1000][2];
    float multiplyElipse[1000][2];

    pointElipse[0][0] = starting_point;
    pointElipse[0][1] = fmod(sqrtf((pow(pointElipse[0][0], 3) + 7)), order);

    multiplyElipse[0][0] = pointElipse[0][0];
    multiplyElipse[0][1] = pointElipse[0][1];

    for (int i = 1; i < order; i++)
    {
        pointElipse[i][0] = starting_point + i;
        pointElipse[i][1] = fmod(sqrtf((pow(pointElipse[i][0], 3) + 7)), order);

        multiplyElipse[i][0] = 1 + i;
        multiplyElipse[i][1] = fmod(multiplyElipse[0][1] * (i + 1), order);
    }

    CSV_file = fopen("curve.csv", "w +");

    for (int i = 0; i < order; i++)
    {
        cout << pointElipse[i][0] << " ";
        cout << pointElipse[i][1] << "      " << multiplyElipse[i][0] << " ";
        cout << multiplyElipse[i][1] << endl;

        fprintf(CSV_file, "%f, ", pointElipse[i][0]);
        fprintf(CSV_file, "%f\n", pointElipse[i][1]);

        printf("\n");
    }

    fclose(CSV_file);

}