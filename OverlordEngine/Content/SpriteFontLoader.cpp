#include "stdafx.h"
#include "SpriteFontLoader.h"

SpriteFont* SpriteFontLoader::LoadContent(const ContentLoadInfo& loadInfo)
{
	const auto pReader = new BinaryReader();
	pReader->Open(loadInfo.assetFullPath);

	if (!pReader->Exists())
	{
		Logger::LogError(L"Failed to read the assetFile!\nPath: \'{}\'", loadInfo.assetSubPath);
		return nullptr;
	}

	TODO_W7(L"Implement SpriteFontLoader >> Parse .fnt file")
	//See BMFont Documentation for Binary Layout

	//Parse the Identification bytes (B,M,F)
	//If Identification bytes doesn't match B|M|F,
	//Log Error (SpriteFontLoader::LoadContent > Not a valid .fnt font) &
	//return nullptr
	const auto id0 = pReader->Read<char>();
	const auto id1 = pReader->Read<char>();
	const auto id2 = pReader->Read<char>();
	const auto version = pReader->Read<char>();

	//		   B			M			 F
	if (id0 != 66 && id1 != 77 && id2 != 70)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Not a valid.fnt font");
		return nullptr;
	}

	//Parse the version (version 3 required)
	//If version is < 3,
	//Log Error (SpriteFontLoader::LoadContent > Only .fnt version 3 is supported)
	//return nullptr
	if (version < 3)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Only .fnt version 3 is supported");
		return nullptr;
	}

	//Valid .fnt file >> Start Parsing!
	//use this SpriteFontDesc to store all relevant information (used to initialize a SpriteFont object)
	SpriteFontDesc fontDesc{};

	//**********
	// BLOCK 0 *
	//**********
	//Retrieve the blockId and blockSize
	auto blockId = pReader->Read<char>();
	auto blockSize = pReader->Read<int>();

	//Retrieve the FontSize [fontDesc.fontSize]
	fontDesc.fontSize = pReader->Read<short>();

	//Move the binreader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	pReader->MoveBufferPosition(12);

	//Retrieve the FontName [fontDesc.fontName]
	std::wstring fontName = pReader->ReadNullString();
	if (fontName.empty())
	{
		Logger::LogError(L"Failed to parse font data: invalid font name");
	}
	fontDesc.fontName = fontName;

	//**********
	// BLOCK 1 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = pReader->Read<char>();
	blockSize = pReader->Read<int>();
	pReader->MoveBufferPosition(4);

	//Retrieve Texture Width & Height [fontDesc.textureWidth/textureHeight]
	fontDesc.textureWidth = pReader->Read<unsigned short>();
	fontDesc.textureHeight = pReader->Read<unsigned short>();

	//Retrieve PageCount
	const auto pageCount = pReader->Read<unsigned short>();
	//> if pagecount > 1
	if (pageCount > 1)
	{
		//	> Log Error (Only one texture per font is allowed!)
		Logger::LogError(L"Only one texture per font is allowed!");
	}
	
	//Advance to Block2 (Move Reader)
	pReader->MoveBufferPosition(blockSize - 10);

	//**********
	// BLOCK 2 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = pReader->Read<char>();
	blockSize = pReader->Read<int>();

	//Retrieve the PageName (BinaryReader::ReadNullString)
	const std::wstring pageName = pReader->ReadNullString();

	//Construct the full path to the page texture file
	//	>> page texture should be stored next to the .fnt file, pageName contains the name of the texture file
	//	>> full texture path = asset parent_path of .fnt file (see loadInfo.assetFullPath > get parent_path) + pageName (filesystem::path::append)
	std::filesystem::path texturePath = loadInfo.assetFullPath.parent_path();
	texturePath.append(pageName);

	//	>> Load the texture (ContentManager::Load<TextureData>) & Store [fontDesc.pTexture]	
	fontDesc.pTexture = ContentManager::Load<TextureData>(texturePath);

	//**********
	// BLOCK 3 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = pReader->Read<char>();
	blockSize = pReader->Read<int>();

	//Retrieve Character Count (see documentation)
	const int charCount = blockSize / 20;

	//Create loop for Character Count, and:
	for (int i = 0; i < charCount; i++)
	{
		//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
		const auto charId = static_cast<wchar_t>(pReader->Read<unsigned int>());

		//> Create instance of FontMetric (struct)
		FontMetric fontMetric{};

		//	> Set Character (CharacterId) [FontMetric::character]
		fontMetric.character = wchar_t(charId);

		//	> Retrieve Xposition (store Local)
		const auto xPos = pReader->Read<unsigned short>();
		//	> Retrieve Yposition (store Local)
		const auto yPos = pReader->Read<unsigned short>();

		//Retrieve & Set Width [FontMetric::width]
		fontMetric.width = pReader->Read<unsigned short>();

		//Retrieve & Set Height [FontMetric::height]
		fontMetric.height = pReader->Read<unsigned short>();

		//Retrieve & Set OffsetX [FontMetric::offsetX]
		fontMetric.offsetX = pReader->Read<short>();

		//Retrieve & Set OffsetY [FontMetric::offsetY]
		fontMetric.offsetY = pReader->Read<short>();

		//Retrieve & Set AdvanceX [FontMetric::advanceX]
		fontMetric.advanceX = pReader->Read<short>();

		//Retrieve & Set Page [FontMetric::page]
		fontMetric.page = pReader->Read<unsigned char>();

		//	> Retrieve Channel (BITFIELD!!!) 
		const auto channel = pReader->Read<unsigned char>();
		
		//	> See documentation for BitField meaning [FontMetrix::channel]
		switch (channel)
		{
		case 1:
			fontMetric.channel = 2;
			break;
		case 2:
			fontMetric.channel = 1;
			break;
		case 4:
			fontMetric.channel = 0;
			break;
		case 8:
			fontMetric.channel = 3;
			break;
		default:
			break;
		}

		//	> Calculate Texture Coordinates using Xposition, Yposition, fontDesc.TextureWidth & fontDesc.TextureHeight [FontMetric::texCoord]
		fontMetric.texCoord.x = static_cast<float>(xPos) / static_cast<float>(fontDesc.textureWidth);
		fontMetric.texCoord.y = static_cast<float>(yPos) / static_cast<float>(fontDesc.textureHeight);

		//> Insert new FontMetric to the metrics [font.metrics] map
		//	> key = (wchar_t) charId
		//	> value = new FontMetric
		fontDesc.metrics.insert(std::pair(wchar_t(charId), fontMetric));
		//(loop restarts till all metrics are parsed)
	}

	//Done!
	delete pReader;
	return new SpriteFont(fontDesc);
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}
