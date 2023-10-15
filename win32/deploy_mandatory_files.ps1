param([string]$src_dir,[string]$skyscraper_home)

# These may be customized by user, thus do not overwrite with the repo files. 
# The repo will then get .dist extension appended.
$user_cfg_files = @(
  'artwork.xml'
  'aliasMap.csv'
  'peas.json'
  'platforms_idmap.csv'
)

$folders = @(
  '.',
  'cache'
  'import'
  'import/covers'
  'import/maquees'
  'import/screenshots'
  'import/textual'
  'import/textures'
  'import/videos'
  'import/wheels'
  'resources'
)

$dmap = @{
  'examples' = '.'
  'cacheexamples' = 'cache'
  'impexamples' = 'import'
  'resexamples' = 'resources'
}

$fmap = @{}

# Prepare map $fmap with files to copy for each folder
$regex = '^unix:(?<key>.+)\.files=(?<deployfiles>.+)$'
Get-Content $src_dir/skyscraper.pro | Select-String $regex | ForEach-Object {
  $g = $_.Matches.Groups |
    ? { $_.Name -eq 'key' -or $_.Name -eq 'deployfiles' } |
    Select-Object -Property Value
  for ($i=0; $i -lt $g.length; $i+=2) {
    $v = $g[$i].Value
    if ($dmap.ContainsKey($v)) {
      $deployfiles = $g[$i+1].Value.split(' ')
      $fmap.Add($dmap.$v, $deployfiles)
    }
  }
}

$folders | ForEach-Object {
  mkdir -Force "$skyscraper_home/$_" | Out-Null
  $subfolder = $_
  if ($fmap.ContainsKey($subfolder)) {
    $fmap.$subfolder | ForEach-Object {
      $cf = $_.split('/')[-1]
      if (($cf -in $user_cfg_files) -and (Test-Path $skyscraper_home/$subfolder/$cf)) {
        $cf_dist = "{0}.dist" -f $cf
        $cf_print = "$skyscraper_home/$subfolder/$cf".replace("/", "\").replace(".\","")
        "[*] {0} exists. Copying new file to $cf_dist" -f $cf_print
        Copy-Item -Path $src_dir/$_ $skyscraper_home/$subfolder/$cf_dist
      } else {
        Copy-Item -Path $src_dir/$_ $skyscraper_home/$subfolder
      }
    }
  }
}
