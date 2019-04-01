void divider_4_points(const float x, const float y, float percentages[4]) {
    for(int i = 0; i < 4; ++i) {
        percentages[i] = 0;
    }
    if(x > 1 || y > 1) {
        return;
    }

    float percentageAB = 1 - y;
    float percentageCD = y;

    percentages[0] = percentageAB * (1 - x);
    percentages[1] = percentageAB * x;
    percentages[2] = percentageCD * (1 - x);
    percentages[3] = percentageCD * x;
}

void divider_9_points(const float x, const float y, float percentages[9]) {
    for(int i = 0; i < 9; ++i) {
        percentages[i] = 0.0f;
    }

    float relative_percentages[4];
    if(x < 0 && y < 0) {
        // upper-left section
        float relative_x = 1 + x;
        float relative_y = 1 + y;
        divider_4_points(relative_x, relative_y, relative_percentages);
        percentages[0] = relative_percentages[0];
        percentages[1] = relative_percentages[1];
        percentages[3] = relative_percentages[2];
        percentages[4] = relative_percentages[3];
    } else if (x >= 0 && y < 0) {
        float relative_x = x;
        float relative_y = 1 + y;
        divider_4_points(relative_x, relative_y, relative_percentages);
        percentages[1] = relative_percentages[0];
        percentages[2] = relative_percentages[1];
        percentages[4] = relative_percentages[2];
        percentages[5] = relative_percentages[3];
    } else if (x < 0 && y >= 0) {
        float relative_x = 1 + x;
        float relative_y = y;
        divider_4_points(relative_x, relative_y, relative_percentages);
        percentages[3] = relative_percentages[0];
        percentages[4] = relative_percentages[1];
        percentages[6] = relative_percentages[2];
        percentages[7] = relative_percentages[3];
    } else if (x >= 0 && y >= 0) {
        float relative_x = x;
        float relative_y = y;
        divider_4_points(relative_x, relative_y, relative_percentages);
        percentages[4] = relative_percentages[0];
        percentages[5] = relative_percentages[1];
        percentages[7] = relative_percentages[2];
        percentages[8] = relative_percentages[3];
    }
}

/**
 * edgeCase: 0 = wrap, 1 = block
*/
#define BLOCK_OFFSET 0.0001

void kernel calc_9_grids_for_forward_advection(global const int* width, global const int* height, global const int* edgeCase,
global const float* horVel, global const float* verVel, global const float* sourceValues,
global float* grid_1, global float* grid_2, global float* grid_3, global float* grid_4,
global float* grid_5, global float* grid_6, global float* grid_7, global float* grid_8, global float* grid_9)
{
    int index = get_global_id(0);
    int xCoord = index % *width;
    int yCoord = index / *width;
    float updatedHorVel = horVel[index];
    float updatedVerVel = verVel[index];
    if(*edgeCase == 1) {
        if(xCoord == 0 && updatedHorVel <= 0) {
            updatedHorVel = BLOCK_OFFSET;
        } else if (xCoord+1 == *width && updatedHorVel >= 0) {
            updatedHorVel = -BLOCK_OFFSET;
        }
        if(yCoord == 0 && updatedVerVel <= 0) {
            updatedVerVel = BLOCK_OFFSET;
        } else if (yCoord == *height && updatedVerVel >= 0) {
            updatedVerVel = -BLOCK_OFFSET;
        }
    }

    float percentages[9];
    divider_9_points(xCoord, yCoord, percentages);
    // upper-left
    int targetX = xCoord - 1;
    int targetY = yCoord - 1;
    if(*edgeCase == 0) {
        targetX = (targetX + *width) % *width;
        targetY = (targetY + *height) % *height;
    }
    if(targetX >= 0 && targetX < *width && targetY >= 0 && targetY < *height) {
        grid_1[targetX + targetY * *width] = percentages[0] * sourceValues[index];
    }
    // up
    targetX = xCoord;
    targetY = yCoord - 1;
    if(*edgeCase == 0) {
        targetX = (targetX + *width) % *width;
        targetY = (targetY + *height) % *height;
    }
    if(targetX >= 0 && targetX < *width && targetY >= 0 && targetY < *height) {
        grid_2[targetX + targetY * *width] = percentages[1] * sourceValues[index];
    }
    // upper-right
    targetX = xCoord + 1;
    targetY = yCoord - 1;
    if(*edgeCase == 0) {
        targetX = (targetX + *width) % *width;
        targetY = (targetY + *height) % *height;
    }
    if(targetX >= 0 && targetX < *width && targetY >= 0 && targetY < *height) {
        grid_3[targetX + targetY * *width] = percentages[2] * sourceValues[index];
    }
    // left
    targetX = xCoord - 1;
    targetY = yCoord;
    if(*edgeCase == 0) {
        targetX = (targetX + *width) % *width;
        targetY = (targetY + *height) % *height;
    }
    if(targetX >= 0 && targetX < *width && targetY >= 0 && targetY < *height) {
        grid_4[targetX + targetY * *width] = percentages[3] * sourceValues[index];
    }
    // middle
    targetX = xCoord;
    targetY = yCoord;
    if(*edgeCase == 0) {
        targetX = (targetX + *width) % *width;
        targetY = (targetY + *height) % *height;
    }
    if(targetX >= 0 && targetX < *width && targetY >= 0 && targetY < *height) {
        grid_5[targetX + targetY * *width] = percentages[4] * sourceValues[index];
    }
    // right
    targetX = xCoord + 1;
    targetY = yCoord;
    if(*edgeCase == 0) {
        targetX = (targetX + *width) % *width;
        targetY = (targetY + *height) % *height;
    }
    if(targetX >= 0 && targetX < *width && targetY >= 0 && targetY < *height) {
        grid_6[targetX + targetY * *width] = percentages[5] * sourceValues[index];
    }
    // lower-left
    targetX = xCoord - 1;
    targetY = yCoord + 1;
    if(*edgeCase == 0) {
        targetX = (targetX + *width) % *width;
        targetY = (targetY + *height) % *height;
    }
    if(targetX >= 0 && targetX < *width && targetY >= 0 && targetY < *height) {
        grid_7[targetX + targetY * *width] = percentages[6] * sourceValues[index];
    }
    // down
    targetX = xCoord ;
    targetY = yCoord + 1;
    if(*edgeCase == 0) {
        targetX = (targetX + *width) % *width;
        targetY = (targetY + *height) % *height;
    }
    if(targetX >= 0 && targetX < *width && targetY >= 0 && targetY < *height) {
        grid_8[targetX + targetY * *width] = percentages[7] * sourceValues[index];
    }
    // lower-right
    targetX = xCoord + 1;
    targetY = yCoord + 1;
    if(*edgeCase == 0) {
        targetX = (targetX + *width) % *width;
        targetY = (targetY + *height) % *height;
    }
    if(targetX >= 0 && targetX < *width && targetY >= 0 && targetY < *height) {
        grid_9[targetX + targetY * *width] = percentages[8] * sourceValues[index];
    }
}

void kernel sum_9_arrays(global float* target, global const float* grid_1, global const float* grid_2,
global const float* grid_3, global const float* grid_4, global const float* grid_5, global const float* grid_6,
global const float* grid_7, global const float* grid_8, global const float* grid_9) {
    int i = get_global_id(0);
    target[i] = grid_1[i] + grid_2[i] + grid_3[i] + grid_4[i] + grid_5[i] + grid_6[i] + grid_7[i] + grid_8[i] + grid_9[i];
}
