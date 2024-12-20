/***************************************************************************
 *            pegasus.cpp
 *
 *  Wed Jun 18 12:00:00 CEST 2017
 *  Copyright 2017 Lars Muldjord
 *  muldjordlars@gmail.com
 ****************************************************************************/
/*
 *  This file is part of skyscraper.
 *
 *  skyscraper is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  skyscraper is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with skyscraper; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */

#include "pegasus.h"

#include "nametools.h"
#include "strtools.h"

#include <QDate>
#include <QDir>
#include <QMap>
#include <QRegularExpression>
#include <QTextStream>

Pegasus::Pegasus() {}

// Load old game list entries so we can preserve metadata later when
// assembling new game list
bool Pegasus::loadOldGameList(const QString &gameListFileString) {

    QFile gameListFile(gameListFileString);
    if (!gameListFile.exists() || !gameListFile.open(QIODevice::ReadOnly)) {
        return false;
    }

    QTextStream in(&gameListFile);
    QString *multiline = nullptr;
    GameEntry *currEntry = nullptr;

    while (!in.atEnd()) {
        QString line = in.readLine();

        if (line.startsWith('#')) { // comment line
            continue;
        } else if (line.trimmed().isEmpty()) { // empty line
            continue;
        } else if (line.startsWith(' ') || line.startsWith('\t')) { // multiline
            if (multiline) {
                multiline->append("\n" + line.trimmed());
            }
            continue;
        } else if (line.startsWith(':') ||
                   !line.contains(':')) { // unknowed line format
            continue;
        }

        multiline = nullptr;

        QString key = line.section(':', 0, 0).trimmed();
        QString value = line.section(':', 1).trimmed();

        if (key == "game") {
            GameEntry newEntry;
            newEntry.title = value;
            oldEntries.append(newEntry);
            currEntry = &oldEntries.last();
        } else if (!currEntry) { // This is a header line
            if (key == "command") {
                key = "launch"; // "command" is an alias of "launch"
            } else if (key == "cwd") {
                key = "workdir"; // "cwd" is an alias of "workdir"
            }
            multiline = &headerPairs.insert(key, value).value();
        } else if (key == "file" || key == "files") {
            currEntry->path = value;
        } else if (key == "developer") {
            currEntry->developer = value;
        } else if (key == "publisher") {
            currEntry->publisher = value;
        } else if (key == "release") {
            currEntry->releaseDate =
                QDate::fromString(value, "yyyy-MM-dd").toString("yyyyMMdd");
        } else if (key == "genre") {
            currEntry->tags = value;
        } else if (key == "players") {
            currEntry->players = value;
        } else if (key == "rating") {
            currEntry->rating =
                QString::number(value.remove('%').toDouble() / 100.0);
        } else if (key == "assets.boxFront") {
            currEntry->coverFile = makeAbsolute(value, config->inputFolder);
        } else if (key == "assets.screenshot") {
            currEntry->screenshotFile =
                makeAbsolute(value, config->inputFolder);
        } else if (key == "assets.marquee") {
            currEntry->marqueeFile = makeAbsolute(value, config->inputFolder);
        } else if (key == "assets.wheel") {
            currEntry->wheelFile = makeAbsolute(value, config->inputFolder);
        } else if (key == "assets.cartridge") {
            currEntry->textureFile = makeAbsolute(value, config->inputFolder);
        } else if (key == "assets.video") {
            currEntry->videoFile = makeAbsolute(value, config->inputFolder);
            currEntry->videoFormat = value.split('.').last();
        } else if (key == "description") {
            currEntry->description = value;
            multiline = &currEntry->description;
        } else {
            currEntry->pSValuePairs.append({key, value});
            multiline = &currEntry->pSValuePairs.last().second;
        }
    }

    return true;
}

QString Pegasus::makeAbsolute(const QString &filePath,
                              const QString &inputFolder) {
    QString returnPath = filePath;

    if (returnPath.left(1) == ".") {
        returnPath.remove(0, 1);
        returnPath.prepend(inputFolder);
    }
    return returnPath;
}

