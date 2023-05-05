#pragma once
#include <string>
struct CallRecord {
    int subscriberNumber; // Номер абонента
    std::string subscriberName; // ФИО абонента
    bool isOutgoingCall; // Флаг, указывающий, исходящий ли вызов
    int callNumber; // Номер вызова
    std::string callDate; // Дата звонка
    std::string callTime; // Время звонка
    int callDuration; // Продолжительность разговора (в секундах)
    double tariffPerMinute; // Тариф на одну минуту разговора
};
struct User {
    std::string login; // Логин пользователя
    std::string saltedHashPassword; // Захешированный пароль с солью
    std::string salt; // Соль для хеширования пароля
    std::string role; // Роль пользователя 
    int subscriberNumber; // Номер абонента
    std::string subscriberName; // ФИО абонента
};
