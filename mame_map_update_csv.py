#! /usr/bin/env python3

# Generate a Skyscraper compatible mame ROM filename to ROM full name CSV.

import csv
import pandas as pd
import requests
import xmltodict

URL = (
    "https://raw.githubusercontent.com/"
    "RetroPie/EmulationStation/master/resources/mamenames.xml"
)
OUTFILE = "mameMap.csv"

req = requests.get(URL)
lines = req.text.split("\n")
hdr = lines[0].replace("<!--", "").replace("-->", "").strip()
print(f"[+] File info: {hdr}")
lines[0] = "<root>"
lines.append("</root>")
df = pd.read_xml("".join(lines), xpath="//root/*")

if len(df):
    print(f"[+] Found {len(df)} ROM names")

    with open(OUTFILE, "w") as csvoutfile:
        csvoutfile.write(f"# {hdr}\n")
        csvoutfile.write(f"# yarked from: {URL}\n")

    df.to_csv(
        OUTFILE, index=False, header=False, sep=";", quoting=csv.QUOTE_ALL, mode="a"
    )
    print(f"[+] Written to {OUTFILE}")
else:
    print(f"[!] Ran into an error")
