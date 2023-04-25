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
	const auto blockId = pReader->Read<char>();
	const auto blockSize = pReader->Read<int>();
	blockSize;
	blockId;

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
	fontDesc.fontName = std::wstring(fontName.begin(), fontName.end());

	//**********
	// BLOCK 1 *
	//**********
	//Retrieve the blockId and blockSize
	const auto blockId1 = pReader->Read<char>();
	const auto blockSize1 = pReader->Read<int>();
	blockSize1;
	blockId1;

	//Retrieve Texture Width & Height [fontDesc.textureWidth/textureHeight]
	fontDesc.textureWidth = pReader->Read<short>();
	fontDesc.textureHeight = pReader->Read<short>();

	//Retrieve PageCount
	const auto pageCount = pReader->Read<char>();
	//> if pagecount > 1
	if (pageCount > 1)
	{
		//	> Log Error (Only one texture per font is allowed!)
		Logger::LogError(L"Only one texture per font is allowed!");
	}
	
	//Advance to Block2 (Move Reader)
	pReader->MoveBufferPosition(blockSize1 - 10);

	//**********
	// BLOCK 2 *
	//**********
	//Retrieve the blockId and blockSize
	const auto blockId2 = pReader->Read<char>();
	const auto blockSize2 = pReader->Read<int>();
	blockSize2;
	blockId2;
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
	const auto blockId3 = pReader->Read<char>();
	const auto blockSize3 = pReader->Read<int>();
	blockSize3;
	blockId3;

	//Retrieve Character Count (see documentation)
	const auto charCount = pReader->Read<int>();

	//Create loop for Character Count, and:
	for (int i = 0; i < charCount; i++)
	{
		//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
		const auto charId = static_cast<wchar_t>(pReader->Read<int>());

		//> Create instance of FontMetric (struct)
		FontMetric fontMetric;

		//	> Set Character (CharacterId) [FontMetric::character]
		fontMetric.character = charId;

		//	> Retrieve Xposition (store Local)
		const auto xPos = pReader->Read<short>();
		//	> Retrieve Yposition (store Local)
		const auto yPos = pReader->Read<short>();

		//Retrieve & Set Width [FontMetric::width]
		fontMetric.width = pReader->Read<short>();

		//Retrieve & Set Height [FontMetric::height]
		fontMetric.height = pReader->Read<short>();

		//Retrieve & Set OffsetX [FontMetric::offsetX]
		fontMetric.offsetX = pReader->Read<short>();

		//Retrieve & Set OffsetY [FontMetric::offsetY]
		fontMetric.offsetY = pReader->Read<short>();

		//Retrieve & Set AdvanceX [FontMetric::advanceX]
		fontMetric.advanceX = pReader->Read<short>();

		//Retrieve & Set Page [FontMetric::page]
		fontMetric.page = pReader->Read<char>();

		//	> Retrieve Channel (BITFIELD!!!) 
		const auto channel = pReader->Read<char>();
		
		//	> See documentation for BitField meaning [FontMetrix::channel]
		fontMetric.channel = (channel & 1) != 0;
		fontMetric.channel = (channel & 2) != 0;
		fontMetric.channel = (channel & 4) != 0;
		fontMetric.channel = (channel & 8) != 0;

		//	> Calculate Texture Coordinates using Xposition, Yposition, fontDesc.TextureWidth & fontDesc.TextureHeight [FontMetric::texCoord]
		auto left = static_cast<float>(xPos) / static_cast<float>(fontDesc.textureWidth);
		auto top = static_cast<float>(yPos) / static_cast<float>(fontDesc.textureHeight);
		fontMetric.texCoord.x = left + (static_cast<float>(fontMetric.width) / static_cast<float>(fontDesc.textureWidth));
		fontMetric.texCoord.y = top + (static_cast<float>(fontMetric.height) / static_cast<float>(fontDesc.textureHeight));

		//> Insert new FontMetric to the metrics [font.metrics] map
		//	> key = (wchar_t) charId
		//	> value = new FontMetric
		fontDesc.metrics[charId] = fontMetric;
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
