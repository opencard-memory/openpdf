# pdfengine.dll SDK adapter

이 폴더는 상용 PDF SDK 어댑터 구현 위치입니다. `IPdfEditorEngine`을 구현하고 다음 C ABI를 export 하세요.

```cpp
extern "C" __declspec(dllexport) IPdfEditorEngine* CreatePdfEditorEngine();
extern "C" __declspec(dllexport) void DestroyPdfEditorEngine(IPdfEditorEngine* p);
```

필수 기능 계약:
1. 기존 텍스트 교체
2. 이미지 XObject 교체
3. 주석 생성 및 appearance stream 갱신
4. 페이지 삭제/재정렬
5. OCR 후 검색 가능한 text layer 저장
6. AES 기반 표준 보안과 권한
7. PFX 인증서 기반 CMS 전자서명 및 ByteRange 처리
8. AcroForm 필드와 appearance 생성
9. incremental save

SDK 바이너리와 라이선스 키는 저장소에 포함하지 마세요. 공급사 계약에 따라 CI secret과 설치 단계에서 주입하세요.
