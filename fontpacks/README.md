# Offline font packs

Do not commit a developer API key or download fonts during every installation. At release-build time, use a restricted Google Fonts Developer API key to refresh metadata, download the selected TTF/OTF files, and copy each font's license beside it. The final installer bundles English/Latin plus the installer's selected locale by default. Optional CJK and all-language packs can be selected.

Expected folders: `latin-core`, `greek`, `cyrillic`, `ethiopic`, `hebrew`, `devanagari`, `thai`, `hangul`, `japanese`, `cjk-sc`, `cjk-tc`, `all`.
