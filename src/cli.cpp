/*
 *  This file is part of skyscraper.
 *  Copyright 2017 Lars Muldjord
 *  Copyright 2023 Gemba @ GitHub
 *
 *  skyscraper is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
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

#include "cli.h"

#include "strtools.h"

#include <QCommandLineOption>

void Cli::createParser(QCommandLineParser *parser, QString platforms) {

    parser->setApplicationDescription(
        StrTools::getVersionHeader() +
        "Skyscraper looks for compatible game files for the chosen platform "
        "(set with '-p'). It allows you to gather and cache media and game "
        "information for the files using various scraping modules (set with "
        "'-s'). It then lets you generate game lists for the supported "
        "frontends by combining all previously cached resources ('game list "
        "generation mode' is initiated by simply leaving out the '-s' option). "
        "While doing so it also composites game art for all files by following "
        "the recipe at '/home/USER/.skyscraper/artwork.xml'.\n\nIn addition to "
        "the command line options Skyscraper also provides a lot of "
        "customizable options for configuration, artwork, game name aliases, "
        "resource priorities and much more. Please check the full "
        "documentation at 'github.com/Gemba/skyscraper/tree/master/docs' for a "
        "detailed explanation of all features.\n\nRemember that most of the "
        "following options can also be set in the "
        "'/home/USER/.skyscraper/config.ini' file. All cli options and "
        "config.ini options are thoroughly documented at the above link.");
    parser->addHelpOption();
    parser->addVersionOption();
    QCommandLineOption pOption(
        "p",
        "The platform you wish to scrape.\n(Currently supports " + platforms +
            ".)",
        "PLATFORM", "");
    QCommandLineOption fOption(
        "f",
        "The frontend you wish to generate a gamelist for. Remember to leave "
        "out the '-s' option when using this in order to enable Skyscraper's "
        "gamelist generation mode.\n(Currently supports 'emulationstation', "
        "'retrobat', 'attractmode' and 'pegasus'. Default is "
        "'emulationstation')",
        "FRONTEND", "");
    QCommandLineOption eOption(
        "e",
        "Set extra frontend option. This is required by the 'attractmode' "
        "frontend to set the emulator and optionally for the 'pegasus' "
        "frontend to set the launch command.\n(Default is none)",
        "STRING", "");
    QCommandLineOption iOption(
        "i",
        "Folder which contains the game/rom files.\n(default is "
        "'/home/USER/RetroPie/roms/PLATFORM')",
        "PATH", "");
    QCommandLineOption gOption(
        "g", "Game list export folder.\n(default depends on frontend)", "PATH",
        "");
    QCommandLineOption oOption(
        "o", "Game media export folder.\n(default depends on frontend)", "PATH",
        "");
    QCommandLineOption sOption(
        "s",
        "The scraping module you wish to gather resources from for the "
        "platform set with '-p'.\nLeave the '-s' option out to enable "
        "Skyscraper's gamelist generation mode.\n(WEB: 'arcadedb', 'igdb', "
        "'mobygames', 'openretro', 'screenscraper', 'thegamesdb' and "
        "'worldofspectrum', LOCAL: 'esgamelist' and 'import')",
        "MODULE", "");
    QCommandLineOption uOption(
        "u",
        "userKey or UserID and Password for use with the selected scraping "
        "module.\n(Default is none)",
        "KEY/USER:PASSWORD", "");
    QCommandLineOption mOption(
        "m",
        "Minimum match percentage when comparing search result titles to "
        "filename titles.\n(default is 65)",
        "0-100", "");
    QCommandLineOption lOption(
        "l",
        "Maximum game description length. Everything longer than this will be "
        "truncated.\n(default is 2500)",
        "0-10000", "");
    QCommandLineOption tOption(
        "t",
        "Number of scraper threads to use. This might change depending on the "
        "scraping module limits.\n(default is 4)",
        "1-8", "");
    QCommandLineOption cOption(
        "c",
        "Use this config file to set up Skyscraper.\n(default is "
        "'/home/USER/.skyscraper/config.ini')",
        "FILENAME", "");
    QCommandLineOption aOption(
        "a",
        "Specify a non-default artwork.xml file to use when setting up the "
        "artwork compositing when in gamelist generation mode.\n(default is "
        "'/home/USER/.skyscraper/artwork.xml')",
        "FILENAME", "");
    QCommandLineOption dOption("d",
                               "Set custom resource cache folder.\n(default is "
                               "'/home/USER/.skyscraper/cache/PLATFORM')",
                               "FOLDER", "");
    QCommandLineOption addextOption(
        "addext",
        "Add this or these file extension(s) to accepted file extensions "
        "during a scraping run. (example: '*.zst' or '*.zst *.ext')",
        "EXTENSION(S)", "");
    QCommandLineOption flagsOption(
        "flags",
        "Allows setting flags that will impact the run in various ways. See "
        "'--flags help' for a list of all available flags and what they do.",
        "FLAG1,FLAG2,...", "");
    QCommandLineOption cacheOption(
        "cache",
        "This option is the master option for all options related to the "
        "resource cache. It must be followed by 'COMMAND[:OPTIONS]'.\nSee "
        "'--cache help' for a full description of all functions.",
        "COMMAND[:OPTIONS]", "");
    QCommandLineOption refreshOption("refresh", "Same as '--cache refresh'.");
    QCommandLineOption startatOption(
        "startat",
        "Tells Skyscraper which file to start at. Forces '--refresh' and "
        "'--flags nosubdirs' enabled.",
        "FILENAME", "");
    QCommandLineOption endatOption(
        "endat",
        "Tells Skyscraper which file to end at. Forces '--refresh' and "
        "'--flags nosubdirs' enabled.",
        "FILENAME", "");
    QCommandLineOption includefilesOption(
        "includefiles",
        "(DEPRECATED, please use '--includepattern' instead) Tells Skyscraper "
        "to only include the files matching the provided asterisk pattern(s). "
        "Remember to double-quote the pattern to avoid weird behaviour. You "
        "can add several patterns by separating them with ','. In cases where "
        "you need to match for a comma you need to escape it as '\\,'. "
        "(Pattern example: '\"Super*,*Fighter*\"')",
        "PATTERN", "");
    QCommandLineOption includepatternOption(
        "includepattern",
        "Tells Skyscraper to only include the files matching the provided "
        "asterisk pattern(s). Remember to double-quote the pattern to avoid "
        "weird behaviour. You can add several patterns by separating them with "
        "','. In cases where you need to match for a comma you need to escape "
        "it as '\\,'. (Pattern example: '\"Super*,*Fighter*\"')",
        "PATTERN", "");
    QCommandLineOption excludefilesOption(
        "excludefiles",
        "(DEPRECATED, please use '--excludepattern' instead) Tells Skyscraper "
        "to always exclude the files matching the provided asterisk "
        "pattern(s). Remember to double-quote the pattern to avoid weird "
        "behaviour. You can add several patterns by separating them with ','. "
        "In cases where you need to match for a comma you need to escape it as "
        "'\\,'. (Pattern example: '\"*[BIOS]*,*proto*\"')",
        "PATTERN", "");
    QCommandLineOption excludepatternOption(
        "excludepattern",
        "Tells Skyscraper to always exclude the files matching the provided "
        "asterisk pattern(s). Remember to double-quote the pattern to avoid "
        "weird behaviour. You can add several patterns by separating them with "
        "','. In cases where you need to match for a comma you need to escape "
        "it as '\\,'. (Pattern example: '\"*[BIOS]*,*proto*\"')",
        "PATTERN", "");
    QCommandLineOption fromfileOption(
        "fromfile",
        "(DEPRECATED, please use '--includefrom' instead) Tells Skyscraper to "
        "load the list of filenames to work on from a file. This file can be "
        "generated with the '--cache report:missing' option or made manually.",
        "FILENAME", "");
    QCommandLineOption includefromOption(
        "includefrom",
        "Tells Skyscraper to only include the files listed in FILENAME. One "
        "filename per line. This file can be generated with the '--cache "
        "report:missing' option or made manually.",
        "FILENAME", "");
    QCommandLineOption excludefromOption(
        "excludefrom",
        "Tells Skyscraper to exclude all files listed in FILENAME. One "
        "filename per line. This file can be generated with the '--cache "
        "report:missing' option or made manually.",
        "FILENAME", "");
    QCommandLineOption maxfailsOption(
        "maxfails",
        "Sets the allowed number of initial 'Not found' results before "
        "rage-quitting. (Default is 42)",
        "1-200", "");
    QCommandLineOption queryOption(
        "query",
        "Allows you to set a custom search query (eg. 'rick+dangerous' for "
        "name based modules or 'sha1=CHECKSUM', 'md5=CHECKSUM' or "
        "'romnom=FILENAME' for the 'screenscraper' module). Requires the "
        "single rom filename you wish to override for to be passed on command "
        "line as well, otherwise it will be ignored.",
        "QUERY", "");
    QCommandLineOption regionOption(
        "region",
        "Add preferred game region for scraping modules that support "
        "it.\n(Default prioritization is 'eu', 'us', 'wor' and 'jp' + others "
        "in that order)",
        "CODE", "eu");
    QCommandLineOption langOption("lang",
                                  "Set preferred result language for scraping "
                                  "modules that support it.\n(Default 'en')",
                                  "CODE", "en");
    QCommandLineOption verbosityOption(
        "verbosity", "Print more info while scraping\n(Default is 0.)", "0-3",
        "0");

#if QT_VERSION >= 0x050800
    includefilesOption.setFlags(QCommandLineOption::HiddenFromHelp);
    excludefilesOption.setFlags(QCommandLineOption::HiddenFromHelp);
    fromfileOption.setFlags(QCommandLineOption::HiddenFromHelp);
#endif

    parser->addOption(pOption);
    parser->addOption(sOption);
    parser->addOption(uOption);
    parser->addOption(iOption);
    parser->addOption(gOption);
    parser->addOption(oOption);
    parser->addOption(fOption);
    parser->addOption(eOption);
    parser->addOption(tOption);
    parser->addOption(aOption);
    parser->addOption(mOption);
    parser->addOption(lOption);
    parser->addOption(cOption);
    parser->addOption(dOption);
    parser->addOption(flagsOption);
    parser->addOption(verbosityOption);
    parser->addOption(cacheOption);
    parser->addOption(refreshOption);
    parser->addOption(langOption);
    parser->addOption(regionOption);
    parser->addOption(queryOption);
    parser->addOption(startatOption);
    parser->addOption(endatOption);
    parser->addOption(includefilesOption);
    parser->addOption(includepatternOption);
    parser->addOption(excludefilesOption);
    parser->addOption(excludepatternOption);
    parser->addOption(fromfileOption);
    parser->addOption(includefromOption);
    parser->addOption(excludefromOption);
    parser->addOption(maxfailsOption);
    parser->addOption(addextOption);
}
