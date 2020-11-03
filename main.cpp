
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>

static const unsigned int testCase = 2;

void printMatrix(float* matrix, unsigned const int rows, unsigned const int cols);
extern "C" void quickMatrixMul(float* matrixNM, float* matrixMP, float* matrixNP, unsigned n, unsigned m, unsigned p);

/**
%rdi = matrixNM
%rsi = matrixMP
%rdx = matrixNP
%rcx = n
%r8 = m
%r9 = p
*/
#if 0
void quickMatrixMulSSE(float* matrixNM, float* matrixMP, float* matrixNP, unsigned n, unsigned m, unsigned p){
    unsigned int i, j, k;
    float r = 0;
    for (k = 0; k < m; ++k) {                   // L1
        for (i = 0; i < n; ++i) {               // L2
            r = matrixNM[i*m + k];
            for (j = 0; j < p; j += 4)          // L3
            {
				if(p - j < 4){
					for(; j < p; ++j)
						movss
				}else{
					movaps
					matrixNP[i*p + j] += r * matrixMP[k*p + j];
				}
			}
        }
    }
}
#endif

#if 0
void quickMatrixMul(float* matrixNM, float* matrixMP, float* matrixNP, unsigned n, unsigned m, unsigned p){
    unsigned int i, j, k;
    float r = 0;
    for (k = 0; k < m; ++k) {               // L1
        for (i = 0; i < n; ++i) {           // L2
            r = matrixNM[i*m + k];
            for (j = 0; j < p; j += 4){         // L3
                matrixNP[i*p + j] += r * matrixMP[k*p + j];
                matrixNP[i*p + j+1] += r * matrixMP[k*p + j+1];
                matrixNP[i*p + j+2] += r * matrixMP[k*p + j+2];
                matrixNP[i*p + j+3] += r * matrixMP[k*p + j+3];
            }
        }
    }
}
#endif

#if 0
/**
%rdi = matrixNM
%rsi = matrixMP
%rdx = matrixNP
%rcx = n
%r8 = m
%r9 = p
*/
void quickMatrixMul(float* matrixNM, float* matrixMP, float* matrixNP, unsigned n, unsigned m, unsigned p){
    unsigned int i, j, k;
    float r = 0;
    for (k = 0; k < m; ++k) {               // L1
        for (i = 0; i < n; ++i) {           // L2
            r = matrixNM[i*m + k];
            for (j = 0; j < p; ++j)         // L3
                matrixNP[i*p + j] += r * matrixMP[k*p + j];
        }
    }
}
#endif

#if 0
// Testing
// m1Rows, m1Columns, m2Columns			
void quickMatrixMul(float* matrixNM, float* matrixMP, float* matrixNP, unsigned n, unsigned m, unsigned p){
    unsigned int i, j, k;
	float r = 0;
        //k representa la suma (que es la multiplicacion de las dos celdas respectivas de las matrices operando) que se va agregando a cada celda en la matriz resultante
        //k = 0 es la primera suma de cada celda resultante. k = 1 es la segunda suma de cada celda resultante, ...
	for (k = 0; k < m; ++k) {
		std::cout << "Iteracion k = " << k << std::endl << std::endl;
                //i representa la fila de la primera matriz
		for (i = 0; i < n; ++i) {
			// Para cada elemento fijo (i, k) en la matriz matrixNM 
			r = matrixNM[i*m + k];
			std::cout << "r = Matrix NM[" << i << "][" << k << "] = " << r << std::endl;
                        //j representa la columna de la segunda matriz
			for (j = 0; j < p; ++j){
				matrixNP[i*p + j] += r * matrixMP[k*p + j];
				//std::cout << "Matrix MP[" << k << "][" << j << "] = " << matrixMP[k*p + j] << std::endl;
				std::cout << "Matrix NP[" << i << "][" << j << "] = " << r << "*" << matrixMP[k*p + j] << std::endl;
			}
			std::cout << "\nResult:\n";
			printMatrix(matrixNP, n, p);
			std::cout << "\n\n";
		}
	} 
}
//C[i*N+j] += A[i*N+k]*B[k*N+j];
#endif


float* loadMatrix(const char* filename, unsigned int &rows, unsigned int &cols){
    std::ifstream input(filename);
    if(!input)
        throw 1;

    input >> rows;
    input.ignore();
    input >> cols;
    input >> std::ws;

    float* matrix = new float[rows*cols];
    if(!matrix)
        throw 2;

    for(size_t row = 0; row < rows; ++row){
        for(size_t column = 0; column < cols; ++column){
            input >> matrix[row*cols + column];
            input.ignore();
        }
    }

    input.close();

    return matrix;
}

bool compare(float* matrixA, float* matrixB, unsigned int elementos){
    for(size_t index = 0; index < elementos; ++index){
        if(matrixA[index] != matrixB[index])
            return false;
    }
    return true;
}

void printMatrix(float* matrix, unsigned const int rows, unsigned const int cols){
    std::cout << std::setprecision(2) << std::setiosflags(std::ios::fixed|std::ios::showpoint);
    for(size_t row = 0; row < rows; ++row){
        for(size_t column = 0; column < cols; ++column)
            std::cout << std::setw(8) << matrix[row*cols + column];
        std::cout << std::endl;
    }
}

int main()
{
    char buffer[64];
    // A matrix
    unsigned int m1Rows = 0;
    unsigned int m1Columns = 0;
    sprintf(buffer, "Casos_prueba/case%u_matrix%u.txt", testCase, 1);
    float* m1;
    try {
		m1 = loadMatrix(buffer, m1Rows, m1Columns);
	} catch (int error) {
		std::cerr << "error: could not open filename or not enough memory" << std::endl;
		return 0;
	}

	std::cout << "MATRIZ 1 NM:" << std::endl;
    printMatrix(m1, m1Rows, m1Columns);
	
    // B matrix
    unsigned int m2Rows = 0;
    unsigned int m2Columns = 0;
    sprintf(buffer, "Casos_prueba/case%u_matrix%u.txt", testCase, 2);
    float* m2;
    try {
		m2 = loadMatrix(buffer, m2Rows, m2Columns);
	} catch (int error) {
		std::cerr << "error: not enough memory or incorrect filename" << std::endl;
		delete [] m1;
		return 0;
	}
	
	std::cout << "\nMATRIZ 2 MP:" << std::endl;
	printMatrix(m2, m2Rows, m2Columns);

    // C matrix result of A x B
    unsigned int elements = m1Rows * m2Columns;
    float* myResult = new float[elements]();
    quickMatrixMul(m1, m2, myResult, m1Rows, m1Columns, m2Columns); // N, M, P
    
    std::cout << "\nMATRIZ RESULTADO NP:" << std::endl;
    printMatrix(myResult, m1Rows, m2Columns);

    // Output matrix to be compared to
    sprintf(buffer, "Casos_prueba/case%u_output.txt", testCase);
    float* output;
    try {
		output = loadMatrix(buffer, m1Rows, m2Columns);
	} catch (int error) {
		std::cerr << "error: not enough memory or incorrect filename" << std::endl;
		delete [] m1;
		delete [] m2;
		return 0;
	}

    if(compare(myResult, output, elements))
        std::cout << "¡SON IGUALES!\n";
    else
        std::cout << "¡NO SON IGUALES. ALERTA\n!";

	delete [] m1;
    delete [] m2;
    delete [] myResult;
    delete [] output;

    return 0;
}
