//--------------------------------------------------------------------------------------//
//                                                                                      //
//                                Utility functions file                                //
//                                                                                      //
//--------------------------------------------------------------------------------------//
// This file contains general utility functions

import java.util.ArrayList;

/**
  * Get the index of the largest item in an array
  */
int getIndexOfLargest(ArrayList array) {
    // If the array is empty or not defined, return -1 as an error value
    if (array == null || array.size() == 0) {
        return -1;
    }

    // Keep track of the index of the largest value
    int largest = 0;

    // Loop through all of the points and find the largets value
    for (int i = 1; i < array.size(); i++) {
        if ((float)array.get(i) > (float)array.get(largest)) {
            largest = i;
        }
    }

    return largest;
}

float PI2 = PI * 2;

/**
  * Return the angle to a point in radians
  */
float angleToPoint(int index, int pointCount) {
    return index * (PI2 / pointCount);
}