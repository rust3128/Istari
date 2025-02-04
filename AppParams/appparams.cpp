#include "appparams.h"
#include "loggincategories.h"
#include "dbsettingsdialog.h"
#include "criptpass.h"

#include <QFile>
#include <QSettings>

// Втановлення значення констант
const QString AppParams::LOG_FILE_NAME = "Istari.log";             // Лог файл
const QString AppParams::CONFIG_FILE_NAME = "Istari.ini";          // Файл налаштувань
const QString AppParams::KEY_SOLT = "SapForever";
const QString AppParams::VEKTOR_KEY = "Poltava1970Rust";

const QMap<int, QString> AppParams::DATABASE_ITEM_HEADS = {
    {1, "Tables"},
    {2, "Generators"}
};

AppParams &AppParams::instance()
{
    static AppParams inst;
    return inst;
}

// Встановлення параметру
void AppParams::setParameter(const QString &paramName, const QVariant &paramValue)
{
    param[paramName] = paramValue;
}

// Отримання параметру
QVariant AppParams::getParameter(const QString& paramName) const {
    if (param.contains(paramName)) {
        return param.value(paramName);
    } else {
        qWarning(logWarning()) << "Параметр" << paramName << "не знайдено!";
        return QVariant(); // Повертає порожній QVariant, якщо параметр не знайдено
    }
}

AppParams::AppParams()
{
    setDefaultParameters(); // Ініціалізація параметрів за замовчуванням
    if(QFile(CONFIG_FILE_NAME).exists()){
        readDatabaseParametersFromIniFile(); // Зчитування параметрів підключення до бази даних
    } else {
        DbSettingsDialog *dlgSetting = new DbSettingsDialog(true);
        dlgSetting->exec();
    }
}

// Метод для встановлення значень за замовчуванням
void AppParams::setDefaultParameters()
{

}
// Метод для зчитування параметрів підключення до бази даних з INI-файлу
void AppParams::readDatabaseParametersFromIniFile()
{
    QSettings settings(CONFIG_FILE_NAME, QSettings::IniFormat);
    QString dbHostName = settings.value("Database/HostName").toString();
    QString dbDatabasePort = settings.value("Database/Port").toString();
    QString dbDatabaseName = settings.value("Database/DatabaseName").toString();
    QString dbUserName = settings.value("Database/UserName").toString();
    CriptPass crP;
    QString dbPassword = crP.decriptPass(settings.value("Database/Password").toString());

    // Встановити параметри підключення до бази даних
    setParameter("dbHostName", dbHostName);
    setParameter("dbDatabasePort", dbDatabasePort);
    setParameter("dbDatabaseName", dbDatabaseName);
    setParameter("dbUserName", dbUserName);
    setParameter("dbPassword", dbPassword);
}
