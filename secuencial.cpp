#include <iostream>
#include <opencv2/opencv.hpp>
#include <chrono>

using namespace cv;
using namespace std;
using namespace std::chrono;

int main() {
    // Lee el archivo de imagen.
    Mat image = imread("gato.jpeg", IMREAD_COLOR);

    // Verificar si la imagen se carga correctamente.
    if (image.empty()) {
        cout << "Error al cargar la imagen." << endl;
        return -1;
    }

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

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Tiempo de procesamiento: " << duration.count() << " microsegundos." << endl;

    imshow("Luminosity Image", luminosityImage);
    waitKey(0);

    return 0;
}