#pragma once
#include <string>
struct CallRecord {
    int subscriberNumber; // ����� ��������
    std::string subscriberName; // ��� ��������
    bool isOutgoingCall; // ����, �����������, ��������� �� �����
    int callNumber; // ����� ������
    std::string callDate; // ���� ������
    std::string callTime; // ����� ������
    int callDuration; // ����������������� ��������� (� ��������)
    double tariffPerMinute; // ����� �� ���� ������ ���������
};
struct User {
    std::string login; // ����� ������������
    std::string saltedHashPassword; // �������������� ������ � �����
    std::string salt; // ���� ��� ����������� ������
    std::string role; // ���� ������������ 
    int subscriberNumber; // ����� ��������
    std::string subscriberName; // ��� ��������
};
