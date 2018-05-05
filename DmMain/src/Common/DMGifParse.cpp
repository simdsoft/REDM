#include "DmMainAfx.h"
#include "DMGifParse.h"

namespace DM
{
	DMGifParse::DMGifParse()
	{
		m_nOffset = 0;
		memset(&m_ctrlExt,0,sizeof(GCTRLEXT));
		memset(&m_gInfo,0,sizeof(GIFGLOBALINFO));
	}

	DMGifParse::~DMGifParse()
	{	
		RemoveAll();
	}

	bool DMGifParse::LoadFromFile(LPCWSTR lpszFilePath)
	{
		bool bRet = false;
		FILE *fp = NULL;
		do 
		{
			_wfopen_s(&fp, lpszFilePath, L"rb");
			if (!fp)
			{
				break;
			}

			/// 头部(6个字节)
			char szSignature[7] = {0};
			fread(szSignature,1,6, fp);						// 标识符(3 字节) ---GIF
			if (0 != strncmp(szSignature,"GIF89a",6))		// 版本(3 字节)  89a-我们只支持89版本！hgy note
			{
				break;
			}

			fread((char*)&m_gInfo.scrWidth,1,2, fp);		// 逻辑屏幕宽(2字节)
			fread((char*)&m_gInfo.scrHeight,1,2, fp);		// 逻辑屏幕高(2字节)

			BYTE be;
			fread((char*)&be,1,1,fp);						// Packed Fields, Global Color Table Flag(1字节)
			if ((be&0x80) != 0)
			{
				m_gInfo.gFlag = true;
			}
			else
			{
				m_gInfo.gFlag = false;
			}
			m_gInfo.colorRes = ((be&0x70)>>4)+1;			// 色彩分辨率
			if (m_gInfo.gFlag)
			{	
				if((be&0x08) != 0)
					m_gInfo.gSort = true;					// 全局调色板是否按优先排序
				else
					m_gInfo.gSort = false;			
				m_gInfo.gSize = 1;
				m_gInfo.gSize <<= ((be&0x07)+1);
			}
			fread((char*)&be,1,1,fp);
			m_gInfo.BKColorIdx = be;						// Background Color Index背景色的调色板索引(1字节)
			fread((char*)&be,1,1,fp);				
			m_gInfo.pixelAspectRatio = be;					// Pixel Aspect Ratio像素长宽比例(1字节)

			// 如果有全局色表，接着逻辑视屏描述块之后，占用的字节数为3*2^（全局色表尺寸+1）
			if (m_gInfo.gFlag)
			{	
				fseek(fp, m_gInfo.gSize*3,SEEK_CUR);
			}

			bRet = GetAllFrames(fp);
			if (bRet)
			{
				m_gInfo.frames = (UINT)GetCount();
			}
		} while (false);
		if (fp)
		{
			fclose(fp);
		}
		return bRet;
	}

	bool DMGifParse::LoadFromMemory(BYTE *pBuf,int bufLen)
	{
		bool bRet = false;
		do 
		{
			m_nOffset = 0;
			if (NULL == pBuf||bufLen<=380)
			{
				break;
			}

			/// 头部(6个字节)
			if (0 != strncmp((char*)pBuf+m_nOffset,"GIF89a",6))
			{
				break;
			}
			m_nOffset += 6;

			memcpy((void*)&m_gInfo.scrWidth, pBuf+m_nOffset,2);m_nOffset+=2;	// 逻辑屏幕宽(2字节)
			memcpy((char*)&m_gInfo.scrHeight, pBuf+m_nOffset,2);m_nOffset+=2;	// 逻辑屏幕宽(2字节)

			BYTE be;
			memcpy((char*)&be, pBuf+m_nOffset,1);m_nOffset+=1;					// Packed Fields, Global Color Table Flag(1字节)
			if ((be&0x80) != 0)
			{
				m_gInfo.gFlag = true;
			}
			else
			{
				m_gInfo.gFlag = false;
			}
			m_gInfo.colorRes = ((be&0x70)>>4)+1;								// 色彩分辨率（不使用）
			if (m_gInfo.gFlag)
			{	
				if((be&0x08) != 0)
					m_gInfo.gSort = true;										// 全局调色板是否按优先排序
				else
					m_gInfo.gSort = false;			
				m_gInfo.gSize = 1;
				m_gInfo.gSize <<= ((be&0x07)+1);
			}
			memcpy((char*)&be, pBuf+m_nOffset,1);m_nOffset+=1;
			m_gInfo.BKColorIdx = be;											// Background Color Index背景色的调色板索引(1字节)
			//fread((char*)&be,1,1,fp);		
			memcpy((char*)&be, pBuf+m_nOffset,1);m_nOffset+=1;
			m_gInfo.pixelAspectRatio = be;										// Pixel Aspect Ratio像素长宽比例(1字节)

			// 如果有全局色表，接着逻辑视屏描述块之后，占用的字节数为3*2^（全局色表尺寸+1）
			if (m_gInfo.gFlag)
			{	
				m_nOffset+=m_gInfo.gSize*3;
			}

			bRet = GetAllFrames(pBuf,bufLen);
			if (bRet)
			{
				m_gInfo.frames = (UINT)GetCount();
			}
		} while (false);
		return bRet;
	}