bool Pegasus::skipExisting(QList<GameEntry> &gameEntries,
                           QSharedPointer<Queue> queue) {
    gameEntries = oldEntries;

    printf("Resolving missing entries...");
    int dots = 0;
    for (int a = 0; a < gameEntries.length(); ++a) {
        dots++;
        if (dots % 100 == 0) {
            printf(".");
            fflush(stdout);
        }
        QFileInfo current(gameEntries.at(a).path);
        for (int b = 0; b < queue->length(); ++b) {
            if (current.isFile()) {
                if (current.fileName() == queue->at(b).fileName()) {
                    queue->removeAt(b);
                    // We assume filename is unique, so break after getting
                    // first hit
                    break;
                }
            } else if (current.isDir()) {
                // Use current.absoluteFilePath here since it is already a
                // path. Otherwise it will use the parent folder
                if (current.absoluteFilePath() == queue->at(b).absolutePath()) {
                    queue->removeAt(b);
                    // We assume filename is unique, so break after getting
                    // first hit
                    break;
                }
            }
        }
    }
    return true;
}

void Pegasus::preserveFromOld(GameEntry &entry) {
    QString fn = getFilename(entry.path);
    for (const auto &oldEntry : oldEntries) {
        if (getFilename(oldEntry.path) == fn) {
            if (entry.developer.isEmpty()) {
                entry.developer = oldEntry.developer;
            }
            if (entry.publisher.isEmpty()) {
                entry.publisher = oldEntry.publisher;
            }
            if (entry.players.isEmpty()) {
                entry.players = oldEntry.players;
            }
            if (entry.description.isEmpty()) {
                entry.description = oldEntry.description;
            }
            if (entry.rating.isEmpty()) {
                entry.rating = oldEntry.rating;
            }
            if (entry.releaseDate.isEmpty()) {
                entry.releaseDate = oldEntry.releaseDate;
            }
            if (entry.tags.isEmpty()) {
                entry.tags = oldEntry.tags;
            }
            entry.pSValuePairs = oldEntry.pSValuePairs;
            break;
        }
    }
}

QString Pegasus::fromPreservedHeader(const QString &key,
                                     const QString &suggested) {
    if (auto it = headerPairs.find(key); it != headerPairs.end()) {
        QString preserved = it.value();
        headerPairs.erase(it);
        return preserved;
    }

    return suggested;
}

void Pegasus::removePreservedHeader(const QString &key) {
    if (auto it = headerPairs.find(key); it != headerPairs.end()) {
        headerPairs.erase(it);
    }
}

QString Pegasus::toPegasusFormat(const QString &key, const QString &value) {
    QString pegasusFormat = value;

    QRegularExpressionMatch match;
    match = QRegularExpression("\\n[\\t ]*\\n").match(pegasusFormat);
    for (const auto &capture : match.capturedTexts()) {
        pegasusFormat.replace(capture,
                              "###NEWLINE###" + tab + ".###NEWLINE###" + tab);
    }
    pegasusFormat.replace("\n", "\n" + tab);
    pegasusFormat.replace("###NEWLINE###", "\n");
    pegasusFormat.prepend(key + ": ");
    pegasusFormat = pegasusFormat.trimmed();

    return pegasusFormat;
}

