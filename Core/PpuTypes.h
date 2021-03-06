#pragma once
#include "stdafx.h"

struct LayerConfig
{
	uint16_t TilemapAddress;
	uint16_t ChrAddress;

	uint16_t HScroll;
	uint16_t VScroll;

	bool DoubleWidth;
	bool DoubleHeight;

	bool LargeTiles;
};

struct Mode7Config
{
	int16_t Matrix[4];

	int16_t HScroll;
	int16_t VScroll;
	int16_t CenterX;
	int16_t CenterY;

	uint8_t ValueLatch;
	
	bool LargeMap;
	bool FillWithTile0;
	bool HorizontalMirroring;
	bool VerticalMirroring;
	bool ExtBgEnabled;
};

struct PpuState
{
	uint16_t Cycle;
	uint16_t Scanline;
	uint32_t FrameCount;
	bool OverscanMode;

	uint8_t BgMode;
	bool DirectColorMode;
	Mode7Config Mode7;
	LayerConfig Layers[4];
};

struct WindowConfig
{
	bool ActiveLayers[6];
	bool InvertedLayers[6];
	uint8_t Left;
	uint8_t Right;

	template<uint8_t layerIndex>
	bool PixelNeedsMasking(int x)
	{
		if(InvertedLayers[layerIndex]) {
			if(Left > Right) {
				return true;
			} else {
				return x < Left || x > Right;
			}
		} else {
			if(Left > Right) {
				return false;
			} else {
				return x >= Left && x <= Right;
			}
		}
	}
};

enum class  WindowMaskLogic
{
	Or = 0,
	And = 1,
	Xor = 2,
	Xnor = 3
};

enum class ColorWindowMode
{
	Never = 0,
	OutsideWindow = 1,
	InsideWindow = 2,
	Always = 3
};

enum PixelFlags
{
	Filled = 0x01,
	AllowColorMath = 0x02,
};