#pragma once
#include "stdafx.h"
#include "PpuTypes.h"
#include "../Utilities/ISerializable.h"

class Console;
class InternalRegisters;

struct SpriteInfo
{
	uint8_t Index;
	int16_t X;
	uint8_t Y;
	bool HorizontalMirror;
	bool VerticalMirror;
	uint8_t Priority;

	uint8_t TileColumn;
	uint8_t TileRow;
	uint8_t Palette;
	bool UseSecondTable;
	uint8_t LargeSprite;
};

class Ppu : public ISerializable
{
public:
	constexpr static uint32_t SpriteRamSize = 544;
	constexpr static uint32_t CgRamSize = 512;
	constexpr static uint32_t VideoRamSize = 0x10000;

private:
	constexpr static int SpriteLayerIndex = 4;
	constexpr static int ColorWindowIndex = 5;

	shared_ptr<Console> _console;
	shared_ptr<InternalRegisters> _regs;

	bool _forcedVblank = false;
	uint8_t _screenBrightness = 0;

	uint16_t _cycle = 0;
	uint16_t _scanline = 0;
	uint32_t _frameCount = 0;
	uint8_t _oddFrame = 0;

	uint16_t _drawStartX = 0;
	uint16_t _drawEndX = 0;
	uint8_t _irqDelay = 0;
	
	uint8_t _bgMode = 0;
	bool _mode1Bg3Priority = false;
	
	uint8_t _mainScreenLayers = 0;
	uint8_t _subScreenLayers = 0;
	LayerConfig _layerConfig[4];

	WindowConfig _window[2];
	WindowMaskLogic _maskLogic[6];
	bool _windowMaskMain[5];
	bool _windowMaskSub[5];

	Mode7Config _mode7;
	
	uint8_t *_vram;
	uint16_t _vramAddress;
	uint8_t _vramIncrementValue;
	uint8_t _vramAddressRemapping;
	bool _vramAddrIncrementOnSecondReg;
	uint16_t _vramReadBuffer = 0;
	
	uint8_t _ppu1OpenBus = 0;
	uint8_t _ppu2OpenBus = 0;

	uint16_t _cgramAddress;
	uint8_t _cgram[Ppu::CgRamSize];

	uint16_t *_outputBuffers[2];
	uint16_t *_currentBuffer;

	SpriteInfo _sprites[33] = {};
	uint8_t _spriteCount = 0;
	uint8_t _spritePriority[256] = {};
	uint8_t _spritePalette[256] = {};
	uint16_t _spritePixels[256] = {};

	uint16_t _pixelsDrawn = 0;
	uint16_t _subPixelsDrawn = 0;

	uint8_t _rowPixelFlags[256];
	uint16_t _mainScreenBuffer[256];

	bool _subScreenFilled[256];
	uint16_t _subScreenBuffer[256];

	uint16_t _mosaicColor[4][2][256] = {};
	uint8_t _mosaicSize = 0;
	uint8_t _mosaicEnabled = 0;
	uint16_t _mosaicStartScanline = 0;

	uint8_t _oamMode = 0;
	uint16_t _oamBaseAddress = 0;
	uint16_t _oamAddressOffset = 0;

	uint8_t _oamRam[Ppu::SpriteRamSize];
	uint16_t _oamRamAddress = 0;
	bool _enableOamPriority = false;
	
	uint16_t _oamRenderAddress = 0;
	uint16_t _internalOamAddress = 0;
	uint8_t _oamWriteBuffer = 0;

	bool _timeOver = false;
	bool _rangeOver = false;

	bool _hiResMode = false;
	bool _screenInterlace = false;
	bool _objInterlace = false;
	bool _overscanMode = false;
	bool _directColorMode = false;

	ColorWindowMode _colorMathClipMode = ColorWindowMode::Never;
	ColorWindowMode _colorMathPreventMode = ColorWindowMode::Never;
	bool _colorMathAddSubscreen = false;
	uint8_t _colorMathEnabled = 0;
	bool _colorMathSubstractMode = false;
	bool _colorMathHalveResult = false;
	uint16_t _fixedColor = 0;

	uint8_t _hvScrollLatchValue = 0;
	uint8_t _hScrollLatchValue = 0;

	uint16_t _horizontalLocation = 0;
	bool _horizontalLocToggle = false;
	uint16_t _verticalLocation = 0;
	bool _verticalLocationToggle = false;
	bool _locationLatched = false;

	void EvaluateNextLineSprites();
	
