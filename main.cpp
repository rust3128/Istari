#include "mainwindow.h"
#include "AppParams/appparams.h"
#include "AppParams/loggincategories.h"
#include <windows.h>
#include <lm.h>


#include <QApplication>
#include <QFile>
#include <QDateTime>
#include <QTranslator>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include <QLockFile>

static QScopedPointer<QFile> m_logFile;  //Покажчик на файл логування
// Оголошення оброблювача логування
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
// Відкриття бази даних програми
bool openDatabaseConnection();
// Функція для читання параметрів з бази даних
bool readParametersFromDatabase();
//Отримання даних користувача
void readUserProfile(QString userName);



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
/* Налаштування логуванння */
    // Встановлюемо файл логування
    m_logFile.reset(new QFile(AppParams::LOG_FILE_NAME));
    // Відкриваємо файл логування
    m_logFile.data()->open(QFile::Append | QFile::Text);
    // Встановлюємо обробник
    qInstallMessageHandler(messageHandler);

/* Встановлення україньского інтерфейсу стандартних діалогів */
    QTranslator qtTranslator;
    if (qtTranslator.load(":/Resources/Translations/qtbase_uk.qm")) {
        a.installTranslator(&qtTranslator);
    } else {
        // Обробка помилки завантаження перекладу Qt
        qCritical(logCritical()) << "Помилка завантаження перекладу для Qt.";
        MyMessage::showNotification(QMessageBox::Critical, "Помилка", "Помилка завантаження перекладу стандартних елементів Qt.","","");
    }


    // Відкрити базу даних
    if (openDatabaseConnection()) {
        // З'єднання з базою даних вдало встановлено

        // Прочитати параметри з бази даних
        if(!readParametersFromDatabase()){
            qCritical(logCritical()) << "Неможливо завантажити параметри програми. Завершення роботи.";
            return 1;
        }
    } else {
        qCritical(logCritical()) << "База даних програми не відкрита. Завершення роботи.";
        return 1;
    }

    // Отримання імені користувача
    QString qUsername = QString::fromLocal8Bit(qgetenv("USERNAME").constData()).toUtf8();

///////  Контролюемо запуск единого екземпляру програми //////////////////
    // Вказуємо шлях до директорії додатку
    QString appDirPath = QCoreApplication::applicationDirPath();
    // Вказуємо шлях до теки "Locks"
    QString locksDirPath = appDirPath + "/Locks";

    // Перевіряємо чи існує папка "Locks", якщо ні — створюємо її
    QDir().mkpath(locksDirPath);

    // Вказуємо шлях до файлу блокування, додаючи ім'я користувача
    QString lockFilePath = locksDirPath + "/Gandalf_" + qUsername + ".lock";
    QLockFile lockFile(lockFilePath);

    // Пробуємо заблокувати файл
    if (!lockFile.tryLock()) {
        QMessageBox::warning(nullptr, "Помилка", "Gandalf вже запущено. Пошукайте у панелі задач!.");
        // Якщо файл заблокований, завершуємо роботу додатку
        return 1;
    }
    readUserProfile(qUsername);



    MainWindow w;
    qInfo(logInfo()) << "Запуск головного вікна програми.";
    w.show();
    return a.exec();
}

//Отримання даних користувача
void readUserProfile(QString userName)
{
    QSqlQuery q;
    q.prepare("EXECUTE PROCEDURE get_userid(:userName)");
    q.bindValue(":userName",userName);
    if(!q.exec()){
        qCritical(logCritical()) << "Не вдалося отримати ID користувача." << q.lastError().text();
    }
    q.next();
    int userID = q.value(0).toInt();
    AppParams::instance().setParameter("userID",userID);
    q.exec("commit work");
    q.finish();

    q.prepare("SELECT u.user_login, u.user_fio, u.user_role, u.isactive "
              "FROM users u WHERE user_id = ?");
    q.bindValue(0, userID);
    if(!q.exec()) {
        qCritical(logCritical()) << "Неможливо отримати дані користувача" << q.lastError().text();
    }
    q.next();
    AppParams::instance().setParameter("userLogin",q.value(0));
    AppParams::instance().setParameter("userFIO",q.value(1));
    AppParams::instance().setParameter("userRole",q.value(2));
    AppParams::instance().setParameter("isActive",q.value(3));

}

