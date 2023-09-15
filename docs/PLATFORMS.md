### What's supported and why

As Skyscraper was built to be used with RetroPie, the list of supported
platforms is largely dictated by their naming scheme and [list of
platforms](https://retropie.org.uk/docs/Supported-Systems). Get a list of
supported platforms with `Skyscraper --help` 

Thanks to the PR from torresflo @ GitHub it is possible to add new platforms by
editing the `platforms.json` file.

Take this example:
```json
        {
            "name": "atari2600",
            "scrapers": [
                "screenscraper"
            ],
            "formats": [
                "*.a26",
                "*.bin",
                "*.gz",
                "*.rom"
            ],
            "aliases": [
                "atari 2600"
            ]
        },
```

- `name`: reflects the platform/folder name, usually provided with `-p` on the
  command line.
- `scrapers`: set of possible scraper sites. Denotes the default scrapers if not
  overridden by `-s` command line flag.
- `formats`: set of ROM file extensions which will be included in scraping if
  not a ROM file is provided via command line.
- `aliases`: set of aliases for this platform. Make sure that the platform names
  from `screenscraper.json` and/or `mobygames.json` are listed here, thus
  Skyscraper will map it to the right platform ID for the scraper.

**Note**: If you need a specific folder name for a platform (on your setup or
due to an EmulationStation theme) use a symbolic link (see `megadrive` and
`genesis` for example on RetroPie) instead of adding a new platform in this JSON
file.