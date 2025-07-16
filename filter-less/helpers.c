#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float avg =
                round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);

            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE origRed = image[i][j].rgbtRed;
            BYTE origGreen = image[i][j].rgbtGreen;
            BYTE origBlue = image[i][j].rgbtBlue;

            image[i][j].rgbtRed =
                round(fmin(0.393 * origRed + 0.769 * origGreen + 0.189 * origBlue, 255.0));
            image[i][j].rgbtGreen =
                round(fmin(0.349 * origRed + 0.686 * origGreen + 0.168 * origBlue, 255.0));
            image[i][j].rgbtBlue =
                round(fmin(0.272 * origRed + 0.534 * origGreen + 0.131 * origBlue, 255.0));
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE tmp = image[i][j];

            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = tmp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int blueSum = 0;
            int greenSum = 0;
            int redSum = 0;

            int count = 0;

            for (int l = 0; l < 3; l++)
            {
                for (int m = 0; m < 3; m++)
                {
                    bool isHeightInBounds = i + l - 1 >= 0 && i + l - 1 < height;
                    bool isWidthInBounds = j + m - 1 >= 0 && j + m - 1 < width;

                    if (isHeightInBounds && isWidthInBounds)
                    {
                        blueSum += copy[i + l - 1][j + m - 1].rgbtBlue;
                        greenSum += copy[i + l - 1][j + m - 1].rgbtGreen;
                        redSum += copy[i + l - 1][j + m - 1].rgbtRed;

                        count++;
                    }
                }
            }

            image[i][j].rgbtBlue = round((float) blueSum / count);
            image[i][j].rgbtGreen = round((float) greenSum / count);
            image[i][j].rgbtRed = round((float) redSum / count);
        }
    }
}
