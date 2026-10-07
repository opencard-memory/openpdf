[Tasks]
Name: "fonts\latin"; Description: "English / Latin core fonts"; Flags: checkedonce
Name: "fonts\locale"; Description: "Fonts for the selected installer language"; Flags: checkedonce
Name: "fonts\cjk"; Description: "Korean, Japanese and Chinese font pack"
Name: "fonts\all"; Description: "All offline language font packs"

[Files]
Source: "..\fontpacks\latin-core\*"; DestDir: "{app}\fonts\latin-core"; Flags: recursesubdirs createallsubdirs; Tasks: fonts\latin
Source: "..\fontpacks\cjk\*"; DestDir: "{app}\fonts\cjk"; Flags: recursesubdirs createallsubdirs; Tasks: fonts\cjk
Source: "..\fontpacks\all\*"; DestDir: "{app}\fonts"; Flags: recursesubdirs createallsubdirs; Tasks: fonts\all
