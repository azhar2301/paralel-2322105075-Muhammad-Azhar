#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <chrono>
#include <string>
#include <algorithm>

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
// SOBEL EDGE DETECTION - SERIAL
// =====================================

void sobelSerial(
    const vector<unsigned char>& input,
    vector<unsigned char>& output,
    int width,
    int height
) {
    // Kernel Sobel X
    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    // Kernel Sobel Y
    int Gy[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    // Memproses seluruh pixel gambar
    for (int y = 0; y < height; y++) {

        for (int x = 0; x < width; x++) {

            int sumX = 0;
            int sumY = 0;

            // Operasi konvolusi 3x3
            for (int ky = -1; ky <= 1; ky++) {

                for (int kx = -1; kx <= 1; kx++) {

                    int pixelX = x + kx;
                    int pixelY = y + ky;

                    int pixel = 0;

                    // Zero-padding pada batas gambar
                    if (
                        pixelX >= 0 &&
                        pixelX < width &&
                        pixelY >= 0 &&
                        pixelY < height
                    ) {
                        pixel = input[pixelY * width + pixelX];
                    }

                    sumX += pixel * Gx[ky + 1][kx + 1];
                    sumY += pixel * Gy[ky + 1][kx + 1];
                }
            }

            // Menghitung kekuatan tepi
            double magnitude = sqrt(
                (double)sumX * sumX +
                (double)sumY * sumY
            );

            // Membatasi nilai pixel maksimal 255
            magnitude = min(magnitude, 255.0);

            output[y * width + x] =
                static_cast<unsigned char>(magnitude);
        }
    }
}

// =====================================
// PROGRAM UTAMA
// =====================================

int main() {

    string inputFile = "input/foto1.pgm";
    string outputFile = "output/sobel_serial.pgm";

    int width, height;

    vector<unsigned char> input;
    vector<unsigned char> output;

    cout << "=================================\n";
    cout << "   SOBEL EDGE DETECTION SERIAL\n";
    cout << "=================================\n";

    // Membaca gambar input
    if (!readPGM(inputFile, input, width, height)) {
        return 1;
    }

    cout << "Gambar berhasil dibaca!\n";
    cout << "Lebar  : " << width << " pixel\n";
    cout << "Tinggi : " << height << " pixel\n";

    output.resize(width * height);

    // Mengukur waktu proses Sobel
    auto start = high_resolution_clock::now();

    sobelSerial(input, output, width, height);

    auto stop = high_resolution_clock::now();

    double executionTime =
        duration<double, milli>(stop - start).count();

    // Menyimpan hasil gambar
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