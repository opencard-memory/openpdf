# 고급 편집 구현 상태와 제품 기준

## 선택 엔진
PoDoFo 라이브러리를 MPL-2.0 조건으로 동적 연결합니다. PoDoFo는 PDF 파싱/수정, 증분 업데이트, PAdES-B 서명, RSA/ECDSA, CID 인코딩과 글꼴 서브셋을 제공합니다. PoDoFo 명령행 도구는 GPL이므로 설치 파일에 포함하지 않습니다.

## 텍스트 재배치
`TextReflowRequest`는 페이지, 편집 사각형, 원문/대체문, 선호 글꼴을 전달합니다. 구현기는 기존 콘텐츠 연산자를 해석하고 대상 구간을 제거한 뒤, 배경 복원 스트림과 새 텍스트 스트림을 페이지 Contents 배열 뒤에 추가해야 합니다. 첫 릴리스는 사각형 안 자동 줄바꿈과 글꼴 축소만 지원합니다. 복잡한 표, 세로쓰기, 임의 Type3 글꼴은 편집 불가로 표시해야 합니다.

## 이미지 XObject 교체
`ImageXObjectReplacement`는 `/Resources/XObject`의 이름으로 이미지를 지정합니다. 구현기는 기존 `cm ... /ImN Do` 배치 행렬을 보존하고 새 이미지 스트림의 Width, Height, ColorSpace, BitsPerComponent, Filter, SMask를 갱신합니다. 동일 XObject가 여러 위치에서 재사용되면 사용자가 전체 교체 또는 해당 위치만 복제 교체를 선택하도록 해야 합니다.

## 전자서명
`PadesSignatureRequest`는 PFX, visible widget, 사유/위치, 증분 저장 여부를 전달합니다. PoDoFo의 `PdfSigningContext`와 `PdfSignerCms`로 PAdES-B CMS 서명을 생성하고 기존 서명을 보존하려면 증분 저장을 사용합니다. 암호는 로그/명령줄에 남기지 않고 메모리에서 사용 후 지워야 합니다.

## 출시 전 필수 검증
- Adobe Acrobat, Edge, Chrome, Foxit에서 출력 PDF 열기
- veraPDF와 PDF Association 테스트 파일
- 서명 검증, 다중 서명, 서명 후 허용 변경
- 손상/암호화/선형화/object stream PDF
- 한국어 CID 글꼴과 대체 글꼴
- 악성 PDF에 대한 프로세스 격리와 퍼징

현재 저장소에는 데이터 계약, 의존성, 설치 라이선스 화면과 엔진 선택이 포함되어 있습니다. PoDoFo 1.1.x API는 버전 간 변경 가능성이 있으므로 정확한 버전을 고정한 다음 어댑터를 완성하고 테스트해야 합니다. 검증되지 않은 가짜 서명이나 콘텐츠 스트림 변조는 제품 빌드에서 활성화하지 마세요.
