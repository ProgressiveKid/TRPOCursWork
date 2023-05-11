#pragma once
#include <string>
struct CallRecord {
    int key; // id ������
    int subscriberNumber; // ����� ��������
    std::string subscriberName; // ��� ��������
    bool isOutgoingCall; // ����, �����������, ��������� �� �����
    int callNumber; // ����� ������
    std::string callDate; // ���� ������
    std::string callTime; // ����� ������
    double callDuration; // ����������������� ��������� (� ��������)
    double tariffPerMinute; // ����� �� ���� ������ ���������
};
struct User {
    int key;
    std::string login; // ����� ������������
    std::string saltedHashPassword; // �������������� ������ � �����
    std::string salt; // ���� ��� ����������� ������
    std::string role; // ���� ������������ 
    int subscriberNumber; // ����� ��������
    std::string subscriberName; // ��� ��������
};
