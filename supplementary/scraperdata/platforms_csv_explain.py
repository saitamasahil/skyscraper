#! /usr/bin/env python3

# Translate the IDs from platforms_map.csv to human readably.

from pathlib import Path
import pandas as pd
import json

SKYSCRAPER_HOME = Path(__file__).parent.parent.parent.resolve()
PLATFORMS_CSV = SKYSCRAPER_HOME/"platforms_map.csv"


def print_data(r):
    folder = r[0]
    scrs_id = r[1]
    moby_id = r[2]
    tgdb_id = r[3]

    last = folder == last_folder
    print(f"    {    '└' if last else '├'}── {folder}")

    scrs_name = "N/A"
    if scrs_id in systemes:
        if 'nom_eu' in systemes[scrs_id]:
            scrs_name = systemes[scrs_id]['nom_eu']
        elif 'noms_commun' in systemes[scrs_id]:
            scrs_name = ", ".join([n for k, n in enumerate(
                systemes[scrs_id]['noms_commun'].split(',')) if k < 3])
        else:
            scrs_name = "[!] no match"

    mobs_name = mobs[moby_id] if moby_id in mobs else "N/A"
    tgdb_name = tgdb_plafs[tgdb_id] if tgdb_id in tgdb_plafs else "N/A"

    print(f"    {' ' if last else '│'}   ├── ScrS {scrs_id:>4d}: {scrs_name}")
    print(f"    {' ' if last else '│'}   ├── Moby {moby_id:>4d}: {mobs_name}")
    print(f"    {' ' if last else '│'}   └── TGDB {tgdb_id:>4d}: {tgdb_name}")


with open(SKYSCRAPER_HOME/"screenscraper_dict.json") as fh:
    systemes = {int(k): v for k, v in json.load(fh).items()}

with open(SKYSCRAPER_HOME/"mobygames_dict.json") as fh:
    mobs = {int(k): v for k, v in json.load(fh).items()}

with open(SKYSCRAPER_HOME/"tgdb_platforms_dict.json") as fh:
    tgdb_plafs = {int(k): v for k, v in json.load(fh).items()}

df = pd.read_csv(PLATFORMS_CSV)
df = df[~df.folder.str.contains('#')]
df = df.astype({col: int for col in df.columns[1:]})
last_folder = df['folder'].values[-1]

print(f"[*] {PLATFORMS_CSV.name}:")
[print_data(row) for row in df.values]

total = df.shape[0]
scrs_count = len(df[(df['screenscraper_id'] != -1)])
moby_count = len(df[(df['mobygames_id'] != -1)])
tgdb_count = len(df[(df['tgdb_id'] != -1)])

print(f"[*] Coverage")
print(
    f"    Screenscraper: {scrs_count:3d}/{total} ({100.0 * scrs_count/total:.1f}%)")
print(
    f"    Moby Games   : {moby_count:3d}/{total} ({100.0 * moby_count/total:.1f}%)")
print(
    f"    The Games DB : {tgdb_count:3d}/{total} ({100.0 * tgdb_count/total:.1f}%)")
