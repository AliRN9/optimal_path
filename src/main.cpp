#include"hed.h"

using namespace std;

const int inf = std::numeric_limits<int>::max();

void decodeFromPng(std::vector<unsigned char>& image, const char* filename, unsigned height, unsigned width) {
    /*
     * Функция для чтения png файла
     */
    std::vector<unsigned char> png;
    //load and decode
    unsigned error = lodepng::load_file(png, filename);
    if (!error) error = lodepng::decode(image, width, height, png);

    //if there's an error, display it
    if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

    //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ... 
}

void getBlueChannel(const std::vector<unsigned char>& image, int** arr, int rows, int cols) {
    /*
     * Функция, которая выделяет синий канал из одномерного вектора и
     * заполняет им двумерный массив
     */
    for (int i = 2, j = 0, k = 0; i < image.size(); i += 4, ++k) {
        if (k == cols) {
            k = 0; // обнуляем номер столбца
            ++j; // увеличиваем номер строки
        }
        arr[j][k] = int(image[i]);
    }
}
void printArray(int** arr, int rows, int cols) {
  
     // Функция для вывода массива в консоль
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j)
            cout << arr[i][j] << " ";
        cout << endl;
    }
}

void drawPath(const vector<int>& path, vector<unsigned char>& image, int rows, int cols, const char* output_file) {
    int i = 0, j = -1;
    int previousPoint = -1;
    for (auto& e : path) {
        if (previousPoint == e)
            ++i;
        else
            j = e;
        previousPoint = e;
        // draw red path R = 255, G = 0, B = 0
        int index = (i * cols + j) * 4;
        image[index] = (unsigned char)255;
        image[++index] = (unsigned char)0;
        image[++index] = (unsigned char)0;
    }



    //Encode the image
    unsigned error = lodepng::encode(output_file, image, cols, rows);

    //if there's an error, display it
    if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

void findOptimalPath(int** arr, vector<int>& path, int x_i, int x_f, int  rows, int cols) {
    //--------------------------------------------------------------------------------
    // Напишите ваш код здесь


    bool nevizit = false;
    Vertice** Arr_V = new Vertice * [rows];
    for (int i = 0; i < rows; ++i)
        Arr_V[i] = new Vertice[cols];

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            Arr_V[i][j].path_w = inf;
            Arr_V[i][j].vizit = false;
            Arr_V[i][j].early = none;
            Arr_V[i][j].mine_w = arr[i][j];
            //std::cout << i << " " << " " << j << " " << Arr_V[i][j].mine_w << std::endl;

        }
    }
    Arr_V[0][x_i].path_w = 0;


    while (!nevizit)
    {
        //int check_min = -1;
        int min_w = inf;
        int min_i = -1, min_j = -1;

        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                if (!Arr_V[i][j].vizit and (Arr_V[i][j].path_w < min_w))
                {
                    min_w = Arr_V[i][j].path_w;
                    min_j = j;//столбец
                    min_i = i;//строка
                    //std::cout << min_i << " " << " " << min_j << " " << min_w << std::endl;
                    //std::cout << i << " " << " " << j << " " << Arr_V[i][j].mine_w << std::endl;

                }
            }
        }
        if (min_i != -1)
        {
            //*std::cout <<i<< min_w;
            if ((min_i + 1 < rows) and !Arr_V[min_i + 1][min_j].vizit and (Arr_V[min_i + 1][min_j].path_w > Arr_V[min_i][min_j].path_w + Arr_V[min_i][min_j].mine_w))
            {
                Arr_V[min_i + 1][min_j].path_w = Arr_V[min_i][min_j].path_w + Arr_V[min_i][min_j].mine_w;
                Arr_V[min_i + 1][min_j].early = up;
                //arr_way[min_i + 1][min_j] = 2;
            }
            if ((min_j - 1 >= 0) and !Arr_V[min_i][min_j - 1].vizit and (Arr_V[min_i][min_j - 1].path_w > Arr_V[min_i][min_j].path_w + Arr_V[min_i][min_j].mine_w))
            {
                Arr_V[min_i][min_j - 1].path_w = Arr_V[min_i][min_j].path_w + Arr_V[min_i][min_j].mine_w;
                Arr_V[min_i][min_j - 1].early = rightt;
            }
            if ((min_j + 1 < cols) and !Arr_V[min_i][min_j + 1].vizit and (Arr_V[min_i][min_j + 1].path_w > Arr_V[min_i][min_j].path_w + Arr_V[min_i][min_j].mine_w))
            {
                Arr_V[min_i][min_j + 1].path_w = Arr_V[min_i][min_j].path_w + Arr_V[min_i][min_j].mine_w;
                Arr_V[min_i][min_j + 1].early = leftt;
            }
            Arr_V[min_i][min_j].vizit = true;

            //nevizit = true;
        }
        else
        {
            nevizit = true;
        }
    }
    int i = cols - 1;
    int j = x_f;
    std::vector<int> path_X;
    std::vector<int> path_Y;
    while (!(i == 0 and j == x_i))
    {
        path_X.push_back(i);
        path.push_back(j);
        switch (Arr_V[i][j].early) {
        case up:
            i -= 1;
            break;
        case rightt:
            j += 1;
            break;
        case leftt:
            j -= 1;
            break;
        }
    }
    path.push_back(j);
    std::reverse(path.begin(), path.end());

   
    for (int i = 0; i < rows; ++i)
        delete[] Arr_V[i];
        delete Arr_V;
  
    //--------------------------------------------------------------------------------C:\Users\Ali\source\repos\Find_optimal_path_problem\src\main.cpp
}

int main(int argc, char* argv[]) {

    if (argc != 7) {
        cerr << "Wrong input arguments" << endl;
        return -1;
    }
    int rows = stoi(argv[2]), cols = stoi(argv[3]); // определение строк и столбцов как у матрицы в математике
    int x_i = stoi(argv[4]), x_f = stoi(argv[5]);
    char* input_file = argv[1], * output_file = argv[6];
    // читаем png изображение
    std::vector<unsigned char> image; // vector to store value of pixels with all 4 channels RGBA
    decodeFromPng(image, input_file, rows, cols); // read png file

    // способ 3 стр. 115 создания двумерного массива
    // выделяем память

    int** arr = new int* [rows];
    for (int row = 0; row < rows; ++row)
        arr[row] = new int[cols];

    // инициализируем выделенный массив arr синим каналом нашего изображения
    getBlueChannel(image, arr, rows, cols);

    // можно опционально выводить массив в консоль для отладки 
    printArray(arr, rows, cols);

    //функцию ниже вам нужно реализовать
    //-----------------------------------------------------------------------------
    vector<int> path;  // вектор к которому нужно делать push_back для добавления точек пути 
    findOptimalPath(arr, path, x_i, x_f, rows, cols);
    //------------------------------------------------------------------------------

    for (int i = 0; i < path.size(); i++)
    {
        cout <<"sdaas" << path[i];
    }
     // далее можно отрисовать path используя фукцию ниже
    drawPath(path, image, rows, cols, output_file);


    // очищаем память
    for (int row = 0; row < rows; ++row)
        delete[] arr[row];
    delete[] arr;
    getchar();
}
