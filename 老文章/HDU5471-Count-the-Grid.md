---
title: HDU5471 Count the Grid（未完）
tag: [容斥]
---

#### 题目

You get a grid of h rows and w columns. Rows are numbered 1, 2, 3, ... , h from top to bottom while columns are numbered 1, 2 , 3, ... , w from left to right. Each cell can be represented by a pair of numbers (x, y), which means this cell is located at row x column y.
You fill the every cell with an integer ranging from 1 to m.
Then you start to play with the gird. Every time you chose a rectangle whose upper left cell is (x1, y1) and lower right cell is (x2, y2), finally you calculate the maximum value v of this rectangle.
After you played n times, you left. When you return, you find that the grid is disappeared. You only remember n rectangles and their corresponding maximum value. You are wondering how many different gird can satisfy your memory. Two grids are different if there is a cell filled different integer.
Give your answer modulo (109+7).
Your memory may have some mistakes so that no grid satisfies it, in this case just output 0.

 

一个矩阵中填数，数的范围[1,m]。给你 N 个小矩阵并且告诉你此矩阵中的最大值 v，求有多少种大矩阵满足
所给条件。（%1e9+7）

#### 思路

**redpojoe神仙**的做法，有些难理解。

对于每一块小区域来说，设它里面的元素的最大值为$v$，那么它里面的元素的取值范围

算法步骤：

1. 矩阵求交