#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <chrono>
#include <string>
#include <algorithm>
#include <pthread.h>
#include <cstdlib>

using namespace std;
using namespace chrono;

// =====================================
// MEMBACA GAMBAR PGM
// =====================================

bool readPGM(
    const string& filename,
    vector<unsigned char>& image,
    int& width,
    int& height
) {
    ifstream file(filename, ios::binary);

    if (!file.is_open()) {
        cerr << "Gagal membuka gambar!\n";
        return false;
    }

    string format;
    file >> format;

    if (format != "P5") {
        cerr << "Format gambar harus PGM P5!\n";
        return false;
    }

    int maxValue;
    file >> width >> height >> maxValue;
    file.get();

    if (maxValue != 255 || width <= 0 || height <= 0) {
        cerr << "Header PGM tidak valid!\n";
        return false;
    }

    image.resize(width * height);

    file.read(
        reinterpret_cast<char*>(image.data()),
        image.size()
    );

    if (!file) {
        cerr << "Gagal membaca data pixel!\n";
        return false;
    }

    return true;
}

// =====================================
// MENYIMPAN GAMBAR PGM
// =====================================

bool savePGM(
    const string& filename,
    const vector<unsigned char>& image,
    int width,
    int height
) {
    ofstream file(filename, ios::binary);

    if (!file.is_open()) {
        cerr << "Gagal menyimpan gambar!\n";
        return false;
    }

    file << "P5\n";
    file << width << " " << height << "\n";
    file << "255\n";

    file.write(
        reinterpret_cast<const char*>(image.data()),
        image.size()
    );

    return file.good();
}

// =====================================
// DATA UNTUK SETIAP THREAD
// =====================================

struct ThreadData {
    const unsigned char* input;
    unsigned char* output;

    int width;
    int height;

    int startRow;
    int endRow;
};

// =====================================
// FUNGSI SOBEL UNTUK THREAD
// =====================================

void* sobelThread(void* arg) {

    ThreadData* data = (ThreadData*)arg;

    const int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    const int Gy[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    // Setiap thread memproses baris yang ditentukan
    for (int y = data->startRow; y < data->endRow; y++) {

        for (int x = 0; x < data->width; x++) {

            int sumX = 0;
            int sumY = 0;

            for (int ky = -1; ky <= 1; ky++) {

                for (int kx = -1; kx <= 1; kx++) {

                    int pixelX = x + kx;
                    int pixelY = y + ky;

                    int pixel = 0;

                    // Zero-padding pada batas gambar
                    if (
                        pixelX >= 0 &&
                        pixelX < data->width &&
                        pixelY >= 0 &&
                        pixelY < data->height
                    ) {
                        pixel = data->input[
                            pixelY * data->width + pixelX
                        ];
                    }

                    sumX += pixel * Gx[ky + 1][kx + 1];
                    sumY += pixel * Gy[ky + 1][kx + 1];
                }
            }

            double magnitude = sqrt(
                (double)sumX * sumX +
                (double)sumY * sumY
            );

            magnitude = min(magnitude, 255.0);

            data->output[y * data->width + x] =
                static_cast<unsigned char>(magnitude);
        }
    }

    return nullptr;
}

// =====================================
// SOBEL EDGE DETECTION - PTHREADS
// =====================================

void sobelPthreads(
    const vector<unsigned char>& input,
    vector<unsigned char>& output,
    int width,
    int height,
    int numThreads
) {
    vector<pthread_t> threads(numThreads);
    vector<ThreadData> threadData(numThreads);

    for (int i = 0; i < numThreads; i++) {

        // Membagi baris gambar untuk setiap thread
        int startRow = i * height / numThreads;
        int endRow = (i + 1) * height / numThreads;

        threadData[i] = {
            input.data(),
            output.data(),
            width,
            height,
            startRow,
            endRow
        };

        int result = pthread_create(
            &threads[i],
            nullptr,
            sobelThread,
            &threadData[i]
        );

        if (result != 0) {
            cerr << "Gagal membuat thread!\n";
            exit(1);
        }
    }

    // Menunggu seluruh thread selesai
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], nullptr);
    }
}

// =====================================
// PROGRAM UTAMA
// =====================================

int main(int argc, char* argv[]) {

    int numThreads = 4;

    if (argc > 1) {
        numThreads = atoi(argv[1]);
    }

    if (numThreads < 1 || numThreads > 8) {
        cerr << "Jumlah thread harus antara 1 sampai 8!\n";
        return 1;
    }

    string inputFile = "input/foto1.pgm";
    string outputFile = "output/sobel_pthreads_" +
                        to_string(numThreads) + ".pgm";

    int width, height;

    vector<unsigned char> input;
    vector<unsigned char> output;

    cout << "=================================\n";
    cout << " SOBEL EDGE DETECTION PTHREADS\n";
    cout << "=================================\n";

    if (!readPGM(inputFile, input, width, height)) {
        return 1;
    }

    cout << "Gambar berhasil dibaca!\n";
    cout << "Lebar   : " << width << " pixel\n";
    cout << "Tinggi  : " << height << " pixel\n";
    cout << "Threads : " << numThreads << endl;

    output.resize(width * height);

    // Mengukur waktu proses termasuk pembuatan
    // thread dan menunggu seluruh thread selesai
    auto start = high_resolution_clock::now();

    sobelPthreads(
        input,
        output,
        width,
        height,
        numThreads
    );

    auto stop = high_resolution_clock::now();

    double executionTime =
        duration<double, milli>(stop - start).count();

    if (!savePGM(outputFile, output, width, height)) {
        return 1;
    }

    cout << "\nProses Sobel selesai!\n";
    cout << "Output         : " << outputFile << endl;
    cout << "Waktu eksekusi : "
         << executionTime << " ms\n";

    cout << "=================================\n";

    return 0;
}