void Pegasus::assembleList(QString &finalOutput,
                           QList<GameEntry> &gameEntries) {
    if (!gameEntries.isEmpty()) {
        finalOutput.append(
            "collection: " +
            fromPreservedHeader("collection", gameEntries.first().platform) +
            "\n");
        finalOutput.append(
            "shortname: " + fromPreservedHeader("shortname", config->platform) +
            "\n");
        if (config->frontendExtra.isEmpty()) {
            finalOutput.append(
                "launch: " +
                fromPreservedHeader("launch",
                                    "/opt/retropie/supplementary/runcommand/"
                                    "runcommand.sh 0 _SYS_ " +
                                        config->platform + " \"{file.path}\"") +
                "\n");
        } else {
            finalOutput.append("launch: " + config->frontendExtra + "\n");
            removePreservedHeader("launch");
        }

        for (auto it = headerPairs.begin(); it != headerPairs.end(); it++) {
            finalOutput.append(toPegasusFormat(it.key(), it.value()) + "\n");
        }

        finalOutput.append("\n");
    }
    int dots = -1;
    int dotMod = gameEntries.length() * 0.1 + 1;
    for (auto &entry : gameEntries) {
        if (++dots % dotMod == 0) {
            printf(".");
            fflush(stdout);
        }

        preserveFromOld(entry);

        if (config->relativePaths) {
            entry.path.replace(config->inputFolder, ".");
        }

        finalOutput.append(toPegasusFormat("game", entry.title) + "\n");
        finalOutput.append(toPegasusFormat("file", entry.path) + "\n");
        // The replace here IS supposed to be 'inputFolder' and not
        // 'mediaFolder' because we only want the path to be relative if
        // '-o' hasn't been set. So this will only make it relative if the
        // path is equal to inputFolder which is what we want.
        if (!entry.rating.isEmpty()) {
            finalOutput.append(
                toPegasusFormat(
                    "rating",
                    QString::number((int)(entry.rating.toDouble() * 100)) +
                        "%") +
                "\n");
        }
        if (!entry.description.isEmpty()) {
            finalOutput.append(
                toPegasusFormat("description",
                                entry.description.left(config->maxLength)) +
                "\n");
        }
        if (!entry.releaseDate.isEmpty()) {
            finalOutput.append(
                toPegasusFormat("release",
                                QDate::fromString(entry.releaseDate, "yyyyMMdd")
                                    .toString("yyyy-MM-dd")) +
                "\n");
        }
        if (!entry.developer.isEmpty()) {
            finalOutput.append(toPegasusFormat("developer", entry.developer) +
                               "\n");
        }
        if (!entry.publisher.isEmpty()) {
            finalOutput.append(toPegasusFormat("publisher", entry.publisher) +
                               "\n");
        }
        if (!entry.tags.isEmpty()) {
            finalOutput.append(toPegasusFormat("genre", entry.tags) + "\n");
        }
        if (!entry.players.isEmpty()) {
            finalOutput.append(toPegasusFormat("players", entry.players) +
                               "\n");
        }
        if (!entry.screenshotFile.isEmpty()) {
            finalOutput.append(
                toPegasusFormat("assets.screenshot",
                                (config->relativePaths
                                     ? entry.screenshotFile.replace(
                                           config->inputFolder, ".")
                                     : entry.screenshotFile)) +
                "\n");
        }
        if (!entry.coverFile.isEmpty()) {
            finalOutput.append(
                toPegasusFormat(
                    "assets.boxFront",
                    (config->relativePaths
                         ? entry.coverFile.replace(config->inputFolder, ".")
                         : entry.coverFile)) +
                "\n");
        }
        if (!entry.marqueeFile.isEmpty()) {
            finalOutput.append(
                toPegasusFormat(
                    "assets.marquee",
                    (config->relativePaths
                         ? entry.marqueeFile.replace(config->inputFolder, ".")
                         : entry.marqueeFile)) +
                "\n");
        }
        if (!entry.textureFile.isEmpty()) {
            finalOutput.append(
                toPegasusFormat(
                    "assets.cartridge",
                    (config->relativePaths
                         ? entry.textureFile.replace(config->inputFolder, ".")
                         : entry.textureFile)) +
                "\n");
        }
        if (!entry.wheelFile.isEmpty()) {
            finalOutput.append(
                toPegasusFormat(
                    "assets.wheel",
                    (config->relativePaths
                         ? entry.wheelFile.replace(config->inputFolder, ".")
                         : entry.wheelFile)) +
                "\n");
        }
        if (!entry.videoFormat.isEmpty() && config->videos) {
            finalOutput.append(
                toPegasusFormat(
                    "assets.video",
                    (config->relativePaths
                         ? entry.videoFile.replace(config->inputFolder, ".")
                         : entry.videoFile)) +
                "\n");
        }
        if (!entry.pSValuePairs.isEmpty()) {
            for (const auto &pair : entry.pSValuePairs) {
                finalOutput.append(toPegasusFormat(pair.first, pair.second) +
                                   "\n");
            }
        }
        finalOutput.append("\n\n");
    }
}

bool Pegasus::canSkip() { return true; }

QString Pegasus::getGameListFileName() {
    return QString("metadata.pegasus.txt");
}

QString Pegasus::getInputFolder() {
    return QString(QDir::homePath() + "/RetroPie/roms/" + config->platform);
}

QString Pegasus::getGameListFolder() { return config->inputFolder; }

QString Pegasus::getCoversFolder() { return config->mediaFolder + "/covers"; }

QString Pegasus::getScreenshotsFolder() {
    return config->mediaFolder + "/screenshots";
}

QString Pegasus::getWheelsFolder() { return config->mediaFolder + "/wheels"; }

QString Pegasus::getMarqueesFolder() {
    return config->mediaFolder + "/marquees";
}

QString Pegasus::getTexturesFolder() {
    return config->mediaFolder + "/textures";
}

QString Pegasus::getVideosFolder() { return config->mediaFolder + "/videos"; }
