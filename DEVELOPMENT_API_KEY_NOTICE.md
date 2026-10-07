# Development API key notice

The Google Fonts Developer API key is embedded in `config/google_fonts_api.dev.json` at the user's explicit request. This file is copied by the development installer. It is not secret once distributed in an EXE/installer and can be extracted by any recipient.

Before a public release:
1. Revoke or rotate this key.
2. Restrict the replacement key to the Google Fonts Developer API and appropriate clients/IPs.
3. Remove `google_fonts_api.dev.json` from the installer.
4. Use a build secret, user-supplied key, or controlled proxy service.
5. Verify that logs and crash dumps do not contain query URLs with the key.
