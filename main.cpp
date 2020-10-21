#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>

static const unsigned int testCase = 0;

//extern "C" void quickMatrixMul(float* matrixNM, float* matrixMP, float* matrixNP, unsigned n, unsigned m, unsigned p);

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

   // printMatrix(m1, m1Rows, m1Columns);

#if 1
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

    // C matrix result of A x B
    unsigned int elements = m1Rows * m2Columns;
    float* myResult = new float[elements]();
    //quickMatrixMul(m1, m2, myResult, m1Rows, m1Columns, m2Columns); // N, M, P

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
        std::cout << "¡SON IGUALES\n!";
    else
        std::cout << "¡NO SON IGUALES. ALERTA\n!";

	delete [] m1;
    delete [] m2;
    delete [] myResult;
    delete [] output;
#endif
    delete [] m1;

    return 0;
}
