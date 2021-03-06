﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using Mesen.GUI.Controls;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;

namespace Mesen.GUI.Debugger
{
	public partial class ctrlPaletteViewer : BaseControl
	{
		public delegate void SelectionChangedHandler();
		public event SelectionChangedHandler SelectionChanged;

		private byte[] _cgRam;
		private Bitmap _paletteImage;

		public int PaletteScale { get; set; } = 16;
		public int SelectedPalette { get; private set; } = 0;
		public PaletteSelectionMode SelectionMode { get; set; } = PaletteSelectionMode.None;

		public ctrlPaletteViewer()
		{
			InitializeComponent();
			if(IsDesignMode) {
				return;
			}

			_paletteImage = new Bitmap(PaletteScale * 16, PaletteScale * 16, PixelFormat.Format32bppArgb);
			picPalette.Image = _paletteImage;
		}

		public byte[] CgRam
		{
			get { return _cgRam; }
		}

		public void RefreshData()
		{
			_cgRam = DebugApi.GetMemoryState(SnesMemoryType.CGRam);
		}

		private uint To8Bit(int color)
		{
			return (uint)((color << 3) + (color >> 2));
		}

		public uint ToArgb(int rgb555) {
			uint b = To8Bit(rgb555 >> 10);
			uint g = To8Bit((rgb555 >> 5) & 0x1F);
			uint r = To8Bit(rgb555 & 0x1F);

			return (0xFF000000 | (r << 16) | (g << 8) | b);
		}

		public void RefreshViewer()
		{
			UInt32[] argbPalette = new UInt32[256];
			for(int i = 0; i < 256; i++) {
				argbPalette[i] = ToArgb(_cgRam[i * 2] | _cgRam[i * 2 + 1] << 8);
			}

			using(Graphics g = Graphics.FromImage(_paletteImage)) {
				GCHandle handle = GCHandle.Alloc(argbPalette, GCHandleType.Pinned);
				Bitmap source = new Bitmap(16, 16, 16 * 4, PixelFormat.Format32bppArgb, handle.AddrOfPinnedObject());
				try {
					g.InterpolationMode = InterpolationMode.NearestNeighbor;
					g.SmoothingMode = SmoothingMode.None;
					g.PixelOffsetMode = PixelOffsetMode.Half;

					g.ScaleTransform(PaletteScale, PaletteScale);
					g.DrawImage(source, 0, 0);

					g.ResetTransform();
					using(Pen pen = new Pen(Color.LightGray, 2)) {
						pen.DashStyle = DashStyle.Dash;
						if(SelectionMode == PaletteSelectionMode.SingleColor) {
							g.DrawRectangle(pen, (SelectedPalette & 0x0F) * PaletteScale, (SelectedPalette / 16) * PaletteScale, PaletteScale, PaletteScale);
						} else if(SelectionMode == PaletteSelectionMode.FourColors) {
							g.DrawRectangle(pen, (SelectedPalette & 0x03) * PaletteScale * 4, (SelectedPalette / 4) * PaletteScale, PaletteScale * 4, PaletteScale);
						} else if(SelectionMode == PaletteSelectionMode.SixteenColors) {
							g.DrawRectangle(pen, 0, SelectedPalette * PaletteScale, PaletteScale * 16, PaletteScale);
						}
					}
				} finally {
					handle.Free();
				}
			}

			picPalette.Size = _paletteImage.Size;
			picPalette.Invalidate();
		}

		private void picPalette_MouseClick(object sender, MouseEventArgs e)
		{
			int paletteIndex = 0;
			if(SelectionMode == PaletteSelectionMode.SingleColor) {
				paletteIndex = (e.Y / PaletteScale) * 16 + (e.X / PaletteScale);
			} else if(SelectionMode == PaletteSelectionMode.FourColors) {
				paletteIndex += e.X / (4 * PaletteScale);
				paletteIndex += (e.Y / PaletteScale) * 4;
			} else if(SelectionMode == PaletteSelectionMode.SixteenColors) {
				paletteIndex = (e.Y / PaletteScale);
			}
			SelectedPalette = paletteIndex;

			SelectionChanged?.Invoke();

			RefreshViewer();
		}
	}

	public enum PaletteSelectionMode
	{
		None,
		SingleColor,
		FourColors,
		SixteenColors
	}
}
