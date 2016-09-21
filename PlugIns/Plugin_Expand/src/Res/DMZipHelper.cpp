#include "Plugin_ExpandAfx.h"
#include "DMZipHelper.h"
#include <zconf.h>
#include <zlib.h>
#ifdef _DEBUG
#pragma comment(lib, "zlib_d.lib")
#else
#pragma comment(lib, "zlib.lib")
#endif // _DEBUG

namespace DM
{
	CDMZipFile::CDMZipFile()
	{
		m_pData    = NULL;
		m_dwSize   = 0;
		m_dwPos    = 0;
#ifdef ZLIB_DECRYPTION
		m_pCrcTable = NULL;
#endif
	}

	CDMZipFile::~CDMZipFile()
	{
		Close();
	}

	bool CDMZipFile::Read(void* pBuffer, DWORD dwSize, LPDWORD pdwRead/* = NULL*/)
	{
		bool bRet = false;
		do 
		{
			if (!IsOpen())
			{
				break;
			}
			if (NULL != pdwRead)
			{
				*pdwRead = 0;
			}
			if (NULL == m_pData)
			{
				break;
			}
			if (0 == dwSize || m_dwPos >= m_dwSize)
			{
				break;
			}
			if (m_dwPos + dwSize > m_dwSize)
			{
				dwSize = m_dwSize - m_dwPos;
			}
			memcpy(pBuffer, m_pData+m_dwPos, dwSize);
			m_dwPos += dwSize;
			if (NULL != pdwRead)
			{
				*pdwRead = dwSize;
			}

			bRet = true;
		} while (false);
		return bRet;
	}

	bool CDMZipFile::Close()
	{
		do 
		{
			if (NULL != m_pData)
			{
				delete[] m_pData;
				m_pData	= NULL;
			}
			m_dwSize	= 0;
			m_dwPos		= 0;
		} while (false);
		return true;
	}

	bool CDMZipFile::IsOpen() const 
	{
		return NULL != m_pData;
	}

	BYTE* CDMZipFile::GetData() const
	{
		if (!IsOpen())
		{
			return NULL;
		}
		return m_pData;
	}

	DWORD CDMZipFile::GetSize() const
	{
		if (!IsOpen())
		{
			return NULL;
		}
		return m_dwSize;
	}

	DWORD CDMZipFile::GetPosition() const
	{
		if (!IsOpen())
		{
			return NULL;
		}
		return m_dwPos;
	}

	DWORD CDMZipFile::Seek(DWORD dwOffset, UINT nFrom)	//	return old pos
	{
		DWORD dwPos = m_dwPos;
		switch (nFrom)
		{
		case FILE_BEGIN:
			m_dwPos = dwOffset;
			break;
		case FILE_END:
			m_dwPos = m_dwSize + dwOffset;
			break;
		case FILE_CURRENT:
			m_dwPos += dwOffset;
			break;
		}
		if (m_dwPos < 0)
		{
			m_dwPos = 0;
		}
		if (m_dwPos >= m_dwSize)
		{
			m_dwPos = m_dwSize;
		}
		return dwPos;
	}

	bool CDMZipFile::Attach(LPBYTE pData, DWORD dwSize)
	{
		bool bRet = false;
		do 
		{
			if (m_pData)
			{
				break;
			}

			m_pData	 = pData;
			m_dwSize = dwSize;
			bRet	 = true;
		} while (false);
		return bRet;
	}

