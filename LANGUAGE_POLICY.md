# Language and font policy

## UI languages
- English
- Korean
- Spanish

The application detects the Windows system locale. Korean selects Korean, any Spanish regional locale selects Spanish, and every other locale falls back to English. Users can override the language in Settings.

## Font coverage
Font availability is independent from UI language. The installer selects the all-language offline font pack by default. User-added fonts and Windows system fonts remain higher priority. Thus the English UI can edit Korean, Japanese, Chinese, Cyrillic, Greek, Hebrew, Devanagari, Thai, Ethiopic and all other bundled scripts.

## Translation catalogs
`i18n/en.json`, `i18n/ko.json`, and `i18n/es.json` contain matching keys for the complete application vocabulary currently defined by the project.
