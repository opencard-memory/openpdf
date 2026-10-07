# OpenPDF Office C++ Windows MVP

Access 스타일 리본 UI를 적용한 C++20 / Qt 6 기반 Windows PDF 데스크톱 MVP입니다.

## 현재 실제 동작 기능
- PDF 열기 및 명령줄 파일 열기
- 다중 페이지 보기
- 페이지 썸네일과 페이지 이동
- 확대, 축소, 너비 맞춤
- Windows 인쇄 대화상자 및 인쇄
- 원본 PDF 복사본 저장
- 한국어 리본형 UI

## 중요: 아직 없는 기능
이 버전은 **실제 원문 개체 편집기가 아니라 배포 가능한 PDF 뷰어 MVP**입니다. PDF 내부 텍스트/이미지 수정, OCR, 주석을 PDF에 삽입, 양식 제작, 암호화 및 전자서명은 상용 PDF SDK 또는 별도의 PDF 쓰기 엔진이 필요합니다.

## 준비물
1. Windows 10/11 x64
2. Visual Studio 2022, Desktop development with C++
3. CMake 3.24 이상
4. Qt 6.6 이상 MSVC 2022 64-bit, 다음 구성요소 포함
   - Qt Widgets
   - Qt PDF
   - Qt PDF Widgets
   - Qt PrintSupport

## 빌드
Qt의 MSVC 명령 프롬프트에서 프로젝트 폴더를 열고:

```bat
set Qt6_DIR=C:\Qt\6.8.3\msvc2022_64\lib\cmake\Qt6
set PATH=C:\Qt\6.8.3\msvc2022_64\bin;%PATH%
build_release.bat
```

완료되면 `dist\OpenPDFOffice.exe`가 만들어집니다. `windeployqt`가 Qt DLL과 PDF 플러그인을 `dist`에 복사합니다. 배포할 때는 exe 하나가 아니라 **dist 폴더 전체**를 전달해야 합니다.

## 직접 CMake 빌드
```bat
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DQt6_DIR=C:\Qt\6.8.3\msvc2022_64\lib\cmake\Qt6
cmake --build build --config Release
windeployqt --release --dir dist build\Release\OpenPDFOffice.exe
copy build\Release\OpenPDFOffice.exe dist\
```

## 제품화 전에 반드시 해야 할 일
- 프로그램명과 로고 상표 조사
- Qt 및 포함된 PDF 엔진의 배포 라이선스 검토
- 코드 서명 인증서 적용
- 설치 프로그램/MSIX 제작
- 자동 업데이트와 충돌 보고
- 악성·손상 PDF에 대한 샌드박싱 및 퍼징
- 암호화 PDF, 대용량 PDF, 인쇄 품질 QA
- 접근성, 다국어, 개인정보 처리방침

## 다음 개발 단계
`IPdfEditorEngine` 추상 계층을 만들고 상용 PDF SDK를 연결하면 텍스트/이미지 개체 편집, 주석 저장, 서명, OCR을 제품 기능으로 발전시킬 수 있습니다.


## Pro SDK 확장
`src/Editor`에 9개 고급 기능의 인터페이스와 `pdfengine.dll` 플러그인 로더가 추가되었습니다. 실제 기능 활성화에는 Apryse 또는 Foxit 같은 C++ PDF SDK의 정식 라이선스와 어댑터 구현이 필요합니다. 자세한 계약은 `sdk/README.md`를 보세요.


## Open-source engine build
`OpenSourceEngine`이 추가되었습니다. 현재 QPDF를 통해 페이지 삭제 효과/재정렬과 AES-256 암호 저장이 실제 동작합니다. 나머지는 `OPEN_SOURCE_STACK.md`의 단계대로 PDFium, Tesseract, OpenSSL writer를 연결합니다. 메뉴가 성공한 것처럼 가장하지 않고 미완성 기능은 명확한 오류를 반환합니다.


## 설치 프로그램과 사용한 오픈소스 화면
`installer/OpenPDFOffice.iss`를 Inno Setup 6으로 컴파일하면 설치 과정에서 **사용한 오픈소스** 제목의 라이선스 화면이 표시됩니다. 설치 후 시작 메뉴에도 같은 문서의 바로 가기가 생성됩니다.
