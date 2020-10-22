#pragma once

#include <unordered_set>

constexpr int MAIN_WINDOW_POINTER_INIT = 0xffff;

constexpr long mainWindowWidth = 800;		//Ширина окна
constexpr long mainWindowHeight = 600;		//Высота окна
constexpr long chooseButtonWidth = 250;		//Ширина кнопки выбора файла
constexpr long chooseButtonHeight = 40;		//Высота кнопки выбора файла
constexpr long sendButtonWidth = 250;		//Ширина кнопки отправки файла
constexpr long sendButtonHeight = 40;		//Высота кнопки отправки файла
constexpr long currentFileWidth = 500;		//Ширина поля с названием файла
constexpr long currentFileHeight = 20;		//Высота поля с названием файла
constexpr long warningWidth = 500;			//Ширина поля с названием файла
constexpr long warningHeight = 35;			//Высота поля с названием файла
constexpr long listWidth = 300;				//Ширина выпадающего списка
constexpr long listHeight = 200;			//Высота выпадающего списка
constexpr long standardOffsetY = 10;		//Обычный отступ по высоте

constexpr short tableColumns = 17;

constexpr char applicationName[] = "UpdatePaymentPageDataBase.exe ";
constexpr char deleteDataBase[] = "DeletePaymentDB.exe";

const std::unordered_set<std::string> toChange = 
{
	"Представительство_ФГБОУ_ВО_\"ЧелГУ\"_в_г.Трехгорном",
	"Представительство_ФГБОУ_ВО_\"ЧелГУ\"_в_г.Нязепетровске",
	"Представительство_ФГБОУ_ВО_\"ЧелГУ\"_в_г.Верхнем_Уфалее",
	"Представительство_ФГБОУ_ВО_\"ЧелГУ\"_в_с._Аргаяш_Челябинской_области",
	"Представительство_ФГБОУ_ВО_\"ЧелГУ\"_в_г.Трехгорном",
	"Инновационно-технологический_центр_Челябинского_государственного_университета_на_базе_АО_?Корпорация",
	"Кафедра_управления_качеством_в_производственно-технологических_системах",
	"Инновационно-технологический_центр_ФГБОУ_ВО_«ЧелГУ»_на_базе_Торгово-_промышленной_палаты_г._Сочи",
	"Инновационно-технологический_центр_ЧелГУ_на_базе_Союза_промышленников_и_предпринимателей_Крыма",
	"Представительство_ФГБОУ_ВО_\"ЧелГУ\"_в_г.Орске",
	"Представительство_ФГБОУ_ВО_\"ЧелГУ\"_в_г.Копейске",
	"Представительство_ФГБОУ_ВО_\"ЧелГУ\"_в_г.Шадринске"
};

const std::string changeTo = "Институт_экономики_отраслей,_бизнеса_и_администрирования(ДОТ)";