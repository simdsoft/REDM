#pragma once

namespace DM
{
	#define ZLIB_DECRYPTION                 // 是否支持加密ZIP
	typedef struct ZIP_FIND_DATA
	{
		WCHAR		szFileName[MAX_PATH];
		WCHAR		szComment[64];
		DWORD		nFileSizeCompressed;
		DWORD		nFileSizeUncompressed;
		FILETIME	ftCreationDate;
		DWORD		dwFileAttributes;
		int			nIndex;
	}ZIP_FIND_DATA, *LPZIP_FIND_DATA;


	class CDMZipFile
	{
	public:
		CDMZipFile();
		~CDMZipFile();
		bool Read(void* pBuffer, DWORD dwSize, LPDWORD pdwRead = NULL);
		bool Close();
		bool IsOpen() const ;
		BYTE* GetData() const;
		DWORD GetSize() const;
		DWORD GetPosition() const;
		DWORD Seek(DWORD dwOffset, UINT nFrom);

		bool Attach(LPBYTE pData, DWORD dwSize);
		void Detach();

	public:
		LPBYTE										m_pData;
		DWORD										m_dwPos;
		DWORD										m_dwSize;


//加密部分--------------------------------------------------------------------
#ifdef ZLIB_DECRYPTION
	public:
		bool DecryptFile(LPCSTR lpszPassword, LPBYTE& pData, DWORD& dwSize, DWORD crc32);
		bool InitKeys(LPCSTR lpszPassword);
		bool DecryptHeader(LPBYTE pData, DWORD dwSize, DWORD crc32);
		bool DecryptData(LPBYTE& pData, DWORD& dwSize);
		void UpdateKeys(BYTE c);
		DWORD crc32(DWORD c, BYTE b);
		BYTE DecryptByte() const;

	public:
		const DWORD*								m_pCrcTable;
		DWORD										m_dwKey[3];
#endif
	};

	//----------------------------------------------------------
	class CDMZipHelper
	{
	public:
		CDMZipHelper();
		~CDMZipHelper();

	public:
		bool LoadFromFile(LPCWSTR lpszFilePath);
		bool LoadFromPE(HMODULE hModule, LPCWSTR lpszName, LPCWSTR lpszType=L"ZIP");
		bool SetPassword(LPCSTR lpszPassword);
		bool IsOpen() const;
		void Close();
		int GetEntries() const;

		// ZIP File API
		int GetFileIndex(LPCWSTR lpszFileName);
		bool GetFile(LPCWSTR lpszFileName, CDMZipFile&file);
		bool GetFile(int iIndex, CDMZipFile& file);
		DWORD GetFileSize(LPCWSTR lpszFileName);
		DWORD GetFileSize(int iIndex);

		// FindFile API
		HANDLE FindFirstFile(LPCWSTR lpszFileName, LPZIP_FIND_DATA lpFindFileData) const;
		bool FindNextFile(HANDLE hFindFile, LPZIP_FIND_DATA lpFindFileData) const;
		bool FindClose(HANDLE hFindFile) const;

	protected:
		bool OpenZip();
		void CloseFile();

		DWORD ReadFile(void* pBuffer, DWORD dwBytes);
		DWORD SeekFile(LONG lOffset, UINT nFrom);

	public:
#pragma pack(2)
		enum{DIR_SIGNATURE = 0x06054b50,};

		struct ZipDirHeader
		{
			DWORD   sig;
			WORD    nDisk;
			WORD    nStartDisk;
			WORD    nDirEntries;
			WORD    totalDirEntries;
			DWORD   dirSize;
			DWORD   dirOffset;
			WORD    cmntLen;
		};

		enum
		{
			FILE_SIGNATURE	= 0x02014b50,
			FILE_COMP_STORE	= 0,
			FILE_COMP_DEFLAT= 8,
		};

		struct ZipDirFileHeader
		{
			DWORD	sig;
			WORD	verMade;
			WORD	verNeeded;
			WORD	flag;
			WORD	compression;		//	COMP_xxxx
			WORD	modTime;
			WORD	modDate;
			DWORD	crc32;
			DWORD	cSize;				//	Compressed size
			DWORD	ucSize;				//	Uncompressed size
			WORD	fnameLen;			//	Filename string follows header.
			WORD	xtraLen;			//	Extra field follows filename.
			WORD	cmntLen;			//	Comment field follows extra field.
			WORD	diskStart;
			WORD	intAttr;
			DWORD	extAttr;
			DWORD	hdrOffset;

			LPSTR GetName() const {return (LPSTR)(this + 1);}
			LPSTR GetExtra() const {return GetName() + fnameLen;}
			LPSTR GetComment() const {return GetExtra() + xtraLen;}
		};

		enum
		{
			LOCAL_SIGNATURE = 0x04034b50,
			LOCAL_COMP_STORE  = 0,
			LOCAL_COMP_DEFLAT = 8,
		};

		struct ZipLocalHeader
		{
			DWORD	sig;
			WORD	version;
			WORD	flag;
			WORD	compression;		//	COMP_xxxx
			WORD	modTime;
			WORD	modDate;
			DWORD	crc32;
			DWORD	cSize;
			DWORD	ucSize;
			WORD	fnameLen;			//	Filename string follows header.
			WORD	xtraLen;			//	Extra field follows filename.
		};

#pragma pack()
		struct FindFileHandle
		{
			wchar_t	szSearch[MAX_PATH];
			int		nPos;
		};

		HANDLE			m_hFile;
		CDMZipFile		m_fileRes;

		ZipDirHeader	m_Header;
		ZipDirFileHeader** m_Files;
		BYTE*			m_DirData;
		char			m_szPassword[64];

	};
}//namespace DM