	template<uint8_t priority, bool forMainScreen>
	void RenderSprites();

	template<bool forMainScreen> void RenderMode0();
	template<bool forMainScreen> void RenderMode1();
	template<bool forMainScreen> void RenderMode2();
	template<bool forMainScreen> void RenderMode3();
	template<bool forMainScreen> void RenderMode4();
	template<bool forMainScreen> void RenderMode5();
	template<bool forMainScreen> void RenderMode6();
	template<bool forMainScreen> void RenderMode7();

	void RenderScanline();

	template<bool forMainScreen>
	void RenderBgColor();

	template<uint8_t layerIndex, uint8_t bpp, bool processHighPriority, bool forMainScreen, uint16_t basePaletteOffset = 0>
	__forceinline void RenderTilemap();
	
	template<uint8_t layerIndex, uint8_t bpp, bool processHighPriority, bool forMainScreen, uint16_t basePaletteOffset, bool hiResMode>
	__forceinline void RenderTilemap();

	template<uint8_t layerIndex, uint8_t bpp, bool processHighPriority, bool forMainScreen, uint16_t basePaletteOffset, bool hiResMode, bool largeTileWidth, bool largeTileHeight>
	__forceinline void RenderTilemap();

	template<uint8_t layerIndex, uint8_t bpp, bool processHighPriority, bool forMainScreen, uint16_t basePaletteOffset, bool hiResMode, bool largeTileWidth, bool largeTileHeight, uint8_t activeWindowCount>
	__forceinline void RenderTilemap();

	template<uint8_t layerIndex, uint8_t bpp, bool processHighPriority, bool forMainScreen, uint16_t basePaletteOffset, bool hiResMode, bool largeTileWidth, bool largeTileHeight, uint8_t activeWindowCount, bool applyMosaic>
	__forceinline void RenderTilemap();

	template<uint8_t layerIndex, uint8_t bpp, bool processHighPriority, bool forMainScreen, uint16_t basePaletteOffset, bool hiResMode, bool largeTileWidth, bool largeTileHeight, uint8_t activeWindowCount, bool applyMosaic, bool directColorMode>
	void RenderTilemap();

	template<uint8_t layerIndex, bool largeTileWidth, bool largeTileHeight>
	void ProcessOffsetMode(uint8_t x, uint16_t realX, uint16_t realY, uint16_t &hScroll, uint16_t &vScroll, uint16_t &addr);

	template<bool forMainScreen>
	__forceinline bool IsRenderRequired(uint8_t layerIndex);

	template<uint8_t bpp>
	__forceinline uint16_t GetTilePixelColor(const uint16_t pixelStart, const uint8_t shift);

	template<uint8_t layerIndex, bool forMainScreen, bool processHighPriority>
	__forceinline void RenderTilemapMode7();

	template<uint8_t layerIndex, bool forMainScreen, bool processHighPriority, bool applyMosaic>
	__forceinline void RenderTilemapMode7();

	template<uint8_t layerIndex, bool forMainScreen, bool processHighPriority, bool applyMosaic, bool directColorMode>
	void RenderTilemapMode7();

	__forceinline void DrawMainPixel(uint8_t x, uint16_t color, uint8_t flags);
	__forceinline void DrawSubPixel(uint8_t x, uint16_t color);

	void ApplyColorMath();
	void ApplyColorMathToPixel(uint16_t &pixelA, uint16_t pixelB, int x, bool isInsideWindow);
	
	template<bool forMainScreen>
	void ApplyBrightness();

	void ApplyHiResMode();

	template<uint8_t layerIndex>
	bool ProcessMaskWindow(uint8_t activeWindowCount, int x);
	void ProcessWindowMaskSettings(uint8_t value, uint8_t offset);

	void UpdateVramReadBuffer();
	uint16_t GetVramAddress();

	void SendFrame();

	bool IsDoubleHeight();
	bool IsDoubleWidth();

public:
	Ppu(shared_ptr<Console> console);
	virtual ~Ppu();

	void Reset();

	uint32_t GetFrameCount();
	uint16_t GetScanline();
	uint16_t GetCycle();
	PpuState GetState();

	void Exec();

	uint16_t* GetScreenBuffer();
	uint8_t* GetVideoRam();
	uint8_t* GetCgRam();
	uint8_t* GetSpriteRam();

	void LatchLocationValues();

	uint8_t Read(uint16_t addr);
	void Write(uint32_t addr, uint8_t value);

	void Serialize(Serializer &s) override;
};