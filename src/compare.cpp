#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

bool readPGM(const string& filename,
             vector<unsigned char>& image,
             int& width, int& height) {

    ifstream file(filename, ios::binary);

    if (!file.is_open()) {
        cerr << "Gagal membuka: " << filename << endl;
        return false;
    }

    string format;
    int maxValue;

    file >> format >> width >> height >> maxValue;
    file.get();

    if (format != "P5") {
        cerr << "Format bukan PGM P5." << endl;
        return false;
    }

    image.resize(width * height);

    file.read(reinterpret_cast<char*>(image.data()),
              image.size());

    return file.good() || file.eof();
}

int main(int argc, char* argv[]) {

    if (argc != 3) {
        cout << "Cara penggunaan:" << endl;
        cout << "compare.exe gambar1.pgm gambar2.pgm" << endl;
        return 1;
    }

    vector<unsigned char> image1, image2;
    int width1, height1, width2, height2;

    if (!readPGM(argv[1], image1, width1, height1))
        return 1;

    if (!readPGM(argv[2], image2, width2, height2))
        return 1;

    if (width1 != width2 || height1 != height2) {
        cout << "HASIL BERBEDA: ukuran gambar tidak sama." << endl;
        return 1;
    }

    if (image1 == image2) {
        cout << "HASIL SAMA: seluruh pixel identik." << endl;
    } else {
        int differentPixels = 0;

        for (size_t i = 0; i < image1.size(); i++) {
            if (image1[i] != image2[i]) {
                differentPixels++;
            }
        }

        cout << "HASIL BERBEDA!" << endl;
        cout << "Jumlah pixel berbeda: "
             << differentPixels << endl;
    }

    return 0;
}