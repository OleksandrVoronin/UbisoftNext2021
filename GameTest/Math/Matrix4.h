// #pragma once
// #include <include/GL/freeglut.h>
//
// #include "Matrix3.h"
// #include "App/app.h"
//
//
// class Matrix4 : public Matrix3
// {
// public:
//     float entries[4][4];
//     
//     Matrix4& Matrix4::operator = (const Matrix4 other)
//     {
//         for(UINT Row = 0; Row < 4; Row++)
//         {
//             for(UINT Col = 0; Col < 4; Col++)
//             {
//                 entries[Row][Col] = other[Row][Col];
//             }
//         }
//         return (*this);
//     }
//
//     Matrix4()
//     {
//         for(int y = 0; y < 4; y++)
//         {
//             for(int x = 0; x < 4; x++)
//             {
//                 entries[y][x] = 0;   
//             }
//         }
//     }
//
//     Matrix4(const Float3 a, const Float3 b, const Float3 c,  const Float3 d)
//     {
//         entries[0][0] = a.x;
//         entries[0][1] = a.y;
//         entries[0][2] = a.z;
//         entries[0][3] = 0;
//
//         entries[1][0] = b.x;
//         entries[1][1] = b.y;
//         entries[1][2] = b.z;
//         entries[1][3] = 0;
//         
//         entries[2][0] = c.x;
//         entries[2][1] = c.y;
//         entries[2][2] = c.z;
//         entries[2][3] = 0;
//         
//         entries[3][0] = d.x;
//         entries[3][1] = d.y;
//         entries[3][2] = d.z;
//         entries[3][3] = 0;
//     }
//
//     Matrix4(const Matrix4 &toCopy)
//     {
//         for(int y = 0; y < 4; y++)
//         {
//             for(int x = 0; x < 4; x++)
//             {
//                 entries[y][x] = toCopy[y][x];   
//             }
//         }
//     }
//
//
//     // static Matrix4 Identity()
//     // {
//     //     Matrix4 result = Matrix4();
//     //     for(int x = 0; x < 4; x++)
//     //     {
//     //         for(int y = 0; y < 4; y++)
//     //         {
//     //             if(x == y)
//     //             {
//     //                 result[x][y] = 1.0f;
//     //             }
//     //             else
//     //             {
//     //                 result[x][y] = 0.0f;
//     //             }
//     //         }
//     //     }
//     //     return result;
//     // }
//
//     // Matrix4 RowReduce()
//     // {
//     //     Matrix4 result = Matrix4(*this);
//     //     for(int lead = 0; lead < 4; lead++)
//     //     {
//     //         float divider = result[lead][lead];
//     //
//     //         if(divider == 0)
//     //         {
//     //             // Need to pivot, swap rows.
//     //             for(int swapCandidate = lead + 1; swapCandidate < 4; swapCandidate++)
//     //             {
//     //                 if(result[swapCandidate][lead] != 0)
//     //                 {
//     //                     SwapRows(lead, swapCandidate);
//     //                     break;
//     //                 }
//     //             }
//     //
//     //             if(divider == 0)
//     //             {
//     //                 break;
//     //             }
//     //         }
//     //         
//     //         for(int x = 0; x < 4; x++)
//     //         {
//     //             result[x][lead] = result[x][lead] / divider;
//     //         }
//     //
//     //         for(int y = lead + 1; y < 4; y++)
//     //         {
//     //             float multiplier = result[lead][y];
//     //             for(int x = 0; x < 4; x++)
//     //             {
//     //                 result[x][y] -= result[x][lead] * multiplier;
//     //             }
//     //         }
//     //     }
//     //     
//     //     return result;
//     // }
//     //
//     // void SwapRows(int a, int b)
//     // {
//     //     std::swap(entries[a], entries[b]);
//     // }
//
// };