	GIFFRAMEPTR DMGifParse::GetFrame(int iElement)
	{
		GIFFRAMEPTR pf = NULL;
		GetObj(iElement,pf);
		return pf;
	}

	bool DMGifParse::GetAllFrames(FILE*fp)
	{	
		bool bRet = true;
		BYTE be;
		bool fileEnd = false;			// 尾记录,用来指示该数据流的结束。取固定值0x3b.
		while (!feof(fp)&&!fileEnd)
		{
			fread((char*)&be,1,1,fp);	//(1字节)
			switch (be)
			{	
			case 0x21:					// Extension Introducer - 标识这是一个扩展块，固定值0×21,这是个可有可无的块，有的话就跳过它
				{
					if (!ParseExtension(fp, m_ctrlExt))
					{
						break;
					}
				}
				break;
			case 0x2c:				   // 图像分隔符 - 用于识别图像描述符的开始。取固定值0x2c
				{
					if (!ParseFrame(fp))
					{
						break;
					}
				}
				break;

			case 0x3b:				  // 尾记录,用来指示该数据流的结束。取固定值0x3b.
				{
					fileEnd = true;
				}
				break;

			case 0x00:
				break;

			default:
				{
					bRet = false;
				}
				break;
			}
		}
		return bRet;
	}


	bool DMGifParse::ParseExtension(FILE*fp,GCTRLEXT&ctrlExt)
	{
		bool bRet = true;
		BYTE be;
		fread((char*)&be,1,1,fp);//(1字节)
		switch (be)
		{	
		case 0xf9:// Graphic Control Label - 标识这是一个图形控制扩展块，固定值0xF9
			{
				while (!feof(fp))
				{	
					fread((char*)&be,1,1,fp);
					if (0 == be)// 块终结，固定值0
						break;
					if (4 == be)// Block Size - 不包括块终结器，固定值4
					{
						ctrlExt.active = true;
						fread((char*)&be,1,1,fp);
						ctrlExt.disposalMethod = (be&0x1c)>>2;
						if ((be&0x02) != 0)
							ctrlExt.userInputFlag = true;
						else
							ctrlExt.userInputFlag = false;
						if((be&0x01) != 0)
							ctrlExt.trsFlag = true;
						else
							ctrlExt.trsFlag = false;
						fread((char*)&ctrlExt.delayTime,1,2,fp);//Delay Time - 单位1/100秒(2字节）
						fread((char*)&be,1,1,fp);
						ctrlExt.trsColorIndex = be;
					}
					else
						fseek(fp, be, SEEK_CUR);// 跳过be个字节
				}
			}
			break;
		case 0xfe:// Comment Label - 标识这是一个注释块，固定值0xFE
		case 0x01:
		case 0xff:// Application Extension Label - 标识这是一个应用程序扩展块，固定值0xFF
			{
				if (false == m_gInfo.bLoop)
				{
					char byChar[17] = {0x0B,'N','E','T','S','C','A','P','E','2','.','0',0x03,0x01,0x00,0x00,0x00};
					DWORD pos = ftell(fp);
					char byChar1[17]= {0};
					fread(byChar1,1,17,fp);
					if (0 == strncmp(byChar1,byChar,14))
					{	
						int iHigh = (int)(byte)byChar1[15];int iLow = (int)(byte)byChar1[14];
						m_gInfo.totalLoopCount = iLow+iHigh*256;
						m_gInfo.bLoop = true;
					}
					fseek(fp, pos, SEEK_SET);
				}
			
				while (!feof(fp)) // 无论上面哪一块，最终都会有个块终结，固定值0
				{	
					fread((char*)&be,1,1,fp);
					if (0 == be)// 块终结，固定值0
						break;
					fseek(fp, be, SEEK_CUR);// 跳过be个字节
				}
			}
			break;
		default:
			bRet = false;
			break;
		}
		return bRet;
	}

