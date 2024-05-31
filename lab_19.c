#include "lab_19.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

void transposeMatrices(FILE* input, FILE* output) {
    int matrixOrder;

    while (fscanf(input, "%d", &matrixOrder) == 1) {
        int matrix[matrixOrder][matrixOrder];

        for (int i = 0; i < matrixOrder; ++i) {
            for (int j = 0; j < matrixOrder; ++j) {
                fscanf(input, "%d", matrix[i] + j);
            }
        }

        fprintf(output, "%d\n", matrixOrder);

        for (int i = 0; i < matrixOrder; ++i) {
            for (int j = 0; j < matrixOrder; ++j) {
                fprintf(output, "%d ", matrix[j][i]);
            }
        }

        fprintf(output, "\n");
    }
}

void trimFloats(FILE* input, FILE* output) {
    double number;

    while (fscanf(input, "%lf", &number) == 1) {
        fprintf(output, "%.2e\n", number);
    }
}

static int executeOperation(int leftOperand, int rightOperand, char operator) {
    switch (operator) {
        case '+': {
            return leftOperand + rightOperand;
        }
        case '-': {
            return leftOperand - rightOperand;
        }
        case '*': {
            return leftOperand * rightOperand;
        }
        case '/': {
            return leftOperand / rightOperand;
        }
        default: {
            fprintf(stderr, "invalid operator '%c'", operator);
            exit(EXIT_FAILURE);
        }
    }
}

void solveSentence(FILE* file) {
    int operands[3];
    char operators[2];

    int currentChar;

    int operandIndex = 0, operatorIndex = 0;

    while ((currentChar = fgetc(file)) != EOF) {
        if (isspace(currentChar)) continue;

        if (isdigit(currentChar)) {
            operands[operandIndex++] = currentChar - '0';
        } else {
            operators[operatorIndex++] = (char) currentChar;
        }
    }

    int result;

    if (operandIndex == 2) {
        result = executeOperation(operands[0], operands[1], operators[0]);
    } else {
        if (operators[1] == '/' || operators[1] == '*') {
            result = executeOperation(
                    operands[0], executeOperation(operands[1], operands[2], operators[1]), operators[0]
            );
        } else {
            result = executeOperation(
                    executeOperation(operands[0], operands[1], operators[0]), operands[2], operators[1]
            );
        }
    }

    fprintf(file, "=%d", result);
}

void filterWordsByContent(FILE* input, FILE* output, char* content) {
    char line[100];

    while (fgets(line, 100, input) != NULL) {
        char* word = strtok(line, " ");

        while (word != NULL) {
            if (strstr(word, content)) {
                fprintf(output, "%s ", word);
            }

            word = strtok(NULL, " ");
        }

        printf(" ");
    }
}

void filterOnlyLongestWords(FILE* input, FILE* output) {
    char line[100];

    while (fgets(line, 100, input) != NULL) {
        char* word = strtok(line, " ");
        char* longestWord = word;
        size_t biggestLength = strlen(longestWord);

        word = strtok(NULL, " ");

        while (word != NULL) {
            size_t length = strlen(word);

            if (length > biggestLength) {
                longestWord = word;
                biggestLength = length;
            }

            word = strtok(NULL, " ");
        }

        fprintf(output, "%s\n", longestWord);
    }
}

typedef struct {
    int coefficient;
    int power;
} PolynomialMember;

typedef struct {
    int freeMember;
    int memberAmount;
    PolynomialMember members[];
} Polynomial;

static int calculatePolynomialMember(PolynomialMember member, int root) {
    return member.coefficient * (int) pow(root, member.power);
}

static int calculatePolynomial(Polynomial polynomial, int root) {
    int result = 0;

    for (int i = 0; i < polynomial.memberAmount; ++i) {
        result += calculatePolynomialMember(polynomial.members[i], root);
    }

    return result + polynomial.freeMember;
}

void removeSentencesWithRoot(FILE* input, FILE* output, int root) {
    Polynomial polynomial;

    while (fread(&polynomial, sizeof(polynomial), 1, input) == 1) {
        if (calculatePolynomial(polynomial, root) != 0) {
            fwrite(&polynomial, sizeof(polynomial), 1, output);
        }
    }
}

