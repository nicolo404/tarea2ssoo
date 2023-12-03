#include <iostream>
#include <opencv2/opencv.hpp>
#include <chrono>

using namespace cv;
using namespace std;
using namespace std::chrono;

int main(int argc, char** argv) {
    // Verificar la cantidad correcta de argumentos de línea de comandos .
    if (argc != 3) {
        cout << "Uso: " << argv[0] << " <imagen_a_color> <imagen_escala_gris>" << endl;
        return -1;
    }

    // Lee el archivo de imagen a color.
    Mat image = imread(argv[1], IMREAD_COLOR);

    // Verificar si la imagen se carga correctamente.
    if (image.empty()) {
        cout << "Error al cargar la imagen a color." << endl;
        return -1;
    }

    // Mostrar información sobre la imagen.
    cout << "Cargando imagen..." << endl;
    cout << "Filas (height): " << image.rows << endl;
    cout << "Columnas (width): " << image.cols << endl;

    // Iniciar el temporizador.
    auto start = high_resolution_clock::now();

    // Convertir la imagen al método de luminosidad.
    Mat luminosityImage(image.rows, image.cols, CV_8UC1);

    for (int r = 0; r < image.rows; r++) {
        for (int c = 0; c < image.cols; c++) {
            Vec3b pixel = image.at<Vec3b>(r, c);
            uchar grayValue = 0.21 * pixel[2] + 0.72 * pixel[1] + 0.07 * pixel[0];
            luminosityImage.at<uchar>(r, c) = grayValue;
        }
    }

    // Detener el temporizador.
    auto stop = high_resolution_clock::now();

    // Calcular y mostrar la duración del procesamiento de la imagen.
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Incia conversion..." << endl;
    cout << "Termina conversion..." << endl;
    cout << "Total tiempo en espera " << duration.count() / 1e6<< endl;

    // Guardar la imagen resultante en escala de grises.
    imwrite(argv[2], luminosityImage);

    // Mostrar la imagen resultante (opcional).
    imshow("Luminosity Image", luminosityImage);
    waitKey(0);


    return 0;
}