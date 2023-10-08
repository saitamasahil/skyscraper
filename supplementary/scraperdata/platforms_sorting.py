#! /usr/bin/env python3

from operator import itemgetter
from pathlib import Path
import json

SKYSCRAPER_HOME = Path(__file__).parent.resolve() / "../.."

INFILE = SKYSCRAPER_HOME / "platforms.json"
OUTFILE = SKYSCRAPER_HOME / "platforms_formatted+sorted.json"

with open(INFILE) as f:
    jd = json.load(f)

for entries in jd["platforms"]:
    for k, v in entries.items():
        if type(v) == list:
            entries[k] = sorted(v)

jd["platforms"] = sorted(jd["platforms"], key=itemgetter("name"))

with open(OUTFILE, "w") as f:
    json.dump(jd, f, ensure_ascii=False, indent=4)