// Функція для читання параметрів з бази даних
bool readParametersFromDatabase()
{
    bool result = false;
    // Виконати запити SQL для отримання параметрів
    QSqlQuery query;
    if (query.exec("SELECT param_name, param_value FROM PARAMETERS")) {
        // Отримати та встановити параметри в AppParameters
        while (query.next()) {
            QString paramName = query.value(0).toString();
            QVariant paramValue = query.value(1);
            AppParams::instance().setParameter(paramName, paramValue);
        }
        qInfo(logInfo()) << "Параметри програми успішно завантажені.";
        result = true;
    }
    return result;
}

//Выдкриття бащи даних програми
bool openDatabaseConnection()
{
    // Встановити з'єднання з базою даних, використовуючи параметри з AppParameters
    QSqlDatabase db = QSqlDatabase::addDatabase("QIBASE");
    db.setHostName(AppParams::instance().getParameter("dbHostName").toString());
    db.setPort(AppParams::instance().getParameter("dbDatabasePort").toInt());
    db.setDatabaseName(AppParams::instance().getParameter("dbDatabaseName").toString());
    db.setUserName(AppParams::instance().getParameter("dbUserName").toString());
    db.setPassword(AppParams::instance().getParameter("dbPassword").toString());
    if(db.open()){
        return true;
    } else {
        QString errMsg = db.lastError().text();
        qCritical(logCritical()) << "Помилка відкриття бази даних додатка:" << errMsg;
        MyMessage::showNotification(QMessageBox::Critical,"Критична помилка","Не можливи відкрити базу даних додатка!","Подальша робота не можлива.",errMsg);
        return false;
    }
}


// Обробка логування
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Открываем поток записи в файл
    QTextStream out(m_logFile.data());
    QTextStream console(stdout);
    // Записываем дату записи
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    // По типу определяем, к какому уровню относится сообщение
    switch (type)
    {
#ifdef QT_DEBUG
    case QtInfoMsg:
        out << "[INF] ";
        console << "Info:     " << msg << " ***[" << "Function: " << context.function << " File: " << context.file << " Line: " << context.line << "] " << Qt::endl;
        break;
    case QtDebugMsg:
        out << "[DBG] " ;
        console << "Debug:    " << msg << " ***[" << "Function: " << context.function << " File: " << context.file << " Line: " << context.line << "] " << Qt::endl;
        break;
    case QtWarningMsg:
        out << "[WRN] ";
        console << "Warning:  " << msg << " ***[" << "Function: " << context.function << " File: " << context.file << " Line: " << context.line << "] "  << Qt::endl;
        break;
    case QtCriticalMsg:
        out << "[CRT] ";
        console << "Critical: " << msg << " ***[" << "Function: " << context.function << " File: " << context.file << " Line: " << context.line << "] " << Qt::endl;
        break;
    case QtFatalMsg:
        out << "[FTL] ";
        console << "Fatality: " << msg << " ***[" << "Function: " << context.function << " File: " << context.file << " Line: " << context.line << "] " << Qt::endl;
        break;
#else
    case QtInfoMsg:     out << "[INF] "; break;
    case QtDebugMsg:    out << "[DBG] "; break;
    case QtWarningMsg:  out << "[WRN] "; break;
    case QtCriticalMsg: out << "[CRT] "; break;
    case QtFatalMsg:    out << "[FTL] "; break;
#endif

    }

    // Записываем в вывод категорию сообщения и само сообщение
    out << context.category << ": " << msg;
    out << "  ***[" << "Function: " << context.function << " File: " << context.file << " Line: " << context.line << "]***" << Qt::endl;
    // Очищаем буферизированные данные
    out.flush();
    console.flush();
}
