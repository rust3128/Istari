#ifndef APPPARAMS_H
#define APPPARAMS_H

#include <QString>
#include <QMap>
#include <QVariant>

class AppParams
{
public:
    static AppParams& instance();
    // Встановлення параметру
    void setParameter(const QString& paramName, const QVariant& paramValue);
    // Отримання параметру
    QVariant getParameter(const QString& paramName) const;

    // Константи
    static const QString LOG_FILE_NAME;                         // Файл для логування
    static const QString CONFIG_FILE_NAME;                      // Файл конфігурації
    static const QString KEY_SOLT;
    static const QString VEKTOR_KEY;
    static const QMap<int, QString> DATABASE_ITEM_HEADS;             //Группи об'єктів бази даних яку будемо відображати
private:
    QMap<QString, QVariant> param;                              // Параметри додатка
    AppParams();
    void setDefaultParameters();                                // Метод для встановлення значень за замовчуванням
    void readDatabaseParametersFromIniFile();                   // Метод для зчитування параметрів підключення до бази даних з INI-файлу
};

#endif // APPPARAMS_H