void sortNumbersByPositivity(FILE* input, FILE* output) {
    fseek(input, 0, SEEK_END);
    long fileSize = ftell(input);
    rewind(input);
    int numberAmount = fileSize / sizeof(int);
    int* numbers = malloc(fileSize);

    fread(numbers, sizeof(int), numberAmount, input);

    for (int i = 0; i < numberAmount; ++i) {
        int number = numbers[i];

        if (numbers > 0) {
            fprintf(output, "%d\n", number);
        }
    }

    for (int i = 0; i < numberAmount; ++i) {
        int number = numbers[i];

        if (numbers < 0) {
            fprintf(output, "%d\n", number);
        }
    }
}

typedef struct {
    int** cells;
    int rows;
    int columns;
} Matrix;

static bool isSymmetricMatrix(Matrix* matrix) {
    for (int i = 0; i < matrix->rows; i++) {
        int* row = matrix->cells[i];

        for (int j = 0; j < matrix->columns; j++) {
            if (i == j) {
                continue;
            }

            if (row[j] != matrix->cells[j][i]) {
                return false;
            }
        }
    }

    return true;
}

static int** allocateCells(int rows, int columns) {
    int** cells = (int**) malloc(sizeof(int*) * rows);

    for (int i = 0; i < rows; i++) {
        cells[i] = (int*) malloc(sizeof(int) * columns);
    }

    return cells;
}

static void transposeSquareMatrix(Matrix* matrix) {
    int** newCells = allocateCells(matrix->rows, matrix->columns);

    for (int i = 0; i < matrix->rows; i++) {
        int* row = matrix->cells[i];

        for (int j = 0; j < matrix->columns; j++) {
            newCells[j][i] = row[j];
        }
    }

    matrix->cells = newCells;
}

void transposeNonSymmetricalMatrices(FILE* input, FILE* output) {
    Matrix matrix;

    while (fread(&matrix, sizeof(Matrix), 1, input) == 1) {
        if (isSymmetricMatrix(&matrix)) {
            transposeSquareMatrix(&matrix);
        }

        fwrite(&matrix, sizeof(Matrix), 1, output);
    }
}

typedef struct {
    char* name;
    char* surname;
    char* patronymic;
    int bestResult;
} Sportsman;

static int compareSportsmen(const void* left, const void* right) {
    return *((const int*) left) - *((const int*) right);
}

void collectTheBestTeam(FILE* input, FILE* output, int teamSize) {
    fseek(input, 0, SEEK_END);
    long fileSize = ftell(input);
    rewind(input);
    int sportsmanAmount = fileSize / sizeof(Sportsman);
    Sportsman* sportsmen = malloc(fileSize);

    fread(sportsmen, sizeof(Sportsman), sportsmanAmount, input);

    qsort(sportsmen, sizeof(Sportsman), sportsmanAmount, compareSportsmen);

    fwrite(sportsmen, sizeof(Sportsman), teamSize, output);
}

typedef struct {
    char name[50];
    float price;
    float totalCost;
    int amount;
} Product;

typedef struct {
    char name[50];
    int amount;
} Order;

void filterProducts(FILE* productsFile, FILE* ordersFile) {
    Product product;
    Order order;

    while (fread(&product, sizeof(Product), 1, productsFile)) {
        fseek(ordersFile, 0, SEEK_SET);
        bool found = false;

        while (fread(&order, sizeof(Order), 1, ordersFile)) {
            if (strcmp(product.name, order.name) == 0) {
                found = true;
                if (order.amount < product.amount) {
                    product.amount -= order.amount;
                    product.totalCost -= product.price * order.amount;
                } else {
                    product.amount = 0;
                    product.totalCost = 0;
                    break;
                }
            }
        }

        if (!found) {
            fseek(productsFile, -sizeof(Product), SEEK_CUR);
        } else {
            fwrite(&product, sizeof(Product), 1, productsFile);
        }
    }
}