	void CDMZipFile::Detach()
	{
		m_pData  = NULL;
		m_dwSize = 0;
		m_dwPos  = 0;
	}

#ifdef ZLIB_DECRYPTION
	bool CDMZipFile::DecryptFile(LPCSTR lpszPassword, LPBYTE& pData, DWORD& dwSize, DWORD crc32)
	{
		bool bRet = false;
		do 
		{
			if (!InitKeys(lpszPassword))
			{
				break;
			}
			if (!DecryptHeader(pData, dwSize, crc32))
			{
				break;
			}
			if (!DecryptData(pData, dwSize))
			{
				break;
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	bool CDMZipFile::InitKeys(LPCSTR lpszPassword)
	{
		bool bRet = false;
		do 
		{
			if (NULL == lpszPassword||0==strlen(lpszPassword))
			{
				break;
			}

			m_pCrcTable = (const DWORD*)get_crc_table();
			m_dwKey[0]	= 305419896;
			m_dwKey[1]	= 591751049;
			m_dwKey[2]	= 878082192;
			int nLen = strlen(lpszPassword);
			for (int i = 0; i < nLen; i++)
			{
				UpdateKeys(lpszPassword[i]);
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	bool CDMZipFile::DecryptHeader(LPBYTE pData, DWORD dwSize, DWORD crc32)
	{
		bool bRet = false;
		do 
		{
			if (dwSize < 12)
			{
				break;
			}
			BYTE header[12];
			memcpy(&header, pData, 12);

			for (int i=0; i<12; i++)
			{
				BYTE c = (BYTE)(header[i] ^ DecryptByte());
				UpdateKeys(c);
				header[i] = c;
			}

			bRet = (header[11] == (BYTE)(crc32>>24));
		} while (false);
		return bRet;
	}

	bool CDMZipFile::DecryptData(LPBYTE& pData, DWORD& dwSize)
	{
		bool bRet = false;
		do 
		{
			LPBYTE pRawData = new BYTE[dwSize-12];
			if (NULL == pRawData)
			{
				break;
			}
			
			LPBYTE p = pRawData;
			for(DWORD i = 12; i < dwSize; i++)
			{
				BYTE c = (BYTE) (pData[i] ^ DecryptByte());
				UpdateKeys(c);
				*p++ = c;
			}
			delete[] pData;
			pData   = pRawData;
			dwSize -= 12;
			bRet = true;
		} while (false);
		return bRet;
	}

	void CDMZipFile::UpdateKeys(BYTE c)
	{      
		m_dwKey[0] = crc32(m_dwKey[0], c);
		m_dwKey[1] = m_dwKey[1] + (m_dwKey[0] & 0x000000FF);
		m_dwKey[1] = m_dwKey[1] * 134775813 + 1;
		BYTE b = (BYTE) (m_dwKey[1] >> 24);
		m_dwKey[2] = crc32(m_dwKey[2], b);
	}

	DWORD CDMZipFile::crc32(DWORD c, BYTE b)
	{
		DMASSERT(m_pCrcTable);
		return m_pCrcTable[((DWORD) (c) ^ (b)) & 0xFF] ^ ((c) >> 8);
	}

	BYTE CDMZipFile::DecryptByte() const
	{
		DWORD temp = (WORD) (m_dwKey[2] | 2);
		return (BYTE)((temp * (temp ^ 1)) >> 8);
	}
#endif	//ZLIB_DECRYPTION

	//---------------------------------------------------------------------
	CDMZipHelper::CDMZipHelper()
	{
		m_hFile	  = INVALID_HANDLE_VALUE;
		m_Files   = NULL;
		m_DirData = NULL;
		memset(&m_Header, 0, sizeof(m_Header));
	}

	CDMZipHelper::~CDMZipHelper()
	{
		Close();
	}


	bool CDMZipHelper::LoadFromFile(LPCWSTR lpszFilePath)
	{
		bool bRet = false;
		do 
		{
			if (NULL == lpszFilePath)
			{
				break;
			}
			HANDLE hFile = ::CreateFileW(lpszFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (INVALID_HANDLE_VALUE==hFile)
			{
				break;
			}

			Close();
			m_hFile	= hFile;
			bRet = OpenZip();
			if (!bRet)
			{
				CloseHandle(m_hFile);
				m_hFile = INVALID_HANDLE_VALUE;
			}
		} while (false);
		return bRet;
	}

	bool CDMZipHelper::LoadFromPE(HMODULE hModule, LPCWSTR lpszName, LPCWSTR lpszType/*=L"ZIP"*/)
	{
		bool bRet = false;
		do 
		{
			HRSRC hResInfo = ::FindResourceW(hModule, lpszName, lpszType);
			if (NULL == hResInfo)
			{
				break;
			}

			DWORD dwLength = ::SizeofResource(hModule, hResInfo);
			if (0 == dwLength)
			{
				break;
			}

			HGLOBAL hResData = ::LoadResource(hModule, hResInfo);
			if (NULL == hResData)
			{
				break;
			}

			BYTE* pData = (BYTE*)::LockResource(hResData);
			if (NULL == pData)
			{
				break;
			}

			Close();
			m_fileRes.Attach(pData, dwLength);

			m_hFile = (HANDLE)hResInfo;

			bRet = OpenZip();
			if(!bRet)
			{
				m_fileRes.Detach();
				m_hFile = INVALID_HANDLE_VALUE;
			}
		} while (false);
		return bRet;
	}

	bool CDMZipHelper::SetPassword(LPCSTR lpszPassword)
	{
		bool bRet = false;
		do 
		{
			if (!lpszPassword)
			{
				break;
			}

			if (strlen(lpszPassword) >= sizeof(m_szPassword)-1)
			{
				break;
			}
			strcpy_s(m_szPassword, lpszPassword);
			bRet = true;
		} while (false);
		return bRet;
	}

	bool CDMZipHelper::IsOpen() const
	{
		return (INVALID_HANDLE_VALUE!=m_hFile&&NULL!=m_hFile);
	}

	void CDMZipHelper::Close()
	{
		CloseFile();

		if (m_Files != NULL)
		{
			delete[] m_Files;
			m_Files = NULL;
		}
		if (m_DirData != NULL)
		{
			free(m_DirData);
			m_DirData = NULL;
		}
		memset(&m_Header, 0, sizeof(m_Header));
	}

	int CDMZipHelper::GetEntries() const
	{
		return m_Header.nDirEntries;
	}

	// ZIP File API
	int CDMZipHelper::GetFileIndex(LPCWSTR lpszFileName)
	{
		int nIndex = -1;
		do 
		{
			if (!IsOpen())
			{
				break;
			}

			ZIP_FIND_DATA fd;
			HANDLE hFindFile = FindFirstFile(lpszFileName, &fd);
			if (INVALID_HANDLE_VALUE == hFindFile)
			{
				break;
			}
			FindClose(hFindFile);
			nIndex = fd.nIndex;
		} while (false);

		return nIndex;
	}

	bool CDMZipHelper::GetFile(LPCWSTR lpszFileName, CDMZipFile&file)
	{
		return GetFile(GetFileIndex(lpszFileName),file);
	}

	bool CDMZipHelper::GetFile(int iIndex, CDMZipFile& file)
	{
		bool bRet = false;
		do 
		{
			if (!IsOpen())
			{
				break;
			}
			if (NULL == m_hFile||INVALID_HANDLE_VALUE == m_hFile)
			{
				break;
			}
			if (iIndex < 0 || iIndex >= m_Header.nDirEntries)
			{
				break;
			}

			ZipLocalHeader hdr;
			SeekFile(m_Files[iIndex]->hdrOffset, FILE_BEGIN);
			DWORD dwRead = ReadFile(&hdr, sizeof(hdr));
			if (dwRead != sizeof(hdr))
			{
				break;
			}
			if (hdr.sig != LOCAL_SIGNATURE)
			{
				break;
			}
			SeekFile(hdr.fnameLen + hdr.xtraLen, FILE_CURRENT);

			// Decompress file if needed.
			LPBYTE pData;
			pData = new BYTE[hdr.cSize];

			if (pData == NULL)
			{
				break;
			}

			dwRead = ReadFile(pData, hdr.cSize);
			if (dwRead != hdr.cSize)
			{
				delete[] pData;
				break;
			}

			DWORD dwSize = hdr.cSize;
			if (hdr.flag&1)
			{
#ifdef ZLIB_DECRYPTION
				if (0 == strlen(m_szPassword))
				{
					delete[] pData;
					break;
				}
				if (!file.DecryptFile(m_szPassword, pData, dwSize, hdr.crc32))
				{
					delete[] pData;
					break;
				}
#else
				delete[] pData;
				break;
#endif
			}

			bRet = true; // 
			switch (hdr.compression)
			{
			case LOCAL_COMP_STORE:
				break;
			case LOCAL_COMP_DEFLAT: 
				{
					LPBYTE pTarget;
					pTarget = new BYTE[hdr.ucSize];
					z_stream stream = { 0 };
					stream.next_in = (Bytef*) pData;
					stream.avail_in = (uInt) hdr.cSize;
					stream.next_out = (Bytef*) pTarget;
					stream.avail_out = hdr.ucSize;
					stream.zalloc = (alloc_func) NULL;
					stream.zfree = (free_func) NULL;
					// Perform inflation; wbits < 0 indicates no zlib header inside the data.
					int err = inflateInit2(&stream, -MAX_WBITS);
					if (err == Z_OK)
					{
						err = inflate(&stream, Z_OK);
						if (err != Z_OK && err != Z_STREAM_END)
						{
							break;
						}
						inflateEnd(&stream);
						if (err == Z_STREAM_END)
							err = Z_OK;
						inflateEnd(&stream);
					}

					delete[] pData;
					if (err != Z_OK)
					{
						delete[] pTarget;
						bRet = false;
						break;// 仅跳出switch
					}
					pData = pTarget;
				}
				break;
			default:
				bRet = false;
				break;// 仅跳出switch
			}

			if (bRet)
			{
				// The memory we allocated is passed to the file, which
				// takes ownership of it.
				file.Attach(pData, hdr.ucSize);
			}

		} while (false);
		return bRet;
	}

	DWORD CDMZipHelper::GetFileSize(LPCWSTR lpszFileName)
	{
		return GetFileSize(GetFileIndex(lpszFileName));
	}

	DWORD CDMZipHelper::GetFileSize(int iIndex)
	{
		DWORD dwSize = 0;
		do 
		{
			if (-1 == iIndex)
			{
				break;
			}

			if (NULL == m_hFile||INVALID_HANDLE_VALUE == m_hFile)
			{
				break;
			}

			if (iIndex < 0||iIndex >= m_Header.nDirEntries)
			{
				break;
			}

			ZipLocalHeader hdr;
			SeekFile(m_Files[iIndex]->hdrOffset, FILE_BEGIN);

			DWORD dwRead = ReadFile(&hdr, sizeof(hdr));
			if (dwRead != sizeof(hdr))
			{
				break;
			}
			dwSize = hdr.ucSize;

		} while (false);

		return dwSize;
	}

	// FindFile API
	HANDLE CDMZipHelper::FindFirstFile(LPCWSTR lpszFileName, LPZIP_FIND_DATA lpFindFileData) const
	{
		FindFileHandle* pFF = NULL;
		do 
		{
			if (!IsOpen())
			{
				break;
			}
			::ZeroMemory(lpFindFileData, sizeof(ZIP_FIND_DATA));
			if (lpszFileName == NULL || lpszFileName[0] == '\0')
			{
				break;
			}

			pFF = new FindFileHandle;
			if (pFF == NULL)
			{
				break;
			}

			wcscpy_s(pFF->szSearch, lpszFileName);
			pFF->nPos = 0;

			if (!FindNextFile((HANDLE)pFF, lpFindFileData))
			{
				delete pFF;
				pFF = NULL;
				break;
			}
		} while (false);
		if (NULL == pFF)
		{
			return INVALID_HANDLE_VALUE;
		}
		else
		{
			return (HANDLE)pFF;
		}
	}

	bool CDMZipHelper::FindNextFile(HANDLE hFindFile, LPZIP_FIND_DATA lpFindFileData) const
	{
		bool bRet = false;
		do 
		{
			if (!IsOpen())
			{
				break;
			}
			if (INVALID_HANDLE_VALUE == hFindFile||NULL == hFindFile)
			{
				break;
			}
			FindFileHandle* pFF = reinterpret_cast<FindFileHandle*>(hFindFile);

			bRet = true;// 预先设置为真
			while (TRUE)
			{
				if (pFF->nPos >= m_Header.nDirEntries)
				{
					bRet = false;// 失败了
					break;
				}
				// Extract filename and match with pattern
				ZipDirFileHeader* fh = m_Files[pFF->nPos];
				wchar_t szFile[MAX_PATH] = { 0 };
				::OemToCharBuffW(fh->GetName(), szFile, fh->fnameLen);
				//CStringW strInfo = szFile;strInfo += L"\n";// 这个是高效率调用地方，使用打印会导致解析慢
				//OutputDebugStringW(strInfo);
#ifdef ZIP_WILDCARD
				if (::PathMatchSpecW(szFile, pFF->szSearch) != NULL)
#else
				if (0 == _wcsicmp(szFile, pFF->szSearch))
#endif
				{
					// Copy data to the ZIP_FIND_DATA structure
					wcscpy_s(lpFindFileData->szFileName, szFile);
					lpFindFileData->szComment[0] = _T('\0'); // unsupported right now
					lpFindFileData->nFileSizeCompressed = fh->cSize;
					lpFindFileData->nFileSizeUncompressed = fh->ucSize;
					::DosDateTimeToFileTime(fh->modDate, fh->modTime, &lpFindFileData->ftCreationDate);
					lpFindFileData->nIndex = pFF->nPos;

					// Figure out the file attributes
					DWORD& dwFlags = lpFindFileData->dwFileAttributes = 0;
					if (fh->flag & 1)
						dwFlags |= FILE_ATTRIBUTE_ENCRYPTED;
					if (fh->flag & (2|4))
						dwFlags |= FILE_ATTRIBUTE_COMPRESSED;
					if (fh->flag & (16|32|64))
						dwFlags |= FILE_ATTRIBUTE_OFFLINE; // unsupported compression used
					if (dwFlags == 0)
						dwFlags = (fh->compression == LOCAL_COMP_STORE) ? FILE_ATTRIBUTE_DIRECTORY : FILE_ATTRIBUTE_NORMAL;

					// Ready for next entry...
					pFF->nPos++;
					break; //查找成功
				}
				pFF->nPos++;
			}
		} while (FALSE);
		return bRet;
	}

	bool CDMZipHelper::FindClose(HANDLE hFindFile) const
	{
		bool bRet = false;
		do 
		{
			if (INVALID_HANDLE_VALUE == hFindFile||NULL == hFindFile)
			{
				break;
			}

			FindFileHandle* pFF = reinterpret_cast<FindFileHandle*>(hFindFile);
			delete pFF;
			bRet = true;
		} while (false);
		return bRet;
	}

	// 辅助
	bool CDMZipHelper::OpenZip()
	{
		bool bRet = false;
		do 
		{
			if (NULL == m_hFile||INVALID_HANDLE_VALUE == m_hFile)
			{
				break;
			}
			SeekFile(-(LONG)sizeof(m_Header), FILE_END);
			DWORD dwRead = ReadFile(&m_Header, sizeof(m_Header));
			if (dwRead != sizeof(m_Header) || m_Header.sig != DIR_SIGNATURE)
			{
				Close();
				break;
			}
			if (m_Header.nDirEntries>=1000)// Sanity check
			{
				break;
			}
			m_DirData = (LPBYTE)malloc(m_Header.dirSize);
			m_Files = new ZipDirFileHeader*[m_Header.nDirEntries];
			if (NULL == m_Files||NULL == m_DirData)
			{
				Close();
				break;
			}
			SeekFile(-(LONG)(sizeof(m_Header) + m_Header.dirSize), FILE_END);
			dwRead = ReadFile(m_DirData, m_Header.dirSize);
			if (dwRead != m_Header.dirSize)
			{
				Close();
				break;
			}

			// 开始读文件内容
			bRet = true;
			LPBYTE pData = m_DirData;
			for (int i=0; i < m_Header.nDirEntries; i++)
			{
				// Set the header pointer in the m_Files array
				ZipDirFileHeader* fh = (ZipDirFileHeader*) pData;
				m_Files[i] = fh;
				if (fh->sig != FILE_SIGNATURE)
				{
					Close();
					bRet = false; // 读失败了
					break;
				}
				// Convert UNIX slash to Windows backslash in ANSI string
				LPSTR pszName = fh->GetName();
				for(int j = 0; j < fh->fnameLen; j++, pszName++)
					if (*pszName == '/')
						*pszName='\\';

				// Get next header
				pData += sizeof(ZipDirFileHeader) + fh->fnameLen + fh->xtraLen + fh->cmntLen;
			}
			m_szPassword[0] = '\0';
		} while (false);
		return bRet;
	}

	void CDMZipHelper::CloseFile()
	{
		if (INVALID_HANDLE_VALUE != m_hFile&&NULL != m_hFile)
		{
			if (m_fileRes.IsOpen())
				m_fileRes.Detach();
			else
				::CloseHandle(m_hFile);
			m_hFile = INVALID_HANDLE_VALUE;
		}
	}

	DWORD CDMZipHelper::ReadFile(void* pBuffer, DWORD dwBytes)
	{
		DWORD dwRead = 0;
		do 
		{
			if (NULL == m_hFile||INVALID_HANDLE_VALUE == m_hFile)
			{
				break;
			}

			if (m_fileRes.IsOpen())
				m_fileRes.Read(pBuffer, dwBytes, &dwRead);
			else
				::ReadFile(m_hFile, pBuffer, dwBytes, &dwRead, NULL);

		} while (false);
		return dwRead;
	}

	DWORD CDMZipHelper::SeekFile(LONG lOffset, UINT nFrom)
	{
		if (NULL == m_hFile||INVALID_HANDLE_VALUE == m_hFile)
		{
			return 0;
		}

		if (m_fileRes.IsOpen())
		{
			m_fileRes.Seek(lOffset, nFrom);
			return m_fileRes.GetPosition();
		}

		return ::SetFilePointer(m_hFile, lOffset, NULL, nFrom);
	}

}//namespace DM