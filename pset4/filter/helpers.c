#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //iterate throughout each pixel
    for (int i = 0; i < height; i++) //height aka row
    {
        for (int j = 0; j < width; j++) //width aka column
        {
            int OG_average = round(((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) /
                                    (float) 3)); //average of original RGB values
            image[i][j].rgbtBlue = OG_average; //set new values of RGB to the average of OG RGB
            image[i][j].rgbtGreen = OG_average;
            image[i][j].rgbtRed = OG_average;
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
            int sepiaRed = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue); //new sepia values
            int sepiaGreen = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);
            int sepiaBlue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue);

            //caps the sepia values at 255
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;

        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        int counter = 1;
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j]; //setup temporary array to equal the starting pixel on left half
            image[i][j] = image[i][width - counter]; //swap starting pixel to equal end pixel on right half
            image[i][width - counter] = temp; //swap end pixel to equal starting pixel (temp)
            counter++;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE temp = image[i][j]; //create a temp so original values don't get altered
            if (i == 0) //edge (literally lol) cases for the pixels on the outtermost layer. first row cases
            {
                if (j == 0) //top left corner
                {
                    temp.rgbtBlue = round((image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue + image[i + 1][j + 1].rgbtBlue
                                           + image[i + 1][j].rgbtBlue) / 4.0); //3 pixels surrounding top left corner

                    temp.rgbtRed = round((image[i][j].rgbtRed + image[i][j + 1].rgbtRed + image[i + 1][j + 1].rgbtRed
                                          + image[i + 1][j].rgbtRed) / 4.0);

                    temp.rgbtGreen = round((image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen + image[i + 1][j + 1].rgbtGreen
                                            + image[i + 1][j].rgbtGreen) / 4.0);
                }
                else if (j == width - 1) //top right corner
                {
                    temp.rgbtBlue = round((image[i][j].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j - 1].rgbtBlue
                                           + image[i][j - 1].rgbtBlue) / 4.0); //3 pixels surrounding top right corner

                    temp.rgbtGreen = round((image[i][j].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j - 1].rgbtGreen
                                            + image[i][j - 1].rgbtGreen) / 4.0); //3 pixels surrounding top right corner

                    temp.rgbtRed = round((image[i][j].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j - 1].rgbtRed
                                          + image[i][j - 1].rgbtRed) / 4.0); //3 pixels surrounding top right corner
                }
                else //remaining middle pixels on top row
                {
                    temp.rgbtBlue = round((image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue + image[i + 1][j + 1].rgbtBlue
                                           + image[i + 1][j].rgbtBlue + image[i + 1][j - 1].rgbtBlue + image[i][j - 1].rgbtBlue) / 6.0); //6.0 pixels surrounding

                    temp.rgbtGreen = round((image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen + image[i + 1][j + 1].rgbtGreen
                                            + image[i + 1][j].rgbtGreen + image[i + 1][j - 1].rgbtGreen + image[i][j - 1].rgbtGreen) / 6.0); //6.0 pixels surrounding

                    temp.rgbtRed = round((image[i][j].rgbtRed + image[i][j + 1].rgbtRed + image[i + 1][j + 1].rgbtRed
                                          + image[i + 1][j].rgbtRed + image[i + 1][j - 1].rgbtRed + image[i][j - 1].rgbtRed) / 6.0); //6.0 pixels surrounding
                }
            }
            if (j == 0) //first column cases
            {
                if (i == 0) //top left corner
                {
                    temp.rgbtBlue = round((image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue + image[i + 1][j + 1].rgbtBlue
                                           + image[i + 1][j].rgbtBlue) / 4.0); //3 pixels surrounding top left corner

                    temp.rgbtRed = round((image[i][j].rgbtRed + image[i][j + 1].rgbtRed + image[i + 1][j + 1].rgbtRed
                                          + image[i + 1][j].rgbtRed) / 4.0);

                    temp.rgbtGreen = round((image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen + image[i + 1][j + 1].rgbtGreen
                                            + image[i + 1][j].rgbtGreen) / 4.0);
                }
                else if (i == height - 1) //bottom left corner
                {
                    temp.rgbtBlue = round((image[i][j].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue
                                           + image[i][j + 1].rgbtBlue) / 4.0); //3 pixels surrounding bottom left corner

                    temp.rgbtRed = round((image[i][j].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed
                                          + image[i][j + 1].rgbtRed) / 4.0);

                    temp.rgbtGreen = round((image[i][j].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen
                                            + image[i][j + 1].rgbtGreen) / 4.0);
                }
                else //remaining middle pixels on utmost left column
                {
                    temp.rgbtBlue = round((image[i][j].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue
                                           + image[i][j + 1].rgbtBlue + image[i + 1][j + 1].rgbtBlue + image[i + 1][j].rgbtBlue) / 6.0); //6.0 pixels surrounding

                    temp.rgbtGreen = round((image[i][j].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen
                                            + image[i][j + 1].rgbtGreen + image[i + 1][j + 1].rgbtGreen + image[i + 1][j].rgbtGreen) / 6.0);

                    temp.rgbtRed = round((image[i][j].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed
                                          + image[i][j + 1].rgbtRed + image[i + 1][j + 1].rgbtRed + image[i + 1][j].rgbtRed) / 6.0);
                }
            }
            if (i == height - 1) //last row cases
            {
                if (j == 0) //bottom left corner
                {
                    temp.rgbtBlue = round((image[i][j].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue
                                           + image[i][j + 1].rgbtBlue) / 4.0); //3 pixels surrounding bottom left corner

                    temp.rgbtRed = round((image[i][j].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed
                                          + image[i][j + 1].rgbtRed) / 4.0);

                    temp.rgbtGreen = round((image[i][j].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen
                                            + image[i][j + 1].rgbtGreen) / 4.0);
                }
                else if (j == width - 1) //bottom right corner
                {
                    temp.rgbtBlue = round((image[i][j].rgbtBlue + image[i][j - 1].rgbtBlue + image[i - 1][j - 1].rgbtBlue
                                           + image[i - 1][j].rgbtBlue) / 4.0);

                    temp.rgbtGreen = round((image[i][j].rgbtGreen + image[i][j - 1].rgbtGreen + image[i - 1][j - 1].rgbtGreen
                                            + image[i - 1][j].rgbtGreen) / 4.0);

                    temp.rgbtRed = round((image[i][j].rgbtRed + image[i][j - 1].rgbtRed + image[i - 1][j - 1].rgbtRed
                                          + image[i - 1][j].rgbtRed) / 4.0);
                }
                else //remaining middle pixels on last row
                {
                    temp.rgbtBlue = round((image[i][j].rgbtBlue + image[i][j - 1].rgbtBlue + image[i - 1][j - 1].rgbtBlue
                                           + image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i][j + 1].rgbtBlue) / 6.0); //6.0 pixels surrounding

                    temp.rgbtGreen = round((image[i][j].rgbtGreen + image[i][j - 1].rgbtGreen + image[i - 1][j - 1].rgbtGreen
                                            + image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i][j + 1].rgbtGreen) / 6.0); //6.0 pixels surrounding

                    temp.rgbtRed = round((image[i][j].rgbtRed + image[i][j - 1].rgbtRed + image[i - 1][j - 1].rgbtRed
                                          + image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i][j + 1].rgbtRed) / 6.0); //6.0 pixels surrounding
                }
            }
            if (j == width - 1) //last column cases
            {
                if (i == 0) //top right corner
                {
                    temp.rgbtBlue = round((image[i][j].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j - 1].rgbtBlue
                                           + image[i][j - 1].rgbtBlue) / 4.0); //3 pixels surrounding top right corner

                    temp.rgbtGreen = round((image[i][j].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j - 1].rgbtGreen
                                            + image[i][j - 1].rgbtGreen) / 4.0); //3 pixels surrounding top right corner

                    temp.rgbtRed = round((image[i][j].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j - 1].rgbtRed
                                          + image[i][j - 1].rgbtRed) / 4.0); //3 pixels surrounding top right corner
                }
                else if (i == height - 1) //bottom right corner
                {
                    temp.rgbtBlue = round((image[i][j].rgbtBlue + image[i][j - 1].rgbtBlue + image[i - 1][j - 1].rgbtBlue
                                           + image[i - 1][j].rgbtBlue) / 4.0);

                    temp.rgbtGreen = round((image[i][j].rgbtGreen + image[i][j - 1].rgbtGreen + image[i - 1][j - 1].rgbtGreen
                                            + image[i - 1][j].rgbtGreen) / 4.0);

                    temp.rgbtRed = round((image[i][j].rgbtRed + image[i][j - 1].rgbtRed + image[i - 1][j - 1].rgbtRed
                                          + image[i - 1][j].rgbtRed) / 4.0);
                }
                else //middle pixels on last column
                {
                    temp.rgbtBlue = round((image[i][j].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j - 1].rgbtBlue + image[i][j - 1].rgbtBlue
                                           + image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue) / 6.0); //6.0 pixels surrounding

                    temp.rgbtGreen = round((image[i][j].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j - 1].rgbtGreen + image[i][j -
                                            1].rgbtGreen
                                            + image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen) / 6.0); //6.0 pixels surrounding

                    temp.rgbtRed = round((image[i][j].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j - 1].rgbtRed + image[i][j - 1].rgbtRed
                                          + image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed) / 6.0); //6 pixels surrounding
                }
            }
            //bottom code causes seg fault :(
            /*else //every other pixel in the middle thats not on the outer layer
            {
                temp.rgbtBlue = round((image[i][j].rgbtBlue + image[i][j - 1].rgbtBlue + image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue
                + image[i - 1][j + 1].rgbtBlue + image[i][j + 1].rgbtBlue + image[i + 1][j + 1].rgbtBlue + image[i + 1][j].rgbtBlue +
                image[i + 1][j - 1].rgbtBlue) / 9.0); //9.0 pixels surrounding

                temp.rgbtGreen = round((image[i][j].rgbtGreen + image[i][j - 1].rgbtGreen + image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen
                + image[i - 1][j + 1].rgbtGreen + image[i][j + 1].rgbtGreen + image[i + 1][j + 1].rgbtGreen + image[i + 1][j].rgbtGreen +
                image[i + 1][j - 1].rgbtGreen) / 9.0); //9.0 pixels surrounding

                temp.rgbtRed = round((image[i][j].rgbtRed + image[i][j - 1].rgbtRed + image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed
                + image[i - 1][j + 1].rgbtRed + image[i][j + 1].rgbtRed + image[i + 1][j + 1].rgbtRed + image[i + 1][j].rgbtRed +
                image[i + 1][j - 1].rgbtRed) / 9.0); //9.0 pixels surrounding
            }
            image[i][j].rgbtBlue = temp.rgbtBlue;
            image[i][j].rgbtGreen = temp.rgbtGreen;
            image[i][j].rgbtRed = temp.rgbtRed;*/
        }
    }
}
