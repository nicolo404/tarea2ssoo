#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include <vector>
#include <chrono>
#include <iomanip> // Necesario para setw y setprecision

using namespace cv;
using namespace std;

// Función para convertir un píxel a escala de grises utilizando el método de luminosidad.
void convertToGrayscale(Point3_<uchar>* p, uchar& grayscaleValue) {
    // Método de luminosidad: Y = 0.299*R + 0.587*G + 0.114*B
    grayscaleValue = 0.299 * p->z + 0.587 * p->y + 0.114 * p->x;
}

// Función que procesa una porción de la imagen en escala de grises utilizando hebras.
void processImagePart(Mat& image, int startRow, int endRow) {
    for (int r = startRow; r < endRow; r++) {
        for (int c = 0; c < image.cols; c++) {
            Point3_<uchar>* p = image.ptr<Point3_<uchar>>(r, c);
            uchar grayscaleValue;
            convertToGrayscale(p, grayscaleValue);
            p->x = p->y = p->z = grayscaleValue; // Establecer R, G, B al mismo valor de escala de grises
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Uso: " << argv[0] << " <nombre_imagen_entrada> <nombre_imagen_salida> <numero_hebras>" << endl;
        return -1;
    }

    // Lee el archivo de la imagen de entrada.
    cout << "Loading image..." << endl;
    Mat image = imread(argv[1], IMREAD_COLOR);

    // Verificar si la imagen se cargó correctamente.
    if (image.empty()) {
        cout << "Error al cargar la imagen" << endl;
        return -1;
    }

    // Mostrar información sobre la imagen.
    cout << "Rows (height): " << image.rows << endl;
    cout << "Cols (width): " << image.cols << endl;

    // Obtener el número de hebras desde la línea de comandos.
    int numThreads = atoi(argv[3]);

    // Calcular la cantidad de filas por cada hebra.
    int rowsPerThread = image.rows / numThreads;

    // Vector para almacenar las hebras.
    vector<thread> threads;

    // Medir el tiempo de inicio
    auto start_time = chrono::high_resolution_clock::now();

    // Crear las hebras y procesar las porciones de la imagen.
    cout << "Start conversion..." << endl;
    for (int i = 0; i < numThreads; i++) {
        int startRow = i * rowsPerThread;
        int endRow = (i == numThreads - 1) ? image.rows : (i + 1) * rowsPerThread;
        threads.emplace_back(processImagePart, ref(image), startRow, endRow);
    }

    // Esperar a que todas las hebras terminen.
    for (auto& t : threads) {
        t.join();
    }

    // Medir el tiempo de finalización
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::duration<double>>(end_time - start_time);

    cout << "End conversion..." << endl;
    cout << fixed << setprecision(6);
    cout << "Total time spent in seconds is " << duration.count() << endl;

    // Guardar la imagen procesada.
    imwrite(argv[2], image);

    cout << "Imagen procesada y guardada como " << argv[2] << endl;

    return 0;
}

