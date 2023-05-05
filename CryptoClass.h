#include <string>
#include <fstream>
#include <windows.h>
#include <wincrypt.h>
#include "Structures.h"
#include <vector>
#include <iomanip>
#include <string_view>
#include <type_traits>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <array>
#include <random>
#include <functional>
#pragma once
using namespace std;
class CryptoClass {
public:
    static std::string to_hex_string(const BYTE* data, const DWORD size) {
        std::string hex_string;
        for (DWORD i = 0; i < size; i++) {
            hex_string += char((data[i] >> 4) + 'a');
            hex_string += char((data[i] & 0x0F) + 'a');
        }
        return hex_string;
    }
    // Функция хеширования пароля с солью
    static std::string hashPasswordWithSaltMethod(const std::string& password, const std::string& salt) {
        // Создаем контекст хеширования
        HCRYPTPROV hProv;
        if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
            throw std::runtime_error("Unable to acquire crypto context");
        }
        HCRYPTHASH hHash;
        if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
            throw std::runtime_error("Unable to create hash object");
        }
        // Хеширование с использованием соли
        if (!CryptHashData(hHash, reinterpret_cast<const BYTE*>(password.c_str()), password.length(), 0)) {
            CryptDestroyHash(hHash);
            CryptReleaseContext(hProv, 0);
            throw std::runtime_error("Unable to hash data");
        }
        if (!CryptHashData(hHash, reinterpret_cast<const BYTE*>(salt.c_str()), salt.length(), 0)) {
            CryptDestroyHash(hHash);
            CryptReleaseContext(hProv, 0);
            throw std::runtime_error("Unable to hash data");
        }
        DWORD hashSize = 0;
        DWORD hashSizeLength = sizeof(DWORD);
        if (!CryptGetHashParam(hHash, HP_HASHSIZE, reinterpret_cast<BYTE*>(&hashSize), &hashSizeLength, 0)) {
            CryptDestroyHash(hHash);
            CryptReleaseContext(hProv, 0);
            throw std::runtime_error("Unable to get hash size");
        }
        std::string hashedPassword(hashSize, 0);
        if (!CryptGetHashParam(hHash, HP_HASHVAL, reinterpret_cast<BYTE*>(&hashedPassword[0]), &hashSize, 0)) {
            CryptDestroyHash(hHash);
            CryptReleaseContext(hProv, 0);
            throw std::runtime_error("Unable to get hash value");
        }
        std::string aaa;
        // Очищаем ресурсы
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
       //auto result {find(hashedPassword.begin(), hashedPassword.end(), "\")};
        return CryptoClass::to_hex_string((const BYTE*)hashedPassword.data(), hashSize);
        //return hashedPassword;
    }
   static string generate_salt(int length) {
       string salt_characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
       random_device rd;
       mt19937 generator(rd());
       uniform_int_distribution<int> distribution(0, salt_characters.size() - 1);
       auto rand_char = bind(distribution, generator);
       string saltA = "";
       for (int i = 0; i < length; ++i) {
           saltA += salt_characters[rand_char()];
       }
       return saltA;
   }
};