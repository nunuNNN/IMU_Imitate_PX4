/*
*/

#ifndef _MAXTRIX_H_
#define _MAXTRIX_H_


#ifdef __cplusplus
extern "C" {
#endif
	
void Matrix_Zero(float *A, unsigned short numRows, unsigned short numCols);
void Matrix_Copy(float *pSrc, unsigned short numRows, unsigned short numCols, float *pDst);
int Maxtrix_Add(float *pSrcA, unsigned short numRows, unsigned short numCols, float *pSrcB, float *pDst);
int Maxtrix_Sub(float *pSrcA, unsigned short numRows, unsigned short numCols, float *pSrcB, float *pDst);
int Matrix_Multiply(float* pSrcA, unsigned short numRowsA, unsigned short numColsA, float* pSrcB, unsigned short numColsB, float* pDst);
void Matrix_Multiply_With_Transpose(float *A, unsigned short nrows, unsigned short ncols, float *B, unsigned short mrows, float *C);
void Maxtrix_Transpose(float *pSrc, unsigned short nRows, unsigned short nCols, float *pDst);
int Matrix_Inverse(float* pDst, unsigned short n, float * pSrc);
	
#ifdef __cplusplus
}
#endif	
	
#endif
