### Supported platforms and how to add new platforms for scraping

Get a list of supported platforms with `Skyscraper --help`

After the initial PR from torresflo @ GitHub it is possible to add new platforms
by editing the `platforms.json` file. Since version 3.9.0 onwards this file is
replaced by `peas.json` (short for platforms, extensions/formats, aliases and
scrapers) in the same folder.

Take this example from the `peas.json`:

```json
    [...]
    "megadrive": {
        "aliases": [
            "genesis",
            "mega drive",
            "sega mega drive",
            "sega mega drive/genesis"
        ],
        "formats": [
            "*.bin",
            "*.gen",
            "*.md",
            "*.sg",
            "*.smd"
        ],
        "scrapers": [
            "openretro",
            "screenscraper"
        ]
    },
    [...]
```

-   `megadrive`: this key reflects the platform/folder name (or more generally the
    handle), usually provided with `-p` on the command line.
-   `aliases`: these aliases can be used to allow additional hits for a search.
    The use of aliases is handy especially for scraping websites which do _not_
    provide a web API. Currently Screenscraper, Mobygames or The Games DB are
    providing an web API. See also section about `platforms_idmap.csv` below.
-   `formats`: set of ROM file extensions which will be included in scraping if
    a ROM file is not provided explicitly via command line.
-   `scrapers`: Denotes the possible scrapers if not overridden by `-s` command
    line flag. Will be deprecated in some later versions as it is not used in the
    codebase.

**Note**: If you need a specific folder name for a platform (on your setup or
due to an EmulationStation theme) use a symbolic link (see `megadrive` (folder)
and `genesis` (symlink) for example on RetroPie or `plus4` (folder) and `c16`
(symlink)) instead of adding a new platform in this JSON file.

#### Exact platform mapping

The file `platforms_idmap.csv` defines exact platform id for the web API of the
three before mentioned scraping sites. It is a CSV file which maps the platform
handle (e.g. `megadrive`) to the respective platform id of the scraping sites:

```
folder,screenscraper_id,mobygames_id,tgdb_id
[...]
megadrive,1,16,36
[...]
```

You can display the number with their platform name on each of the three
scraping sites with the script `peas_and_idmap_verify.py`. Find the script
sibling to the Skyscraper executable. This is a part of the output:

```
[...]
    ├── megadrive
    │   ├── ScrS    1: Megadrive
    │   ├── Moby   16: Genesis
    │   └── TGDB   36: Sega Mega Drive
[...]
```

#### Updating `peas.json` and `platforms_idmap.csv`

These two files are ment to be locally edited and extended for additional
platforms. Whenever you add a new segment to the `peas.json` do also lookup the
corresponding platform ids for the scraping sites with an API, for those without
an API do fill in the platform alias which is used on the website in the
`aliases` list.

To find the platform ids for Screenscraper, Mobygames and The Games DB, please
consult the files `screenscraper_platforms.json`, `mobygames_platforms.json` and
`tgdb_platforms.json` which are located sibling to your `config.ini` of the
Skyscraper installation.

### Migrating `platforms.json` and `screenscraper.json`

**Note**: If you ever neither edited `platforms.json` nor `screenscraper.json`
or do not have these files in the Skyscraper config folder (sibling to the
`config.ini`) you can safely ignore this section.

However, if you have local changes in these files you can transfer your local
changes to the new files. The new `peas.json` is the `platforms.json` but in a
more compact form which has less visual overhead and also allows faster parsing.
The `platforms_idmap.csv` maps the former `screenscraper.json` platform ids plus
the platform ids of two more web API sites.

Use the script `convert_platforms_json.py` (sibling to the Skyscraper
executable) to convert the `platforms.json` to a `peas_mine.json` which you can
then diff to the `peas.json` and transfer your changes to `peas.json`.

Use the script `check_screenscraper_json_to_idmap.py` to identify differences
from your `screenscraper.json`. Then use the three `<scraper>_platforms.json`
files to identify the matching platform ids to be entered in
`platform_idmap.csv`. Use `-1` if there is no matching platform id.

#### Summary of changes in the config files

Filenames shown ~~strikethrough~~ are superseeded.
Filenames shown _italic_ are user editable.

| File                     | Introduced        | Notes (version)                                                                                                                                      |
| ------------------------ | ----------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------- |
| ~~`mobygames.json`~~     | 3.8.0             | superseeded by `mobygames_platforms.json` (v3.9.0); not to be edited; IDs are used in `platforms_idmap.csv`                                          |
| ~~`platforms.json`~~     | 3.7.7-2 (@detain) | superseeded by _`peas.json`_ (3.9.0); do edit to add new platforms; uses leaner format than `platforms.json`                                         |
| _`platforms_idmap.csv`_  | 3.9.0             | maps the platform names (handles) from _`peas.json`_ to exact platform IDs used in Moby, Screenscraper or TGDB; do edit to add new platforms         |
| ~~`screenscraper.json`~~ | 3.7.7-2 (@detain) | IDs formerly used in here are part of `platforms_idmap.csv` (3.9.0); superseeded by `screenscraper_platforms.json` (3.9.0) which is not to be edited |
| `tgdb_developers.json`   | 2.5.3 (@muldjord) | uses leaner format (3.9.0); not to be edited                                                                                                         |
| `tgdb_genres.json`       | 3.9.0             | uses leaner format (3.9.0); not to be edited                                                                                                         |
| `tgdb_platforms.json`    | 3.9.0             | uses leaner format (3.9.0); not to be edited; IDs are used in `platforms_idmap.csv`                                                                  |
| `tgdb_publishers.json`   | 2.5.3 (@muldjord) | uses leaner format (3.9.0); not to be edited                                                                                                         |
