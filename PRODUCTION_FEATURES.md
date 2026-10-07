# 상용 기능 통합 상태

## 포함됨
- 9개 기능을 위한 안정적인 C++ 추상 API
- 런타임 `pdfengine.dll` 로딩
- 공급사 SDK 교체 가능한 ABI
- SDK 미연결 시 안전한 오류 반환
- 기존 Qt 뷰어와 독립된 편집/저장 계층

## 공급사 SDK 어댑터에서 구현해야 함
- 텍스트: 글꼴 임베딩, 문자열 폭, content stream 재작성, fallback font
- 이미지: XObject 교체, 색공간/알파/마스크 보존
- 주석: annotation dictionary와 appearance stream
- 페이지: page tree 재작성
- OCR: 언어 리소스, deskew, searchable text layer
- 보안: AES, 사용자/소유자 암호, 권한 비트
- 서명: Windows 인증서/PFX, CMS, timestamp, LTV, 검증
- 양식: AcroForm, widget annotation, appearance
- 저장: 증분 저장 및 서명 ByteRange 보존

## 왜 엔진 바이너리가 빠져 있는가
이 기능들은 Qt PDF 뷰어가 제공하지 않으며, 신뢰할 수 있는 상용 PDF SDK의 라이선스와 바이너리가 필요합니다. SDK를 무단 재배포하거나 가짜 구현으로 서명·암호화를 제공하면 제품과 문서 무결성에 문제가 생깁니다.