	bool DMGifParse::ParseFrame(FILE*fp)
	{
		GIFFRAMEPTR pf = new GIFFRAME;
		memset(pf, 0,sizeof(GIFFRAME));

		fread((char*)&pf->imageLPos,1,2,fp);
		fread((char*)&pf->imageTPos,1,2,fp);
		fread((char*)&pf->imageWidth,1,2,fp);
		fread((char*)&pf->imageHeight,1,2,fp);

		BYTE be;
		UINT lSize = 1;
		bool lFlag = false;
		fread((char*)&be,1,1,fp);
		if ((be&0x80) != 0)
			lFlag = true;
		lSize <<= ((be&0x07)+1);// 颜色列表的位数
		if (lFlag)
			fseek(fp,lSize*3,SEEK_CUR);
		fread((char*)&be,1,1,fp);
		while (!feof(fp))
		{	
			fread((char*)&be,1,1,fp);
			if (0 == be)
				break;
			fseek(fp, be, SEEK_CUR);// 跳过be个字节
		}

		if(m_ctrlExt.active)
		{
			pf->ctrlExt = m_ctrlExt;
			m_ctrlExt.active = false;
		}

		AddObj(pf);
		return true;
	}

#define CMPLEN(x,y)  if(x>y){return false;}
	bool DMGifParse::GetAllFrames(BYTE *pBuf,int bufLen)
	{
		bool bRet = true;
		BYTE be;
		bool fileEnd = false;// 尾记录,用来指示该数据流的结束。取固定值0x3b.
		while (m_nOffset<=bufLen&&!fileEnd)
		{
			memcpy((char*)&be,pBuf+m_nOffset,1);m_nOffset+=1;//(1字节)
			switch (be)
			{	
			case 0x21:			// Extension Introducer - 标识这是一个扩展块，固定值0×21,这是个可有可无的块，有的话就跳过它
				{
					if (!ParseExtension(pBuf,bufLen, m_ctrlExt))
					{
						break;
					}
				}
				break;
			case 0x2c:         // 图像分隔符 - 用于识别图像描述符的开始。取固定值0x2c
				{
					if (!ParseFrame(pBuf,bufLen))
					{
						break;
					}
				}
				break;

			case 0x3b:		 // 尾记录,用来指示该数据流的结束。取固定值0x3b.
				{
					fileEnd = true;
				}
				break;

			case 0x00:
				break;

			default:
				{
					bRet = false;
				}
				break;
			}
		}
		return bRet;
	}

