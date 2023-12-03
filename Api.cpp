#include <iostream>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <iomanip>
#include <omp.h> // Incluir la cabecera de OpenMP

using namespace cv;
using namespace std;

// Función para convertir un píxel a escala de grises utilizando el método de luminosidad.
void convertToGrayscale(Point3_<uchar>* p, uchar& grayscaleValue) {
    // Método de luminosidad: Y = 0.299*R + 0.587*G + 0.114*B
    grayscaleValue = 0.299 * p->z + 0.587 * p->y + 0.114 * p->x;
}

// Función que procesa toda la imagen en escala de grises utilizando OpenMP.
void processImage(Mat& image) {
    #pragma omp parallel for
    for (int r = 0; r < image.rows; r++) {
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
        cout << "Uso: " << argv[0] << " <nombre_imagen_entrada> <nombre_imagen_salida>" << endl;
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

    // Medir el tiempo de inicio
    auto start_time = chrono::high_resolution_clock::now();

    // Procesar toda la imagen en escala de grises utilizando OpenMP.
    cout << "Inicia conversion..." << endl;
    processImage(image);

    // Medir el tiempo de finalización
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::duration<double>>(end_time - start_time);

    cout << "Termina la conversion..." << endl;
    cout << fixed << setprecision(6);
    cout << "Total tiempo en segundos: " << duration.count() << endl;

    // Guardar la imagen procesada.
    imwrite(argv[2], image);

    cout << "Imagen procesada y guardada como " << argv[2] << endl;

    return 0;  // This is the correct location for return 0;
}
