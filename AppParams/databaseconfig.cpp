#include "DatabaseConfig.h"

DatabaseConfig::DatabaseConfig(const QString& hostName, int port, const QString& databaseName, const QString& userName, const QString& password)
    : hostName(hostName), port(port), databaseName(databaseName), userName(userName), password(password) {}

QString DatabaseConfig::getHostName() const {
    return hostName;
}

int DatabaseConfig::getPort() const {
    return port;
}

QString DatabaseConfig::getDatabaseName() const {
    return databaseName;
}

QString DatabaseConfig::getUserName() const {
    return userName;
}

QString DatabaseConfig::getPassword() const {
    return password;
}
