#include <opencv2/opencv.hpp>

int main() {
    cv::Mat image = cv::imread("gato.jpeg");
    if (image.empty()) {
        std::cerr << "Error al cargar la imagen. Asegúrate de que la ruta de la imagen sea correcta." << std::endl;
        return -1;
    }
  
    cv::imshow("Imagen de prueba", image);

    cv::waitKey(0);

    return 0;
}
