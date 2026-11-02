#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <chrono>
#include <string>
#include <algorithm>
#include <omp.h>

using namespace std;

// Membaca gambar PGM format P5
bool readPGM(const string& filename, vector<unsigned char>& image,
             int& width, int& height) {
    ifstream file(filename, ios::binary);

    if (!file.is_open()) {
        cerr << "Gagal membuka file: " << filename << endl;
        return false;
    }

    string format;
    int maxValue;

    file >> format;
    file >> width >> height;
    file >> maxValue;
    file.get();

    if (format != "P5") {
        cerr << "Format gambar harus PGM P5." << endl;
        return false;
    }

    image.resize(width * height);
    file.read(reinterpret_cast<char*>(image.data()),
              image.size());

    return file.good() || file.eof();
}

// Menyimpan gambar PGM format P5
bool savePGM(const string& filename,
             const vector<unsigned char>& image,
             int width, int height) {
    ofstream file(filename, ios::binary);

    if (!file.is_open()) {
        cerr << "Gagal menyimpan file: " << filename << endl;
        return false;
    }

    file << "P5\n";
    file << width << " " << height << "\n";
    file << "255\n";

    file.write(reinterpret_cast<const char*>(image.data()),
               image.size());

    return file.good();
}

// Proses Sobel menggunakan OpenMP
void sobelOpenMP(const vector<unsigned char>& input,
                 vector<unsigned char>& output,
                 int width, int height, int numThreads) {

    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int Gy[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    #pragma omp parallel for num_threads(numThreads) schedule(static)
    for (int y = 0; y < height; y++) {

        for (int x = 0; x < width; x++) {

            int sumX = 0;
            int sumY = 0;

            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {

                    int px = x + kx;
                    int py = y + ky;

                    // Zero-padding pada batas gambar
                    if (px >= 0 && px < width &&
                        py >= 0 && py < height) {

                        int pixel = input[py * width + px];

                        sumX += pixel * Gx[ky + 1][kx + 1];
                        sumY += pixel * Gy[ky + 1][kx + 1];
                    }
                }
            }

            double magnitude = sqrt(
                sumX * sumX + sumY * sumY
            );

            magnitude = min(magnitude, 255.0);

            output[y * width + x] =
                static_cast<unsigned char>(magnitude);
        }
    }
}

int main(int argc, char* argv[]) {

    int numThreads = 4;

    if (argc > 1) {
        numThreads = stoi(argv[1]);
    }

    if (numThreads < 1 || numThreads > 8) {
        cerr << "Jumlah thread harus antara 1 sampai 8." << endl;
        return 1;
    }

    int width, height;

    vector<unsigned char> input;
    vector<unsigned char> output;

    if (!readPGM("input/foto1.pgm", input, width, height)) {
        return 1;
    }

    output.resize(width * height);

    auto start = chrono::high_resolution_clock::now();

    sobelOpenMP(input, output, width, height, numThreads);

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed = end - start;

    string outputFile =
        "output/sobel_openmp_" + to_string(numThreads) + ".pgm";

    if (!savePGM(outputFile, output, width, height)) {
        return 1;
    }

    cout << "Sobel OpenMP berhasil!" << endl;
    cout << "Ukuran gambar : " << width << " x " << height << endl;
    cout << "Jumlah thread : " << numThreads << endl;
    cout << "Waktu proses  : " << elapsed.count() << " detik" << endl;
    cout << "Output        : " << outputFile << endl;

    return 0;
}