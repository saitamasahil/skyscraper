#include "nametools.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>

int main() {

    QString ini = "./scummvm.ini";

    QMap<QString, QPair<QString, QString>> tests = {
        {"scummvm tentacle",
         QPair<QString, QString>("tentacle",
                                 "Day of the Tentacle (Floppy/DOS/English)")},
        {"scummvm dontexists",
         QPair<QString, QString>("dontexists", "dontexists")},
        {"scummvm dig",
         QPair<QString, QString>("dig", "The Dig")},
        {"scummvm use game id from ROM file",
         QPair<QString, QString>("lba", "Little Big Adventure")},
        {"scummvm no valid gameid",
         QPair<QString, QString>("Some Mighty Adventure", "Some Mighty Adventure")},
        {"scummvm no gameid",
         QPair<QString, QString>("No game id in romfile", "No game id in romfile")}

    };

    QFile romfile("rom_samples/The Dig (1995).scummvm");

    qInfo() << "";
    int pass = 0;
    for (auto t : tests.keys()) {
        if (t == "scummvm no gameid") {
            QFile romfile("rom_samples/Some Mighty Adventure.svm");
        } else if (t == "scummvm no gameid") {
            QFile romfile("rom_samples/No game id in romfile.svm");   
        }
        QString baseName = tests.value(t).first;
        QString exp = tests.value(t).second;
        QString out = NameTools::getScummName(QFileInfo(romfile), baseName, ini);
        if (out == exp) {
            qInfo() << "  Pass: " << t;
            pass++;
        } else {
            qWarning() << "Failed: " << t << "\nExp: " << exp
                       << "\nGot: " << out << "\n";
        }
    }
    qInfo() << "\nTests passed: " << pass << "/" << tests.keys().size();
}