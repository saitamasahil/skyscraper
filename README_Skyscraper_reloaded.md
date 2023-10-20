# Welcome to Skyscraper: The enhanced and reloaded edition

Based on commit 654a31b from 2022-10-26 from [this enhanced skyscraper
fork](https://github.com/detain/skyscraper) as development and maintenance
obviously has been stalled there. This branch focuses RetroPie integration but
Skyscraper can also be used without RetroPie.

## Included PRs

These pull requests have been merged into this fork.

- https://github.com/detain/skyscraper/pull/14 (extensions only)
- https://github.com/detain/skyscraper/pull/16
- https://github.com/detain/skyscraper/pull/18
- https://github.com/detain/skyscraper/pull/21
- https://github.com/detain/skyscraper/pull/22
- https://github.com/detain/skyscraper/pull/23
- https://github.com/detain/skyscraper/pull/24
- https://github.com/muldjord/skyscraper/pull/362

## Changes

### 2023-10-20 (Version 3.9.0)

- Feature: Mobygames scraper respects game id from mobygames.com website via
  `--query=<gameid>`. Handy to hint to the right game information when usual
  search returns false positives.
- Feature: Scrapers which provide an web API (Screenscraper, Mobygames, The
  Games DB (tgdb)) have the full platform information shipped with this release
  (see `<scraper>_platforms.json` files). These files are used as reference.
- Feature: Less 'aliases' maintenance needed in former `platforms.json`.
- Update: Streamlined external platform configuration. File `platforms.json` is
  replaced by `peas.json` (platforms, extensions/formats, aliases and scrapers).
  Precise platform determination for Screenscraper, Mobygames and TGDB via
  `platform_idmap.csv` See [platforms documentation](docs/PLATFORMS.md) for
  details.
- Fix: Failed media download when TGDB provides PNG files instead of JPG files
  and vice versa.
- Update: Scriptmodule for this Skyscraper branch now official part of RetroPie.

### 2023-09-23 (Version 3.8.1.2309)

- Feature: OpenRetro scraper retrieves now also score/rating, if available for a
  game. Precedence is to use reviews from external websites first (right
  header), then the Score label (above the game details). See this
  [example](https://openretro.org/amiga/shadow-of-the-beast)
- Feature: Additionally to the existing import with rating values of 0, 0.5, 1,
  1.5 ... 5 ("Star rating") it is possible to use 0.1 ... 1.0 scale for rating
  in import files (EmulationStation `gamelist.xml` internal rating range). See
  also [import formats](docs/IMPORT.md#resource-formats)
- Fix: Wrong score/rating calculation from Mobygames scraper / Mobygames API.
  See also [this
  gist](https://gist.github.com/Gemba/13f0accddcecd68a356721ebac020d76) on how
  to update your existing Skyscraper `db.xml` files.
- Fix: Use of `--query` free-text search in OpenRetro scraper fixed. This bug
  did not occur when the switch is ommitted and a Amiga WHDLoad file is provided
  to Skyscraper.
- Fix: RetroPie Scriptmodule, removed surplus boolean negation (Thanks
  @s1eve-mcdichae1)
- Fix: RetroPie Scriptmodule, fixed use of legacy option `--unattend` (Thanks
  @windg)
- Update: RetroPie Scriptmodule, relaxed the remove function of the scriptmodule
  to not zap the Skyscraper cache. Plus various cleanups.

### 2023-09-15 (Version 3.8.0.2309)

- updated `mamemap.csv` from MAME 0.240 (Arcade).dat, fbneo.dat,
  mame2003-plus.xml and cleanup of surplus device information
- update script for make `mamemap.csv` does no longer rely on mame binary
- removed discontinued `*.php` scripts
- `platforms.json` sorted and formatted
- updated documentation especially to reflect the supported platforms
- scriptmodule file for RetroPie aligned to their naming convention
- Mobygames platform information refactored from hardwired `mobygames.cpp`.to
  `mobygames.json`
