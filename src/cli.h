#ifndef CLI_H
#define CLI_H

#include <QCommandLineParser>
#include <QString>

namespace Cli {
    void createParser(QCommandLineParser *parser, const QString platforms);
}

#endif // CLI_H