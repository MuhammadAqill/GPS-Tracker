#include <stdio.h>
#include <math.h>

#define R 6371000.0 // jejari bumi dalam meter

// Fungsi untuk tukar darjah ke radian
double toRadians(double degree) {
    return degree * M_PI / 180.0;
}

// Fungsi untuk kira jarak guna rumus Haversine
double haversine(double lat1, double lon1, double lat2, double lon2) {
    double dLat = toRadians(lat2 - lat1);
    double dLon = toRadians(lon2 - lon1);

    lat1 = toRadians(lat1);
    lat2 = toRadians(lat2);

    double a = pow(sin(dLat / 2), 2) +
               cos(lat1) * cos(lat2) * pow(sin(dLon / 2), 2);

    double c = 2 * asin(sqrt(a));
    double distance = R * c;

    return distance; // meter
}

int main() {
    double lat1, lon1, lat2, lon2;

    // Contoh koordinat
    lat1 = 2.904685;
    lon1 = 101.864059;
    lat2 = 2.904715;
    lon2 = 101.864105;

    double jarak = haversine(lat1, lon1, lat2, lon2);

    printf("Jarak antara dua titik:\n");
    printf("Dari (%.6f, %.6f)\n", lat1, lon1);
    printf("Ke   (%.6f, %.6f)\n", lat2, lon2);
    printf("Jarak = %.3f meter\n", jarak);

    // Jika jarak lebih 4 meter
    if (jarak > 4.0) {
        printf("⚠️ Terdapat pergerakan lebih dari 4 meter!\n");
    } else {
        printf("✅ Tiada pergerakan ketara.\n");
    }

    return 0;
}
