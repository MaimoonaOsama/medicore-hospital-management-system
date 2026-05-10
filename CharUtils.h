
#ifndef CHARUTILS_H
#define CHARUTILS_H

class CharUtils {
public:
    static int length(const char arr[]) {
        int i = 0;
        while (arr[i] != '\0') i++;
        return i;
    }

    static void copy(char dest[], const char src[], int maxSize) {
        int i = 0;
        while (src[i] != '\0' && i < maxSize - 1) {
            dest[i] = src[i];
            i++;
        }
        dest[i] = '\0';
    }

    static bool equals(const char a[], const char b[]) {
        int i = 0;
        while (a[i] != '\0' && b[i] != '\0') {
            if (a[i] != b[i]) return false;
            i++;
        }
        return a[i] == '\0' && b[i] == '\0';
    }

    
    static int compare(const char a[], const char b[]) {
        int i = 0;
        while (a[i] != '\0' && b[i] != '\0') {
            if (a[i] != b[i]) return (unsigned char)a[i] - (unsigned char)b[i];
            i++;
        }
        return (unsigned char)a[i] - (unsigned char)b[i];
    }

    static char toLowerChar(char c) {
        if (c >= 'A' && c <= 'Z') return c + 32;
        return c;
    }

    static bool equalsIgnoreCase(const char a[], const char b[]) {
        int i = 0;
        while (a[i] != '\0' && b[i] != '\0') {
            if (toLowerChar(a[i]) != toLowerChar(b[i])) return false;
            i++;
        }
        return a[i] == '\0' && b[i] == '\0';
    }

    static int subToInt(const char* s, int offset, int n) {
        int result = 0;
        for (int i = 0; i < n; i++)
            result = result * 10 + (s[offset + i] - '0');
        return result;
    }

    
    static int dateVal(const char* d) {
        int yr = subToInt(d, 6, 4);
        int mo = subToInt(d, 3, 2);
        int day = subToInt(d, 0, 2);
        return yr * 10000 + mo * 100 + day;
    }

   
    static int compareDatesAsc(const char* a, const char* b) {
        return dateVal(a) - dateVal(b);
    }

    static int compareDatesDesc(const char* a, const char* b) {
        return dateVal(b) - dateVal(a);
    }
};

#endif 
