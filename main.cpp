#include "mainwindow.h"
#include "AppParams/appparams.h"
#include "AppParams/loggincategories.h"


#include <QApplication>
#include <QFile>
#include <QDateTime>
#include <QTranslator>

static QScopedPointer<QFile> m_logFile;  //Покажчик на файл логування
// Оголошення оброблювача логування
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

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


    MainWindow w;
    qInfo(logInfo()) << "Запуск головного вікна програми.";
    w.show();
    return a.exec();
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
