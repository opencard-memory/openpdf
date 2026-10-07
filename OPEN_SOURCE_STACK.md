# OpenPDF Office 오픈소스 편집 스택

## 채택 구성

- QPDF: PDF 객체/페이지 구조, 변환, 암호화. Apache-2.0.
- PDFium: 렌더링, 텍스트/이미지 페이지 객체 접근. BSD-3-Clause 및 번들 구성요소 고지 필요.
- Tesseract + Leptonica: OCR. Apache-2.0.
- OpenSSL 3.x: CMS 전자서명 컨테이너. Apache-2.0.
- Qt 6: Windows UI. 선택한 Qt 모듈별 LGPL/GPL/상용 조건 확인.

## 현재 코드에서 실제 활성화

- 안전한 작업 복사본 열기
- QPDF 기반 페이지 선택, 삭제 효과, 재정렬
- QPDF 기반 AES-256 사용자/소유자 암호
- 새 파일 저장

## 다음 구현 묶음

1. PDFium 페이지 객체 편집: 텍스트/이미지 선택, 수정 및 새 content stream 생성
2. QPDF annotation/AcroForm writer: 주석과 양식 appearance stream
3. Tesseract OCR: PDFium 300 DPI 렌더링, TSV 좌표, invisible text layer
4. OpenSSL CMS: `/ByteRange`, 고정 크기 `/Contents`, detached CMS, 증분 저장
5. 서명 검증과 기존 서명 보존 테스트

## 제품 제한

PDF의 기존 텍스트를 Word처럼 재배치하는 기능은 단순 검색/교체가 아닙니다. 폰트 CMap, 글리프 폭, CID 폰트, 임베딩 및 content stream 연산자를 함께 처리해야 합니다. 첫 출시에서는 선택 영역 덮어쓰기 방식과 같은 글꼴 내 교체부터 지원하고, 문단 reflow는 이후 버전으로 분리하세요.
