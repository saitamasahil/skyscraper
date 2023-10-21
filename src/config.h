#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

namespace Config {
    enum class FileOp { KEEP, OVERWRITE, CREATE_DIST };

    void copyFile(const QString &src, const QString &dest,
                  FileOp fileOp = FileOp::OVERWRITE);
    void setupUserConfig();
    void checkLegacyFiles();
    QString getSupportedPlatforms();
}

#endif // CONFIG_H