#include <iostream>
#include <thread>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

struct ThreadInfo {
    Mat* img;
    int startRow;
    int endRow;
};

void convertToGray(ThreadInfo info) {
    for (int i = info.startRow; i < info.endRow; ++i) {
        for (int j = 0; j < info.img->cols; ++j) {
            Vec3b pixel = info.img->at<Vec3b>(i, j);
            uchar gray = (pixel[0] + pixel[1] + pixel[2]) / 3;
            info.img->at<Vec3b>(i, j) = Vec3b(gray, gray, gray);
        }
    }
}

int main() {
    Mat image = imread("imagenacolor.jpg");

    if (image.empty()) {
        cerr << "Error al cargar la imagen." << endl;
        return -1;
    }

    cout << "Matriz original:" << endl;
    cout << image << endl;

    int numThreads = 4;
    int rowsPerThread = image.rows / numThreads;
    vector<thread> threads;
    vector<ThreadInfo> threadInfo;

    for (int i = 0; i < numThreads; ++i) {
        ThreadInfo info;
        info.img = &image;
        info.startRow = i * rowsPerThread;
        info.endRow = (i == numThreads - 1) ? image.rows : (i + 1) * rowsPerThread;
        threadInfo.push_back(info);

        threads.emplace_back(convertToGray, threadInfo[i]);
    }

    for (auto& t : threads) {
        t.join();
    }

    cout << "\nMatriz después de la conversión a escala de grises:" << endl;
    cout << image << endl;

    imwrite("imagen_procesada.jpg", image);

    return 0;
}
