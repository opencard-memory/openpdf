# Offline fonts and localization

## Resolution order
1. User-added TTF/OTF/TTC files
2. Fonts installed on Windows
3. Bundled Google Fonts packs
4. Generic script fallback from `font_substitutions.json`

The resolver never replaces an embedded PDF font while viewing. Substitution is used when the font is missing or when edited text requires a writable/embeddable font. The save dialog must show the chosen replacement and any expected layout change.

## Offline behavior
The shipped installer must contain the selected packs. The Google Fonts Developer API is only used by the release packaging tool or optional Font Manager when online. Cached metadata and already downloaded font files are used offline.

## API key security
The key supplied in chat is intentionally not written into source, JSON, installer scripts, logs, or binaries. Rotate that exposed key and create a new key restricted to the Google Fonts Developer API. Pass it to the packaging step with `OPENPDF_GOOGLE_FONTS_API_KEY` or a local uncommitted settings file.

## Localization
`i18n/locales.json` contains all requested locales. Each locale JSON contains core UI labels, native name and RTL metadata. Files marked `english-fallback-needs-review` require professional translation before a production release. Do not advertise those fallback files as completed translations.
