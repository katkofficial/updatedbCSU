#pragma once

#include <unordered_set>

constexpr int MAIN_WINDOW_POINTER_INIT = 0xffff;

constexpr long mainWindowWidth = 800;		//������ ����
constexpr long mainWindowHeight = 600;		//������ ����
constexpr long chooseButtonWidth = 250;		//������ ������ ������ �����
constexpr long chooseButtonHeight = 40;		//������ ������ ������ �����
constexpr long sendButtonWidth = 250;		//������ ������ �������� �����
constexpr long sendButtonHeight = 40;		//������ ������ �������� �����
constexpr long currentFileWidth = 500;		//������ ���� � ��������� �����
constexpr long currentFileHeight = 20;		//������ ���� � ��������� �����
constexpr long warningWidth = 500;			//������ ���� � ��������� �����
constexpr long warningHeight = 35;			//������ ���� � ��������� �����
constexpr long listWidth = 300;				//������ ����������� ������
constexpr long listHeight = 200;			//������ ����������� ������
constexpr long standardOffsetY = 10;		//������� ������ �� ������

constexpr short tableColumns = 17;

constexpr char applicationName[] = "UpdatePaymentPageDataBase.exe ";
constexpr char deleteDataBase[] = "DeletePaymentDB.exe";

const std::unordered_set<std::string> toChange = 
{
	"�����������������_�����_��_\"�����\"_�_�.����������",
	"�����������������_�����_��_\"�����\"_�_�.�������������",
	"�����������������_�����_��_\"�����\"_�_�.�������_������",
	"�����������������_�����_��_\"�����\"_�_�._������_�����������_�������",
	"�����������������_�����_��_\"�����\"_�_�.����������",
	"������������-���������������_�����_������������_����������������_������������_��_����_��_?����������",
	"�������_����������_���������_�_���������������-���������������_��������",
	"������������-���������������_�����_�����_��_�����ӻ_��_����_�������-_������������_������_�._����",
	"������������-���������������_�����_�����_��_����_�����_��������������_�_����������������_�����",
	"�����������������_�����_��_\"�����\"_�_�.�����",
	"�����������������_�����_��_\"�����\"_�_�.��������",
	"�����������������_�����_��_\"�����\"_�_�.���������"
};

const std::string changeTo = "��������_���������_��������,_�������_�_�����������������(���)";