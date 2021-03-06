﻿using Mesen.GUI.Controls;
using Mesen.GUI.Debugger;
using Mesen.GUI.Utilities;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mesen.GUI.Config
{
	public class DebuggerInfo
	{
		public Size WindowSize = new Size(0, 0);
		public Point WindowLocation;

		public bool ShowByteCode = false;

		public int BreakOnValue = 0;
		public int BreakInCount = 1;
		public BreakInMetric BreakInMetric = BreakInMetric.CpuInstructions;

		public string FontFamily = BaseControl.MonospaceFontFamily;
		public FontStyle FontStyle = FontStyle.Regular;
		public float FontSize = BaseControl.DefaultFontSize;
		public int TextZoom = 100;

		public bool ShowSelectionLength = false;
		public WatchFormatStyle WatchFormat = WatchFormatStyle.Hex;

		public XmlColor CodeOpcodeColor = Color.FromArgb(22, 37, 37);
		public XmlColor CodeLabelDefinitionColor = Color.Blue;
		public XmlColor CodeImmediateColor = Color.Chocolate;
		public XmlColor CodeAddressColor = Color.DarkRed;
		public XmlColor CodeCommentColor = Color.Green;
		public XmlColor CodeEffectiveAddressColor = Color.SteelBlue;

		public XmlColor CodeVerifiedDataColor = Color.FromArgb(255, 252, 236);
		public XmlColor CodeUnidentifiedDataColor = Color.FromArgb(255, 242, 242);
		public XmlColor CodeUnexecutedCodeColor = Color.FromArgb(225, 244, 228);

		public XmlColor CodeExecBreakpointColor = Color.FromArgb(140, 40, 40);
		public XmlColor CodeWriteBreakpointColor = Color.FromArgb(40, 120, 80);
		public XmlColor CodeReadBreakpointColor = Color.FromArgb(40, 40, 200);
		public XmlColor CodeActiveStatementColor = Color.Yellow;
	}
	
	public enum BreakInMetric
	{
		CpuInstructions,
		PpuCycles,
		Scanlines,
		Frames
	}
}
