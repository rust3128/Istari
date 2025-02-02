#ifndef DATABASECONFIG_H
#define DATABASECONFIG_H

#include <QString>

class DatabaseConfig {
public:
    DatabaseConfig(const QString& hostName, int port, const QString& databaseName, const QString& userName, const QString& password);

    QString getHostName() const;
    int getPort() const;
    QString getDatabaseName() const;
    QString getUserName() const;
    QString getPassword() const;

private:
    QString hostName;
    int port;
    QString databaseName;
    QString userName;
    QString password;
};

#endif // DATABASECONFIG_H