	bool DMGifParse::ParseExtension(BYTE *pBuf,int bufLen,GCTRLEXT&ctrlExt)
	{
		bool bRet = true;
		BYTE be;
		memcpy((char*)&be,pBuf+m_nOffset,1);m_nOffset+=1;CMPLEN(m_nOffset,bufLen);//(1字节)
		switch (be)
		{	
		case 0xf9:// Graphic Control Label - 标识这是一个图形控制扩展块，固定值0xF9
			{
				while (m_nOffset<=bufLen)
				{	
					memcpy((char*)&be,pBuf+m_nOffset,1);m_nOffset+=1;
					if (0 == be)// 块终结，固定值0
						break;
					if (4 == be)// Block Size - 不包括块终结器，固定值4
					{
						ctrlExt.active = true;
						memcpy((char*)&be,pBuf+m_nOffset,1);m_nOffset+=1;CMPLEN(m_nOffset,bufLen);;
						ctrlExt.disposalMethod = (be&0x1c)>>2;
						if ((be&0x02) != 0)
							ctrlExt.userInputFlag = true;
						else
							ctrlExt.userInputFlag = false;
						if((be&0x01) != 0)
							ctrlExt.trsFlag = true;
						else
							ctrlExt.trsFlag = false;
						memcpy((char*)&ctrlExt.delayTime,pBuf+m_nOffset,2);m_nOffset+=2;CMPLEN(m_nOffset,bufLen);//Delay Time - 单位1/100秒(2字节）
						memcpy((char*)&be,pBuf+m_nOffset,1);m_nOffset+=1;CMPLEN(m_nOffset,bufLen);
						ctrlExt.trsColorIndex = be;
					}
					else
						m_nOffset += (int)(byte)be;CMPLEN(m_nOffset,bufLen);// 跳过be个字节
				}
			}
			break;
		case 0xfe:// Comment Label - 标识这是一个注释块，固定值0xFE
		case 0x01:
		case 0xff:// Application Extension Label - 标识这是一个应用程序扩展块，固定值0xFF
			{
				if (false == m_gInfo.bLoop)
				{
					char byChar[17] = {0x0B,'N','E','T','S','C','A','P','E','2','.','0',0x03,0x01,0x00,0x00,0x00};
					char byChar1[17]= {0};
					m_nOffset+=17;CMPLEN(m_nOffset,bufLen);m_nOffset-=17;
					memcpy(byChar1, pBuf+m_nOffset,17);CMPLEN(m_nOffset,bufLen);
					if (0 == strncmp(byChar1,byChar,14))
					{	
						int iHigh = (int)(byte)byChar1[15];int iLow = (int)(byte)byChar1[14];
						m_gInfo.totalLoopCount = iLow+iHigh*256;
						m_gInfo.bLoop = true;
					}
				}

				while (m_nOffset<=bufLen) // 无论上面哪一块，最终都会有个块终结，固定值0
				{	
					memcpy((char*)&be,pBuf+m_nOffset,1);m_nOffset+=1;CMPLEN(m_nOffset,bufLen);
					if (0 == be)// 块终结，固定值0
						break;
					m_nOffset+=be;CMPLEN(m_nOffset,bufLen);// 跳过be个字节
				}
			}
			break;
		default:
			bRet = false;
			break;
		}
		return bRet;
	}

#define CMPLENDEL(x,y,z)  if(x>y){delete[]z;return false;}
	bool DMGifParse::ParseFrame(BYTE *pBuf,int bufLen)
	{
		GIFFRAMEPTR pf = new GIFFRAME;

		memcpy((char*)&pf->imageLPos,pBuf+m_nOffset,2);m_nOffset+=2;CMPLENDEL(m_nOffset,bufLen,pf);
		memcpy((char*)&pf->imageTPos,pBuf+m_nOffset,2);m_nOffset+=2;CMPLENDEL(m_nOffset,bufLen,pf);
		memcpy((char*)&pf->imageWidth,pBuf+m_nOffset,2);m_nOffset+=2;CMPLENDEL(m_nOffset,bufLen,pf);
		memcpy((char*)&pf->imageHeight,pBuf+m_nOffset,2);m_nOffset+=2;CMPLENDEL(m_nOffset,bufLen,pf);

		BYTE be;
		UINT lSize = 1;
		bool lFlag = false;
		memcpy((char*)&be,pBuf+m_nOffset,1);m_nOffset+=1;CMPLENDEL(m_nOffset,bufLen,pf);
		if ((be&0x80) != 0)
			lFlag = true;
		lSize <<= ((be&0x07)+1);// 颜色列表的位数
		if (lFlag)
			m_nOffset+=lSize*3;CMPLENDEL(m_nOffset,bufLen,pf);
		memcpy((char*)&be,pBuf+m_nOffset,1);m_nOffset+=1;CMPLENDEL(m_nOffset,bufLen,pf);
		while (m_nOffset<=bufLen)
		{	
			memcpy((char*)&be,pBuf+m_nOffset,1);m_nOffset+=1;CMPLENDEL(m_nOffset,bufLen,pf);
			if (0 == be)
				break;
			m_nOffset+=be;CMPLENDEL(m_nOffset,bufLen,pf);// 跳过be个字节
		}

		if(m_ctrlExt.active)
		{
			pf->ctrlExt = m_ctrlExt;
			m_ctrlExt.active = false;
		}

		AddObj(pf);
		return true;
	}

	void DMGifParse::PreArrayObjRemove(const GIFFRAMEPTR &obj)
	{
		delete obj;
	}

}//namespace DM