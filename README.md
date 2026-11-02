# Sobel Edge Detection — Parallel Programming

## 1. Deskripsi Proyek

Proyek ini merupakan implementasi algoritma **Sobel Edge Detection** untuk mendeteksi tepi pada citra grayscale berformat PGM (Portable GrayMap).

Program dibuat menggunakan tiga pendekatan:

- **Serial:** pemrosesan gambar secara berurutan.
- **Pthreads:** pemrosesan paralel menggunakan POSIX Threads.
- **OpenMP:** pemrosesan paralel menggunakan compiler directive.

Tujuan proyek ini adalah membandingkan performa, kompleksitas kode, dan karakteristik dari ketiga pendekatan tersebut.

## 2. Persiapan Gambar Input

Program Sobel Edge Detection menggunakan gambar grayscale dengan format **PGM (P5)** sebagai input.

Jika belum memiliki gambar grayscale berformat PGM, lakukan konversi terlebih dahulu menggunakan program Image Converter berikut:

**[Image Converter to Graysacle — GitHub](https://github.com/azhar2301/Image-Converter-to-Grayscale)**

Langkah-langkah:

1. Siapkan gambar dengan format JPG atau JPEG.
2. Jalankan program Image Converter untuk mengubah gambar menjadi format PGM.
3. Salin hasil konversi ke folder `input/` pada proyek Sobel Edge Detection.
4. Pastikan nama file gambar adalah `foto1.pgm`.

Setelah gambar input tersedia, program Sobel Edge Detection dapat dijalankan.

## 3. Struktur Folder

```text
sobel_program/
├── hasil-pengujian-terminal.txt
├── input/
│   └── foto1.pgm
├── output/
├── src/
│   ├── main_serial.cpp
│   ├── main_pthreads.cpp
│   ├── main_openmp.cpp
│   └── compare.cpp
├── Makefile
└── README.md
```

Keterangan:

- `hasil-pengujian-terminal.txt` berisi dokumentasi hasil pengujian program.
- `input/` berisi gambar grayscale berformat PGM yang akan diproses.
- `output/` merupakan lokasi penyimpanan hasil pemrosesan.
- `src/` berisi source code implementasi algoritma Sobel.
- `Makefile` digunakan untuk mengotomatisasi proses kompilasi dan eksekusi program.

## 4. Persyaratan Sistem

Perangkat lunak yang dibutuhkan:

- Windows
- MSYS2 UCRT64
- GCC / G++
- GNU Make
- OpenMP
- Pthreads

## 5. Cara Kompilasi

Buka terminal **MSYS2 UCRT64** pada folder `sobel_program`, kemudian jalankan:

```bash
make
```

Perintah tersebut akan mengompilasi seluruh program berdasarkan konfigurasi pada Makefile.

Untuk menghapus file hasil kompilasi:

```bash
make clean
```

## 6. Cara Menjalankan Program

Pastikan gambar `foto1.pgm` sudah tersedia di folder `input/`.

Terdapat dua cara untuk menjalankan program.

### A. Menjalankan Seluruh Program Sekaligus

Untuk menjalankan implementasi Serial, Pthreads, dan OpenMP secara berurutan, gunakan perintah:

```bash
make run
```

Perintah ini menjalankan seluruh pengujian berdasarkan konfigurasi pada Makefile.

### B. Menjalankan Program Secara Terpisah

Jika ingin menjalankan masing-masing implementasi secara manual, gunakan perintah berikut.

**Serial**

```bash
./serial.exe
```

**Pthreads**

```bash
./pthreads.exe 1
./pthreads.exe 2
./pthreads.exe 4
./pthreads.exe 8
```

**OpenMP**

```bash
./openmp.exe 1
./openmp.exe 2
./openmp.exe 4
./openmp.exe 8
```

### C. Membandingkan Hasil Gambar

Untuk memastikan hasil pemrosesan paralel sama dengan hasil serial, gunakan program pembanding:

```bash
./compare.exe output/sobel_serial.pgm output/sobel_pthreads_4.pgm
```

Sesuaikan nama file output berdasarkan hasil yang dihasilkan program.

## 7. Hasil Program

Hasil pemrosesan gambar akan disimpan secara otomatis di folder `output/`.

Implementasi algoritma Sobel menggunakan metode **zero-padding** untuk menangani pixel yang berada pada batas gambar.

## 8. Pengujian Performa

Pengujian dilakukan dengan membandingkan implementasi Serial, Pthreads, dan OpenMP.

Variasi jumlah thread yang digunakan pada implementasi Pthreads dan OpenMP adalah 1, 2, 4, dan 8 thread.

Parameter yang dianalisis meliputi:

- Waktu eksekusi (*Execution Time*).
- Percepatan pemrosesan (*Speedup*).
- Efisiensi penggunaan thread (*Efficiency*).
- Kompleksitas kode.
- Perbandingan karakteristik Pthreads dan OpenMP.

## 9. Dokumentasi Pengujian

Dokumentasi hasil pengujian tersedia dalam file `hasil-pengujian-terminal.txt` yang berada di direktori utama proyek (root folder).

File tersebut berisi hasil eksekusi program Serial, Pthreads, dan OpenMP, termasuk pengujian kesamaan hasil citra.