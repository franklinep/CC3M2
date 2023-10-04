#include <bits/stdc++.h>
using namespace std;

int const M = INT16_MAX;
////////////////////////////MAXIMIZACION SIMPLE/////////////////////////////////
void fillTable(int row, int col, double ** table){
    for (int i=0; i < row; ++i)
    {
        for(int j = 0; j < col; ++j){
            cin >> table[i][j];
        }
    }
}
pair<int,int> indexPivot(int n, int row, int col, double ** table){
	pair<int,int> index;
	// * COLUMNA
	// Buscamos el mayor negativo entre x1,x2,x3,xn
	int last = row-1;
	double max = 0;
	for (int j = 0; j < n; ++j)
	{
		if(table[last][j] < max){
			max = table[last][j];
			index.second = j;
		}
	}
	// * FILA
	// El menor numero positivo al dividir (biq/aiq)
	int j_b = col-1, j_pivot = index.second;
	double min = INT_MAX;
	for (int i = 0; i < row - 1; ++i)
	{
		if(table[i][j_pivot] > 0){
			double r = table[i][j_b]/table[i][j_pivot];
			if( r < min){
				min = r;
				index.first = i;
			}
		}
	}

	return index;
}
bool negativos(int n, int row, double ** &table){
	int last = row-1;
	for (int j = 0; j < n; ++j)
		if(table[last][j] < 0) return true;

	return false;
}
void printTable(int row, int col, double ** table){
    for(int i=0; i < row; i++){
        for(int j=0;j<col;j++){
            cout << right << setw(10) << table[i][j];
        }
        cout << "\n";
    }
}
void conversionTable(int row, int col, int ipiv, int jpiv,double **table){
    double r;
    for (int i = 0; i < row; ++i)
    {
        if(i!=ipiv){
            r = table[i][jpiv];
            for (int j = 0; j < col; ++j)
            {
                table[i][j] -= r * table[ipiv][j];
            }
        }
    }
}
void simplexMethodMaximize(){
	double pivot;
    int n, m;
	cout << "Numero variables: "; cin >> n;
	cout << "Numero de restricciones: "; cin >> m;
	// 1. Rellenamos nuestra table[][]
	int row = m + 1, col = m + n + 1;
	auto** table = new double*[row];
    for (int i = 0; i < row; ++i) {
        table[i] = new double[col];
    }
    fillTable(row, col, table);
    printTable(row, col, table);
	cout << "\nIngresando al bucle.....\n";
	// 2. Bucle
	do{
		// Indentificamos nuestro pivote
        pair<int,int> mypair = indexPivot(n, row, col, table);
		int ipiv = mypair.first;
		int jpiv = mypair.second;
		pivot = table[ipiv][jpiv];
        cout << "Nuestro pivote[" << ipiv << "]" << "[" << jpiv << "] = " << pivot << endl;
		// Convertimos a la unidad el pivote mediante operaciones filas
		for (int j = 0; j < col; ++j) table[ipiv][j] /= pivot;
		// Convertimos 0's en la columna del pivote mediante operaciones fila
		// OP : filaActual - razon*filaPivote
        conversionTable(row, col, ipiv, jpiv, table);
		cout << "\nActualizando tabla ..........\n";
		printTable(row, col, table);
	}while(negativos(n, row, table));
    // Liberar la memoria asignada
    for (int i = 0; i < row; ++i) {
        delete[] table[i];
    }
    delete[] table;
}
//////////////////////////////MINIMIZACION SIMPLE/////////////////////////////////
pair<int,int> indexPivot_2(int row, int col, double **table, double *&arr){
    int last = row-1;
    double diff;
    double min = INT16_MAX;
    pair<int,int> index;
    // COLUMNA DEL PIVOTE
    for (int j = 0; j < col-1; ++j) {
        diff = table[last][j];
        for (int i = 0; i < row-1; ++i) {
            diff = diff - table[i][j]*arr[i];
        }
        if (diff < min){
            min = diff;
            index.second = j;
        }
    }
    // FILA DEL PIVOTE
    // El menor numero positivo al dividir (biq/aiq)
    int j_b = col-1, j_pivot = index.second;
    min = INT16_MAX;
    for (int i = 0; i < row - 1; ++i)
    {
        if(table[i][j_pivot] > 0){
            double r = table[i][j_b]/table[i][j_pivot];
            if( r < min){
                min = r;
                index.first = i;
            }
        }
    }
    // Cambiamos los valores de los coeficientes basicos
    arr[index.first] = table[last][index.second];
    return index;
}
bool negativos_2(int row, int col, double ** &table, double *arr){
    int last = row-1;
    for (int j = 0; j < col-1; ++j){
        double diff = table[last][j];
        for (int i = 0; i < row-1; ++i) {
            diff = diff - table[i][j]*arr[i];
        }
        if(diff < 0) return true;
    }

    return false;
}
void simplexMethodMinimize(){
    double pivot;
    int n, m;
    cout << "Numero de Variables: "; cin >> n;
    cout << "Numero de Restricciones: "; cin>> m;
    int row = m + 1, col = n + 2*m + 1;
    auto** table = new double*[row];
    for (int i = 0; i < row; ++i) {
         table[i] = new double[col];
    }
    // Array de los coeficientes Basicos
    auto *arr = new double[m];
    for (int i = 0; i < m; ++i) arr[i] = M;
    // Rellenamos nuestra table[][]
    fillTable(row, col, table);
    //table[0][0] = 1, table[0][1] = 4, table[0][2] = -1, table[0][3] = 1, table[0][4] = 0, table[0][5] = 0, table[0][6] = 3.5;
    //table[1][0] = 1, table[1][1] = 2, table[1][2] = 0, table[1][3] = 0, table[1][4] = -1, table[1][5] = 1, table[1][6] = 2.5;
    //table[2][0] = 3, table[2][1] = 8, table[2][2] = 0, table[2][3] = M, table[2][4] = 0, table[2][5] = M, table[2][6] = 0;
    /*
    table = [[1,4,-1,1,0,0,3.5],
             [1,2,0,0,-1,1,2.5],
             [3,8,0,M,0,M,0]
             ];
             */
    printTable(row, col, table);
    do {
        // Indentificamos nuestro pivote
        pair<int,int> mypair =  indexPivot_2(row, col, table, arr);
        int ipiv = mypair.first;
        int jpiv = mypair.second;
        cout << "\nNuestro pivote[" << ipiv << "]" << "[" << jpiv << "] = " << pivot << endl;
        pivot = table[ipiv][jpiv]; cout << pivot;
        // Convertimos a la unidad el pivote mediante operaciones filas
        for (int j = 0; j < col; ++j) table[ipiv][j] /= pivot;
        // Convertimos 0's en la columna del pivote mediante operaciones fila, excepto la ultima fila
        // OP : filaActual - razon*filaPivote2
        conversionTable(row-1, col, ipiv, jpiv, table);
        cout << "\nActualizando tabla ..........\n";
        printTable(row, col, table);
    }while(negativos_2(row, col, table, arr));
    // Liberar la memoria asignada
     for (int i = 0; i < row; ++i) {
         delete[] table[i];
     }
     delete[] table;
     delete[] arr;
}
/////////////////////////////MINIMIZACION MIXTA/////////////////////////////////
pair<int,int> indexPivot_3(int row, int col, double **table, double *&arr){
    int last = row-1;
    double diff;
    double max = -1;
    pair<int,int> index;
    // COLUMNA DEL PIVOTE
    for (int j = 0; j < col-1; ++j) {
        diff = table[last][j];
        cout << diff;
        for (int i = 0; i < row-1; ++i) {
            diff = diff - table[i][j]*arr[i];
        }
        if (diff > max){
            max = diff;
            index.second = j;
        }
    }
    // FILA DEL PIVOTE
    // El menor numero positivo al dividir (biq/aiq)
    int j_b = col-1, j_pivot = index.second;
    double min = INT16_MAX;
    for (int i = 0; i < row - 1; ++i)
    {
        if(table[i][j_pivot] > 0){
            double r = table[i][j_b]/table[i][j_pivot];
            if( r < min){
                min = r;
                index.first = i;
            }
        }
    }
    // Cambiamos las variables basis anteriores por los nuevos "Ci"
    arr[index.first] = table[last][index.second];
    return index;
}
bool positivos(int row, int col, double ** &table, double *arr){
    int last = row-1;
    for (int j = 0; j < col-1; ++j){
        double diff = table[last][j];
        for (int i = 0; i < row-1; ++i) {
            diff = diff - table[i][j]*arr[i];
        }
        if( diff > 0) return true;
    }
    return false;
}
void simplexMethod2Minimize(){
    double pivot;
    int n;
    int m1,m2,m3;
    cout << "Numero de variables: "; cin >> n;
    cout << "Restricciones \"<=\" :"; cin >> m1;
    cout << "Restricciones \">=\" :"; cin >> m2;
    cout << "Restricciones \"=\" :"; cin >> m3;
    int row, col;
    row = m1 + m2 + m3 + 1; col = n + m1 + 2*m2 + m3 + 1;
    auto** table = new double*[row];
    for (int i = 0; i < row; ++i) {
        table[i] = new double[col];
    }
    cout << "FILAS Y COLUMNAS : " << row << "\t" << col << endl;
    // Array de los coeficientes Basicos
    auto *arr = new double[row-1];
    arr[0] = 0, arr[1] = -M, arr[2] = -M;
    // Rellenamos nuestra table[][]
    //fillTable(row, col, table);
    table[0][0] = 2, table[0][1] = 5; table[0][2] = -1, table[0][3] = 1, table[0][4] = 0, table[0][5] = 0, table[0][6] = 0, table[0][7] = 18;
    table[1][0] = -1, table[1][1] = 1, table[1][2] = 2, table[1][3] = 0, table[1][4] = -1, table[1][5] = 1, table[1][6] = 0; table[1][7] = 14;
    table[2][0] = 3, table[2][1] = 2, table[2][2] = 2, table[2][3] = 0, table[2][4] = 0, table[2][5] = 0, table[2][6] = 1; table[2][7] = 26;
    table[3][0] = 6, table[3][1] = -7, table[3][2] = -4, table[3][3] = 0, table[3][4] = 0, table[3][5] = -M, table[3][6] = -M, table[3][7] = 0;
    //IMPRESION DE LA TABLA
    printTable(row, col, table);
    do {
        // Indentificamos nuestro pivote
        pair<int,int> mypair =  indexPivot_3(row, col, table, arr);
        int ipiv = mypair.first;
        int jpiv = mypair.second;
        cout << "\nNuestro pivote[" << ipiv << "]" << "[" << jpiv << "] = " << pivot << endl;
        pivot = table[ipiv][jpiv]; cout << pivot;
        // Convertimos a la unidad el pivote mediante operaciones filas
        for (int j = 0; j < col; ++j) table[ipiv][j] /= pivot;
        // Convertimos 0's en la columna del pivote mediante operaciones fila, excepto la ultima fila
        // OP : filaActual - razon*filaPivote
        conversionTable(row-1, col, ipiv, jpiv, table);
        cout << "\nActualizando tabla ..........\n";
        printTable(row, col, table);
    }while(positivos(row, col, table, arr));
    table[3][7] = 6*table[2][7] - 4*table[1][7];
    cout << "\n--------RESULTADO FINAL --------\n";
    printTable(row,col,table);
    // Liberar la memoria asignada
    for (int i = 0; i < row; ++i) {
        delete[] table[i];
    }
    delete[] table;
    delete[] arr;
}

/** @author = Frankin
 * date 18/09/23
 * */
int main(){
	int opt;
    cout << M;
	do{
		cout << "\n=====Simplex Method====\n";
		cout << "1.Maximizar >=\n2.Minimizar <=\n3.Minimizar Mixto (<=, >=, =)\n4.Salir\nElige una opcion: ";
		cin >> opt;
		switch(opt){
			case 1:
                simplexMethodMaximize();
				break;
			case 2:
                simplexMethodMinimize();
				break;
            case 3:
                simplexMethod2Minimize();
                break;
			case 4:
				cout << "Have a good day!\n";
				system("cls");
				break;
			default:
				cout << "Invalid option.\n";
		}
	}while(opt != 4);
}